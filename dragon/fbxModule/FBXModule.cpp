//
//  UIModule.cpp
//  dragon
//
//  Created by hugo on 16/4/13.
//
//

#include <stdlib.h>

#include "FBXModule.hpp"
#include "Manager.hpp"
#include "FBXMessage.hpp"
#include "Logger.hpp"
#include "../core/UUID.hpp"
#include "../rendererModule/EventRenderer.cpp"
#include "../core/EventComponent.hpp"
#include "../core/EventInput.hpp"
#include "../core/MeshComponent.hpp"

namespace dragon {
    
    FBXModule::FBXModule()
    : fbxManager(nullptr)
    , fbxScene(nullptr)
    , fbxPassword("")
    , rendererNode(nullptr) {
        name = "__fbx_mod";
        waitingPaths.clear();
        
        EventComponent* event = Manager::getInstance()->getComponent<EventComponent>();
        if (nullptr != event) {
            EventComponent::EventObserverFun cb = std::bind(&FBXModule::onInputEvent, this,
                                                            std::placeholders::_1,
                                                            std::placeholders::_2,
                                                            std::placeholders::_3);
            event->addObserver(static_cast<int>(EventComponent::Event::EVENT_INPUT), cb);
        }
    }

    FBXModule::~FBXModule() {
        if (nullptr != fbxManager) {
            fbxManager->Destroy();
            fbxManager = nullptr;
        }
    }

    void FBXModule::addWaitingPath(const std::string& path) {
        waitingPaths.push_back(path);
    }
    
    std::string FBXModule::getWaitingPath() {
        std::string path = *waitingPaths.begin();
        waitingPaths.erase(waitingPaths.begin());
        return path;
    }
    
    void FBXModule::onUpdate() {
        if (waitingPaths.size() > 0) {
            parserFBX(getWaitingPath());
        }
    }

    void FBXModule::onMessage(Message* msg) {
        if (nullptr == msg) {
            return;
        }
        FBXMessage* fbxMsg = dynamic_cast<FBXMessage*>(msg);
        if (nullptr == fbxMsg) {
            LOGD("FBXModule", "trans to FBXMessage fail");
            return;
        }

        std::string path = fbxMsg->getFilePath();
        if (!path.empty()) {
            addWaitingPath(path);
        }
    }
    
    void FBXModule::onInputEvent(int event, Object* data, Object* userData) {
        if (event == static_cast<int>(EventComponent::Event::EVENT_INPUT)) {
            EventInput* event = dynamic_cast<EventInput*>(data);
            if (nullptr == event) {
                return;
            }
            switch (event->getEventType()) {
                case EventInput::EventInputType::DROP_FILE: {
                    auto paths = event->getPaths();
                    for (auto path : paths) {
                        addWaitingPath(path);
                    }
                    break;
                }
                default: {
                    LOGD("FBXModule", "unknow eventInput:%d", event->getEventType());
                    break;
                }
            }
        } else {
            LOGD("FBXModule", "unknow event:%d", event);
        }

    }
    
    bool FBXModule::parserFBX(const std::string& file) {
        LOGD("FBXModule", "parser fbx: %s", file.c_str());
        
        if (!createFBX()) {
            return false;
        }
        
        bool bRet = importFBX(file);
        if (!bRet) {
            LOGD("FBXModule", "import fbx file fail");
            return false;
        }

        if (nullptr != rendererNode) {
            rendererNode->release();
            rendererNode = nullptr;
        }
        rendererNode = new Node();
        rendererNode->createTransformIf();
        rendererNode->retain();
        processFBXNode(fbxScene->GetRootNode());

        return true;
    }
    
    bool FBXModule::createFBX() {
        if (nullptr != fbxManager) {
            fbxManager->Destroy();
            fbxManager = nullptr;
        }

        fbxManager = FbxManager::Create();
        if (nullptr == fbxManager) {
            LOGD("FBXModule", "create fbx manager fail");
            return false;
        } else {
            LOGD("FBXModule", "fbx version: %s", fbxManager->GetVersion());
        }
        
        FbxIOSettings* fbxIOSetting = FbxIOSettings::Create(fbxManager, IOSROOT);
        fbxManager->SetIOSettings(fbxIOSetting);
        
        fbxScene = FbxScene::Create(fbxManager, "Fbx Scene");
        if (nullptr == fbxScene) {
            LOGD("FBXModule", "create scene fail");
            if (nullptr != fbxManager) {
                fbxManager->Destroy();
                fbxManager = nullptr;
            }
            return false;
        }
        
        return true;
    }
    
