//
//  UIModule.hpp
//  dragon
//
//  Created by hugo on 16/4/13.
//
//

#ifndef FBXModule_hpp
#define FBXModule_hpp

#include <vector>

#include "fbxsdk.h"

#include "../core/Module.hpp"

namespace dragon {
    class FBXModule : public Module {
    public:
        FBXModule();
        ~FBXModule();

        virtual bool init();
        virtual void update(int dt);
        virtual void onStateEvent(StateEvent e);
        virtual void onMessage(Message* msg);

    protected:
        void addWaitingPath(const std::string& path);
        std::string getWaitingPath();
        bool parserFBX(const std::string& file);
        
        bool createFBX();
        bool importFBX(const std::string& file);
        void processFBXNode(FbxNode* node);
        void processMesh(FbxNode* node);
        void processMaterial(FbxMesh* node, int triangleCount);
        
        void onInputEvent(int event, Object* data, Object* userData);

    protected:
        std::vector<std::string> waitingPaths;
        
        FbxManager* fbxManager;
        FbxScene* fbxScene;
        std::string fbxPassword;
    };
}

#endif /* UIModule_hpp */
