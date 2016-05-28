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
        
        const rapidjson::Value& meshes = d["meshes"];
        for (auto it = meshes.Begin(); it != meshes.End(); it++) {
            MeshComponent* comp = new MeshComponent();
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
                comp->addVertexIndex(memory);
            }
            
            node->addComponent(comp);
        }

        Manager::getInstance()->postEvent(static_cast<int>(EventComponent::Event::EVENT_RENDERER_NODE), node);

        return node;
    }
    
    
}