    bool FBXModule::importFBX(const std::string &file) {
        FbxString fbxFilePath = FbxString(file.c_str());
        int sdkMajor, sdkMinor, sdkRevision;
        int fileMajor, fileMinor, fileRevision;
        FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);
        LOGD("FBXModule", "fbx sdk version is %d.%d.%d", sdkMajor, sdkMinor, sdkRevision);
        
        FbxIOSettings* fbxIOSetting = fbxManager->GetIOSettings();
        FbxImporter *fbxImporter = FbxImporter::Create(fbxManager, "fbx importer");
        bool importStatus = fbxImporter->Initialize(fbxFilePath.Buffer(), -1, fbxIOSetting);
        fbxImporter->GetFileVersion(fileMajor, fileMinor, fileRevision);
        if (!importStatus) {
            FbxString error = fbxImporter->GetStatus().GetErrorString();
            LOGD("FBXModule", "import fbx file fail:%s", error.Buffer());
            
            if (nullptr != fbxManager) {
                fbxManager->Destroy();
            }
            return false;
        }
        
        if (fbxImporter->IsFBX()) {
            LOGD("FBXModule", "fbx (%s) version is %d.%d.%d", fbxFilePath.Buffer(), fileMajor, fileMinor, fileRevision);
        } else {
            LOGD("FBXModule", "fbx (%s) format is not fbx", fbxFilePath.Buffer());
            return false;
        }
        
        if (!fbxPassword.empty()) {
            fbxIOSetting->SetStringProp(IMP_FBX_PASSWORD, FbxString(fbxPassword.c_str()));
            fbxIOSetting->SetStringProp(IMP_FBX_PASSWORD_ENABLE, true);
        }
        
        importStatus = fbxImporter->Import(fbxScene);
        fbxImporter->Destroy();
        if (!importStatus) {
            LOGD("FBXModule", "import fbx to scene fail");
            return false;
        }
        
        //trans all mesh to trangulate
        FbxGeometryConverter converter(fbxManager);
        converter.Triangulate(fbxScene, true);

