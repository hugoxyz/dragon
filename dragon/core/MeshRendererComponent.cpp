//
//  MeshRendererComponent.cpp
//  dragon
//
//  Created by hugo on 16/6/17.
//
//

#include "FileUtils.hpp"
#include "Utils.hpp"
#include "Node.hpp"
#include "MeshRendererComponent.hpp"
#include "MaterialComponent.hpp"
#include "CameraComponent.hpp"
#include "Manager.hpp"

namespace dragon {
    MeshRendererComponent::MeshRendererComponent()
    : projection("perspective") {
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
        if (nullptr == host) {
            return;
        }

        Node* cameraNode = Manager::getInstance()->getChild("__camera_node");
        CameraComponent* cameraComp = cameraNode->getComponent<CameraComponent>();
        CameraComponent::Projection stage_proj = cameraComp->getProjection();
        if (0 == Utils::compare(projection, "orthogonal")) {
            cameraComp->setProjection(CameraComponent::Projection::Orthogonal);
        } else {
            cameraComp->setProjection(CameraComponent::Projection::Perspective);
        }

        MeshComponent* mesh = host->getComponent<MeshComponent>();
        if (nullptr == mesh) {
            return;
        }
        for (auto it : parts) {
            MaterialComponent* material = dynamic_cast<MaterialComponent*>(host->getComponent(it.second));
            if (nullptr == material) {
                continue;
            }
            GLProgram *program = material->getGLProgram();
            host->apply(program);
            cameraComp->apply(program);
            
            glBindBuffer(GL_ARRAY_BUFFER, mesh->getBufferLocation());
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBufferLocation(it.first));
            glDrawElements(GL_TRIANGLES, mesh->getIndexBufferSize(it.first), GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        
        cameraComp->setProjection(stage_proj);
    }
    
    void MeshRendererComponent::parser(const rapidjson::Value &json) {
        auto partsIt = json.FindMember("projection");
        if (partsIt != json.MemberEnd()) {
            projection = partsIt->value.GetString();
        }

        partsIt = json.FindMember("parts");
        if (partsIt != json.MemberEnd()) {
            const auto& parts = partsIt->value;
            for (int i = 0; i < parts.Size(); i++ ) {
                const auto& part = parts[i];
                addPart(part["mesh"].GetString(), part["material"].GetString());
            }
        }
    }
}
