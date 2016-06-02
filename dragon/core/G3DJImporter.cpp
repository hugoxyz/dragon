//
//  G3DJImporter.cpp
//  dragon
//
//  Created by hugo on 16/5/24.
//
//

#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "G3DJImporter.hpp"
#include "Utils.hpp"

#include "Logger.hpp"
#include "Manager.hpp"
#include "MeshComponent.hpp"
#include "EventComponent.hpp"
#include "RotateComponent.hpp"

namespace dragon {
    Node* G3DJImporter::import(const std::string &file) {
        Node* node = nullptr;
        node = new Node();
        node->createTransformIf();
        
        // parser json
        std::ifstream ifs(file);
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document d;
        d.ParseStream(isw);
        
        std::map<std::string, GLData*> vertexIdxMap;
        const rapidjson::Value& meshes = d["meshes"];
        MeshComponent* comp = new MeshComponent();
        for (auto it = meshes.Begin(); it != meshes.End(); it++) {
            comp->setShaderPath("./resources/shader/default.vsh", "./resources/shader/default.fsh");
            
            const rapidjson::Value& mesh = *it;
            const rapidjson::Value& attr = mesh["attributes"];
            int unitSize = 0;
            bool bPos = false;
            bool bNormal = false;
            bool bUV = false;
            
            for (auto it = attr.Begin(); it != attr.End(); it++) {
                if (0 == Utils::compare(it->GetString(), "POSITION")) {
                    bPos = true;
                } else if (0 == Utils::compare(it->GetString(), "NORMAL")) {
                    bNormal = true;
                } else if (0 == Utils::compare(it->GetString(), "TEXCOORD0")) {
                    bUV = true;
                }
            }
            if (bPos) {
                unitSize += sizeof(gl::Vertex);
            }
            if (bNormal) {
                unitSize += sizeof(gl::Normal);
            }
            if (bUV) {
                unitSize += sizeof(gl::UV);
            }
            const rapidjson::Value& val = mesh["vertices"];
            gl::Vertex v;
            gl::Normal n;
            gl::UV uv;
            auto valPos = val.Begin();
            GLData* vertexesMem = new GLData();
            vertexesMem->setUnitSize(unitSize);
            do {
                if (bPos) {
                    if (valPos == val.End()) {
                        break;
                    }
                    v.x = valPos->GetFloat();
                    valPos++;
                    if (valPos == val.End()) {
                        break;
                    }
                    v.y = valPos->GetFloat();
                    valPos++;
                    if (valPos == val.End()) {
                        break;
                    }
                    v.z = valPos->GetFloat();
                    valPos++;
                }
                if (bNormal) {
                    if (valPos == val.End()) {
                        break;
                    }
                    n.x = valPos->GetFloat();
                    valPos++;
                    if (valPos == val.End()) {
                        break;
                    }
                    n.y = valPos->GetFloat();
                    valPos++;
                    if (valPos == val.End()) {
                        break;
                    }
                    n.z = valPos->GetFloat();
                    valPos++;
                }
                if (bUV) {
                    if (valPos == val.End()) {
                        break;
                    }
                    uv.u = valPos->GetFloat();
                    valPos++;
                    if (valPos == val.End()) {
                        break;
                    }
                    uv.v = valPos->GetFloat();
                    valPos++;
                }
                void* p = vertexesMem->addMemory(nullptr);
                memcpy(p, &v, sizeof(gl::Vertex));
                p = (char*)p + sizeof(gl::Vertex);
                memcpy(p, &n, sizeof(gl::Normal));
                p = (char*)p + sizeof(gl::Normal);
                memcpy(p, &uv, sizeof(gl::UV));
            } while(true);
            vertexesMem->setVertexOffset(0);
            vertexesMem->setNormalOffset(sizeof(gl::Vertex));
            vertexesMem->setTextureOffset(sizeof(gl::Vertex) + sizeof(gl::UV));
            comp->addVertexes(vertexesMem);
            
            const rapidjson::Value& partsValue = mesh["parts"];
            for(auto partIt = partsValue.Begin(); partIt != partsValue.End(); partIt++) {
                const rapidjson::Value& partValue = * partIt;                
                const rapidjson::Value& indicesValue = partValue["indices"];
                GLData* memory = new GLData();
                memory->setUnitSize(sizeof(int));
                int i = 0;
                for(auto indicesIt = indicesValue.Begin(); indicesIt != indicesValue.End(); indicesIt++) {
                    i = indicesIt->GetInt();
                    memory->addMemory(&i);
                }
                memory->retain();
                vertexIdxMap.insert(std::make_pair(partValue["id"].GetString(), memory));
            }
            
            node->addComponent(comp);
        }
        
        std::map<std::string, Material*> materialMap;
        const rapidjson::Value& materials = d["materials"];
        for (rapidjson::SizeType i = 0; i < materials.Size(); i++) {
            Material* mat = Material::parserFromJosn(materials[i]);
            mat->retain();
            materialMap.insert(std::make_pair(materials[i]["id"].GetString(), mat));
        }
        
        for (rapidjson::SizeType i = 0; i < d["nodes"].Size(); i++) {
            const rapidjson::Value& node = d["nodes"][i];
            for (rapidjson::SizeType i = 0; i < node["parts"].Size(); i++) {
                const rapidjson::Value& part = node["parts"][i];
                comp->addVertexIndex(vertexIdxMap[part["meshpartid"].GetString()]);
                comp->addMaterial(materialMap[part["materialid"].GetString()]);
            }
        }
        
        node->addComponent(new RotateComponent());

        Manager::getInstance()->postEvent(static_cast<int>(EventComponent::Event::EVENT_RENDERER_NODE), node);

        for (auto it : vertexIdxMap) {
            it.second->release();
        }
        vertexIdxMap.clear();
        for (auto it : materialMap) {
            it.second->release();
        }
        materialMap.clear();

        return node;
    }
    
    
}