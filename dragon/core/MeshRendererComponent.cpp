//
//  MeshRendererComponent.cpp
//  dragon
//
//  Created by hugo on 16/6/17.
//
//

#include "MeshRendererComponent.hpp"
#include "FileUtils.hpp"

namespace dragon {
    MeshRendererComponent::MeshRendererComponent() {
        parts.clear();
    }
    
    MeshRendererComponent::~MeshRendererComponent() {
    }

    MeshRendererComponent* MeshRendererComponent::parserFromFile(const std::string& f) {
        std::string file = FileUtils::getInstance()->fullPath(f);
        unsigned char* data = nullptr;
        int dataLen = 0;
        FileUtils::getInstance()->read(file, &data, &dataLen);
        rapidjson::Document d;
        d.Parse((const char*)data, dataLen);

        return MeshRendererComponent::parserFromJosn(d);
    }
    
    MeshRendererComponent* MeshRendererComponent::parserFromJosn(const rapidjson::Value& json) {
        MeshRendererComponent* mat = new MeshRendererComponent();
        mat->parser(json);

        return mat;
    }
    
    void MeshRendererComponent::addPart(std::string meshIdxName, std::string materialName) {
        parts.insert(std::make_pair(meshIdxName, materialName));
    }

    void MeshRendererComponent::onUpdate() {
    }
    
    void MeshRendererComponent::parser(const rapidjson::Value &json) {
        auto partsIt = json.FindMember("parts");
        if (partsIt != json.MemberEnd()) {
            const auto& parts = partsIt->value;
            for (int i = 0; i < parts.Size(); i++ ) {
                const auto& part = parts[i];
                addPart(part["material"].GetString(), part["mesh"].GetString());
            }
        }
    }
}