        return true;
    }
    
    void FBXModule::processFBXNode(FbxNode *node) {
        if (nullptr == node) {
            return;
        }
        
        FbxNodeAttribute* attr = node->GetNodeAttribute();
        if (attr) {
            switch (attr->GetAttributeType()) {
                case FbxNodeAttribute::EType::eMesh: {
                    processMesh(node);
                    break;
                }
                default: {
                    LOGD("FBXModule", "processFBXNode not support type %d", attr->GetAttributeType());
                    break;
                }
            }
        }
        
        //process child
        for (int i = 0; i < node->GetChildCount(); i ++) {
            processFBXNode(node->GetChild(i));
        }
    }
    
    void FBXModule::processMesh(FbxNode *node) {
        if (nullptr == node) {
            return;
        }

        FbxMesh* fbxMesh = node->GetMesh(); // dynamic_cast<FbxMesh*>(node->GetNodeAttribute());
        if (nullptr == fbxMesh) {
            return;
        }
        LOGD("FBXModule", "processMesh: %s", fbxMesh->GetName());
        if(!fbxMesh->IsTriangleMesh()) {
            LOGE("FBXModule", "processMesh: mesh is not mesh");
            return;
        }
        
        int triangleCount = fbxMesh->GetPolygonCount();
        LOGD("FBXModule", "mesh triangel count: %d", triangleCount);
        int vertexCounter = 0;

        EventRenderer *event = new EventRenderer();
        EventRenderer::Color color;
        EventRenderer::Normal normal;
        EventRenderer::UV uv;
        bool haveValue = false;
        gl::Vertex vertex;

        MeshComponent *meshComp = new MeshComponent();
        meshComp->createVertexesIf(triangleCount * 3);
        meshComp->setShaderPath("./resources/shader/default.vsh", "./resources/shader/default.fsh");
        FbxVector4* fbxControlPoints = fbxMesh->GetControlPoints();
        for(int i = 0 ; i < triangleCount ; ++i) {
            int polygonSize = fbxMesh->GetPolygonSize(i);
            if (polygonSize > 3) {
                LOGE("FBXModule", "processMesh: polygon is not triangle");
                break;
            }
            for(int j = 0 ; j < 3 ; j++) {
                int ctrlPointIndex = fbxMesh->GetPolygonVertex(i , j);

                // Read the vertex
                double x = fbxControlPoints[ctrlPointIndex][0];
                double y = fbxControlPoints[ctrlPointIndex][1];
                double z = fbxControlPoints[ctrlPointIndex][2];
                
                vertex.x = x;
                vertex.y = y;
                vertex.z = z;
                meshComp->addVertex(vertex);

                //read color
                if (fbxMesh->GetElementVertexColorCount() > 0) {
                    FbxGeometryElementVertexColor* pVertexColor = fbxMesh->GetElementVertexColor();
                    haveValue = false;
                    switch (pVertexColor->GetMappingMode()) {
                        case FbxGeometryElement::EMappingMode::eByControlPoint: {
                            switch(pVertexColor->GetReferenceMode()) {
                                case FbxGeometryElement::EReferenceMode::eDirect: {
                                    double r = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;
                                    double g = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;
                                    double b = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;
                                    double a = pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;
                                    color.r = r;
                                    color.g = g;
                                    color.b = b;
                                    color.a = a;
                                    haveValue = true;

                                    break;
                                }
                                case FbxGeometryElement::EReferenceMode::eIndexToDirect: {
                                    int id = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);
                                    double r = pVertexColor->GetDirectArray().GetAt(id).mRed;
                                    double g = pVertexColor->GetDirectArray().GetAt(id).mGreen;
                                    double b = pVertexColor->GetDirectArray().GetAt(id).mBlue;
                                    double a = pVertexColor->GetDirectArray().GetAt(id).mAlpha;
                                    color.r = r;
                                    color.g = g;
                                    color.b = b;
                                    color.a = a;
                                    haveValue = true;
                                    
                                    break;
                                }
                                default: {
                                    break;
                                }
                            }
                            break;
                        }
                        case FbxGeometryElement::EMappingMode::eByPolygonVertex: {
                            switch (pVertexColor->GetReferenceMode()) {
                                case FbxGeometryElement::EReferenceMode::eDirect: {
                                    double r = pVertexColor->GetDirectArray().GetAt(vertexCounter).mRed;
                                    double g = pVertexColor->GetDirectArray().GetAt(vertexCounter).mGreen;
                                    double b = pVertexColor->GetDirectArray().GetAt(vertexCounter).mBlue;
                                    double a = pVertexColor->GetDirectArray().GetAt(vertexCounter).mAlpha;
                                    color.r = r;
                                    color.g = g;
                                    color.b = b;
                                    color.a = a;
                                    haveValue = true;
                                    
                                    break;
                                }
                                case FbxGeometryElement::EReferenceMode::eIndexToDirect: {
                                    int id = pVertexColor->GetIndexArray().GetAt(vertexCounter);
                                    double r = pVertexColor->GetDirectArray().GetAt(id).mRed;
                                    double g = pVertexColor->GetDirectArray().GetAt(id).mGreen;
                                    double b = pVertexColor->GetDirectArray().GetAt(id).mBlue;
                                    double a = pVertexColor->GetDirectArray().GetAt(id).mAlpha;
                                    color.r = r;
                                    color.g = g;
                                    color.b = b;
                                    color.a = a;
                                    haveValue = true;
                                    
                                    break;
                                }
                                default: {
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    if (haveValue) {
                        event->appendColor(color);
                    }
                }

                // 每个顶点对应的 uv, normal 可能不至一个，可能会有多个通道
                // fbxMesh->GetElementUVCount() 对应的就是通道数量
                int layer_count = fbxMesh->GetElementUVCount();
                if (layer_count > 1) {
                    layer_count = 1; //现在暂只用第一个通道的值
                }
                for (int idx = 0; idx < layer_count; idx++) {
                    const FbxGeometryElementUV* fbxUV = fbxMesh->GetElementUV(idx);
                    haveValue = false;
                    switch (fbxUV->GetMappingMode()) {
                        case FbxGeometryElement::EMappingMode::eByControlPoint: {
                            switch (fbxUV->GetReferenceMode()) {
                                case FbxGeometryElement::EReferenceMode::eDirect: {
                                    double u = fbxUV->GetDirectArray().GetAt(ctrlPointIndex)[0];
                                    double v = fbxUV->GetDirectArray().GetAt(ctrlPointIndex)[1];
                                    uv.u = u;
                                    uv.v = v;
                                    haveValue = true;
                                    break;
                                }
                                case FbxGeometryElement::EReferenceMode::eIndexToDirect: {
                                    int id = fbxUV->GetIndexArray().GetAt(ctrlPointIndex);
                                    double u = fbxUV->GetDirectArray().GetAt(id)[0];
                                    double v = fbxUV->GetDirectArray().GetAt(id)[1];
                                    uv.u = u;
                                    uv.v = v;
                                    haveValue = true;
                                    break;
                                }
                                default: {
                                    break;
                                }
                            }
                            break;
                        }
                        case FbxGeometryElement::EMappingMode::eByPolygonVertex: {
                            switch (fbxUV->GetReferenceMode()) {
                                case FbxGeometryElement::EReferenceMode::eDirect:
                                case FbxGeometryElement::EReferenceMode::eIndexToDirect: {
                                    int uvIdx = fbxMesh->GetTextureUVIndex(i, j);
                                    double u = fbxUV->GetDirectArray().GetAt(uvIdx)[0];
                                    double v = fbxUV->GetDirectArray().GetAt(uvIdx)[1];
                                    uv.u = u;
                                    uv.v = v;
                                    haveValue = true;
                                    break;
                                }
                                default: {
                                    break;
                                }
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    if (haveValue) {
                        event->appendUV(uv);
                    }
                }
                
                layer_count = fbxMesh->GetElementNormalCount();
                if (layer_count > 1) {
                    layer_count = 1; //现在暂只用第一个通道的值
                }
                for(int idx = 0; idx < layer_count; idx++) {
                    FbxGeometryElementNormal* fbxNormal = fbxMesh->GetElementNormal(idx);
                    haveValue = false;
                    switch (fbxNormal->GetMappingMode()) {
                        case FbxGeometryElement::EMappingMode::eByControlPoint: {
                            switch (fbxNormal->GetReferenceMode()) {
                                case FbxGeometryElement::EReferenceMode::eDirect: {
                                    double x = fbxNormal->GetDirectArray().GetAt(ctrlPointIndex)[0];
                                    double y = fbxNormal->GetDirectArray().GetAt(ctrlPointIndex)[1];
                                    double z = fbxNormal->GetDirectArray().GetAt(ctrlPointIndex)[2];
                                    normal.x = x;
                                    normal.y = y;
                                    normal.z = z;
                                    haveValue = true;
                                    break;
                                }
                                case FbxGeometryElement::EReferenceMode::eIndexToDirect: {
                                    int id = fbxNormal->GetIndexArray().GetAt(ctrlPointIndex);
                                    double x = fbxNormal->GetDirectArray().GetAt(id)[0];
                                    double y = fbxNormal->GetDirectArray().GetAt(id)[1];
                                    double z = fbxNormal->GetDirectArray().GetAt(id)[2];
                                    normal.x = x;
                                    normal.y = y;
                                    normal.z = z;
                                    haveValue = true;
                                    break;
                                }
                                default:
                                    break;
                            }
                            break;
                        }
                        case FbxGeometryElement::EMappingMode::eByPolygonVertex: {
                            switch (fbxNormal->GetReferenceMode()) {
                                case FbxGeometryElement::EReferenceMode::eDirect: {
                                    double x = fbxNormal->GetDirectArray().GetAt(vertexCounter)[0];
                                    double y = fbxNormal->GetDirectArray().GetAt(vertexCounter)[1];
                                    double z = fbxNormal->GetDirectArray().GetAt(vertexCounter)[2];
                                    normal.x = x;
                                    normal.y = y;
                                    normal.z = z;
                                    haveValue = true;
                                    break;
                                }
                                case FbxGeometryElement::EReferenceMode::eIndexToDirect: {
                                    int id = fbxNormal->GetIndexArray().GetAt(vertexCounter);
                                    double x = fbxNormal->GetDirectArray().GetAt(id)[0];
                                    double y = fbxNormal->GetDirectArray().GetAt(id)[1];
                                    double z = fbxNormal->GetDirectArray().GetAt(id)[2];
                                    normal.x = x;
                                    normal.y = y;
                                    normal.z = z;
                                    haveValue = true;
                                    break;
                                }
                                default:
                                    break;
                            }
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                    if (haveValue) {
                        event->appendNormal(normal);
                    }
                }

                vertexCounter++;  
            }
        }
        if (rendererNode) {
            rendererNode->addComponent(meshComp);
        }
        Manager::getInstance()->postEvent(static_cast<int>(EventComponent::Event::EVENT_RENDERER_NODE), rendererNode);

        //取 纹理与材质 的相联表, 现在只取第一层的
        int layerCount = fbxMesh->GetElementMaterialCount();
        if (layerCount > 1) {
            layerCount = 1;
        }
        for (int layer = 0; layer < layerCount; layer++) {
            FbxGeometryElementMaterial* fbxMaterial = fbxMesh->GetElementMaterial(layer);
            if (nullptr != fbxMaterial) {
                auto& materialIndex = fbxMaterial->GetIndexArray();
                if (triangleCount == materialIndex.GetCount()) {
                    int* index = event->createMaterialMeshAssociate(triangleCount);
                    for (int k = 0; k < triangleCount; k++) {
                        index[k] = materialIndex[k];
                    }
                } else {
                    LOGD("FBXModule", "material count not same with triangle");
                }
            }
        }
        
//        Manager::getInstance()->postEvent(static_cast<int>(EventComponent::Event::EVENT_RENDERER), event);
        
        // 根据读入的信息组装三角形，并以某种方式使用即可，比如存入到列表中、保存到文件等...
    }
    
    void FBXModule::processMaterial(FbxMesh* node, int triangleCount) {
        
    }

}