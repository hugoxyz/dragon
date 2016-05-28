//
//  MeshComponent.hpp
//  dragon
//
//  Created by hugo on 16/5/6.
//
//

#ifndef MeshComponent_hpp
#define MeshComponent_hpp

#include <vector>

#include "glfw3.h"
#include "Component.hpp"
#include "GLStructDefine.h"
#include "../rendererModule/GLProgram.hpp"
#include "GLData.hpp"

namespace dragon {
    class MeshComponent : public Component {
    public:
        MeshComponent();
        ~MeshComponent();

//        void createVertexesIf(int length);
//        void addVertex(const gl::Vertex& vertex);
//        gl::Vertex* getVertex();
        
        void addVertexes(GLData* v);
        void addVertexIndex(GLData* idx);
        
        void setShaderPath(const std::string& vshader_path, const std::string& fshader_path);
        
        virtual void onResume() override;
        virtual void onPreUpdate() override;
        virtual void onUpdate() override;
        virtual void onAfterUpdate() override;
        virtual void onSuspend() override;

        void onCameraProject(int event, Object* data, Object* userData);
        void onCameraView(int event, Object* data, Object* userData);

    protected:
//        gl::Vertex* vertexes;
//        int vertexLength;
//        int vertexCapacity;

        GLData* vertexes;
        std::vector<GLData*> vertexIndexVec;

        GLuint glBuffer;
        bool glBufferInvalid;
        
        std::string vshader_path;
        std::string fshader_path;
        GLProgram* program;
        
        int cameraObserveTag;
        int cameraViewObserveTag;
        bool projectMatrixDirty;
        bool viewMatrixDirty;
        bool moduleMatrixDirty;

    };
}

#endif /* MeshComponent_hpp */
