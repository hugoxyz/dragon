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
#include "Logger.hpp"

#include "Widget.hpp"

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
        file = f;
        
        std::ifstream ifs(file);
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document d;
        d.ParseStream(isw);
        
        Node* n = parserNode(d);
        
        return n;
    }
    
    Node* SceneImporter::parserNode(const rapidjson::Value& json) {
        Node* n = nullptr;
        std::string type = json["type"].GetString();
        
        if (0 == Utils::compare(type, "Widget")) {
            ui::Widget* w = new ui::Widget();

            n = w;
        } else {
            LOGD("SceneImporter", "unsupport type:%s", type.c_str());
        }
        if (nullptr == n) {
            return nullptr;
        }

        // add component
        const rapidjson::Value& components = json["components"];
        for (auto it = components.MemberBegin(); it != components.MemberEnd(); it++) {
            Component* comp = parserComponent(it->name.GetString(), it->value);
            n->addComponent(comp);
        }

        // add child
        const rapidjson::Value& children = json["children"];
        for (int i = 0; i < children.Size(); i++) {
            Node* c = parserNode(children[i]);
            n->addChild(c);
        }

        return n;
    }
    
    Component* SceneImporter::parserComponent(const std::string& name, const rapidjson::Value& json) {
        return nullptr;
    }
}
