//
//  SceneImporter.cpp
//  dragon
//
//  Created by hugo on 16/6/11.
//
//

#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "SceneImporter.hpp"
#include "Utils.hpp"
#include "FileUtils.hpp"
#include "Logger.hpp"

#include "Widget.hpp"

#include "TransformComponent.hpp"

namespace dragon {
    Node* SceneImporter::import(const std::string &file) {
        SceneImporter *import = new SceneImporter();
        
        Node* n = import->parser(file);

        return n;
    }
    
    SceneImporter::SceneImporter()
    : file("") {
    }
    
    Node* SceneImporter::parser(const std::string& f) {
        file = FileUtils::getInstance()->fullPath(f);
        unsigned char* data = nullptr;
        int dataLen = 0;
        FileUtils::getInstance()->read(file, &data, &dataLen);
        rapidjson::Document d;
        d.Parse((const char*)data, dataLen);

        if (rapidjson::Type::kNullType == d.GetType()) {
            return nullptr;
        }
        Node* n = parserNode(d);
        
        FREEIF(data);
        return n;
    }
    
    Node* SceneImporter::parserNode(const rapidjson::Value& json) {
        Node* n = nullptr;
        std::string type = json["type"].GetString();
        
        if (0 == Utils::compare(type, "Widget")) {
            ui::Widget* w = new ui::Widget();
            w->setName(json["name"].GetString());

            n = w;
        } else {
            LOGD("SceneImporter", "unsupport type:%s", type.c_str());
        }
        if (nullptr == n) {
            return nullptr;
        }

        // add component
        auto memberIt = json.FindMember("components");
        if (memberIt != json.MemberEnd()) {
            const rapidjson::Value& components = memberIt->value;
            for (auto it = components.MemberBegin(); it != components.MemberEnd(); it++) {
                Component* comp = parserComponent(it->name.GetString(), it->value);
                n->addComponent(comp);
            }
        }

        // add child
        memberIt = json.FindMember("children");
        if (memberIt != json.MemberEnd()) {
            const rapidjson::Value& children = memberIt->value;
            for (int i = 0; i < children.Size(); i++) {
                Node* c = parserNode(children[i]);
                n->addChild(c);
            }
        }

        return n;
    }
    
    Component* SceneImporter::parserComponent(const std::string& name, const rapidjson::Value& json) {
        Component* comp = nullptr;
        if (0 == Utils::compare(name, "TransformComponent")) {
            TransformComponent* trans = new TransformComponent();
            trans->setPosition(transToVec3(json["position"]));
            trans->setScale(transToVec3(json["scale"]));
            trans->setRotation(transToVec3(json["rotation"]));
            
            comp = trans;
        }
        
        return comp;
    }
    
    glm::vec3 SceneImporter::transToVec3(const rapidjson::Value& json) {
        float x = json["x"].GetFloat();
        float y = json["y"].GetFloat();
        float z = json["z"].GetFloat();
        
        return glm::vec3(x, y, z);
    }
}
