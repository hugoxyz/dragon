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
#include "MeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "MeshRendererComponent.hpp"

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
                auto compVec = parserComponent(it->name.GetString(), it->value);
                for (auto comp : compVec) {
                    n->addComponent(comp, comp->getName());
                }
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
    
    std::vector<Component*> SceneImporter::parserComponent(const std::string& name, const rapidjson::Value& json) {
        std::vector<Component*> compVec;
        Component* comp = nullptr;
        if (0 == Utils::compare(name, "transform")) {
            comp = parserTransform(json);
            if (nullptr != comp) {
                compVec.push_back(comp);
            }
        } else if (0 == Utils::compare(name, "mesh")) {
            comp = parserMesh(json);
            if (nullptr != comp) {
                compVec.push_back(comp);
            }
        } else if (0 == Utils::compare(name, "materials")) {
            for (int i = 0; i < json.Size(); i++) {
                comp = nullptr;
                comp = parserMaterial(json[i]);
                if (nullptr != comp) {
                    compVec.push_back(comp);
                }
            }
        } else if (0 == Utils::compare(name, "renderer")) {
            comp = parserRenderer(json);
            if (nullptr != comp) {
                compVec.push_back(comp);
            }
        }

        return compVec;
    }
    
    glm::vec3 SceneImporter::transToVec3(const rapidjson::Value& json) {
        float x = json[0].GetFloat();
        float y = json[1].GetFloat();
        float z = json[2].GetFloat();
        
        return glm::vec3(x, y, z);
    }
    
    Component* SceneImporter::parserTransform(const rapidjson::Value& json) {
        TransformComponent* trans = new TransformComponent();
        trans->setPosition(transToVec3(json["position"]));
        trans->setScale(transToVec3(json["scale"]));
        trans->setRotation(transToVec3(json["rotation"]));
        
        return trans;
    }
    
    Component* SceneImporter::parserMesh(const rapidjson::Value& json) {
        MeshComponent* mesh = new MeshComponent();
        
        int attrMask = 0;
        const rapidjson::Value& attr = json["attributes"];
        for (auto it = attr.Begin(); it != attr.End(); it++) {
            if (0 == Utils::compare(it->GetString(), "POSITION")) {
                attrMask |= MeshComponent::Attribute::Position;
            } else if (0 == Utils::compare(it->GetString(), "COLOR")) {
                attrMask |= MeshComponent::Attribute::Color;
            } else if (0 == Utils::compare(it->GetString(), "NORMAL")) {
                attrMask |= MeshComponent::Attribute::Normal;
            } else if (0 == Utils::compare(it->GetString(), "TEXCOORD0")) {
                attrMask |= MeshComponent::Attribute::Texcoord0;
            }
        }
        mesh->enableAttribute(attrMask);

        const rapidjson::Value& vertices = json["vertices"];
        void* meshBuffer = mesh->createMeshBuffer(vertices.Size()*sizeof(float));
        float* cur = (float*)meshBuffer;
        for (auto b = vertices.Begin(); b != vertices.End(); b++) {
            *cur = b->GetFloat();
            cur += 1;
        }
        
        const rapidjson::Value& parts = json["parts"];
        for (auto it = parts.Begin(); it != parts.End(); it++) {
            const rapidjson::Value& part = *it;
            const rapidjson::Value& indices = part["indices"];
            int* meshIndexBuffer = (int*)mesh->createMeshIndexBuffer(part["id"].GetString(), indices.Size()*sizeof(int));
            for (auto idx = indices.Begin(); idx != indices.End(); idx++) {
                *meshIndexBuffer = idx->GetInt();
                meshIndexBuffer += 1;
            }
        }

        return mesh;
    }
    
    Component* SceneImporter::parserMaterial(const rapidjson::Value &json) {
        Component* comp = MaterialComponent::parserFromJosn(json);
        return comp;
    }

    Component* SceneImporter::parserRenderer(const rapidjson::Value &json) {
        Component* comp = MeshRendererComponent::parserFromJosn(json);
        return comp;
    }
}
