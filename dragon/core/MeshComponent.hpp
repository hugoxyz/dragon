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

#include "MeshBuffer.hpp"
#include "GLData.hpp"
#include "Material.hpp"

namespace dragon {
    class MeshComponent : public Component {
    public:
        enum Attribute {
            None = 0,
            Position = 1,
            Color = 1 << 1,
            Normal = 1 << 2,
            Texcoord0 = 1 << 3,
            End = 1 << 4
        };

    public:
        MeshComponent();
        ~MeshComponent();

        int enableAttribute(int attr);
        int disableAttribute(int attr);

        void* createMeshBuffer(int sizeOfByte);
        void* createMeshIndexBuffer(const std::string& name, int sizeOfByte);
        
        /*
         * attr: when attr is End, can take as, return value is unit size
         */
        int getOffset(Attribute attr);

        virtual void apply(GLProgram* program);
        
        GLuint getBufferLocation();
        GLuint getIndexBufferLocation(const std::string& name);
        int getIndexBufferSize(const std::string& name);


//        void createVertexesIf(int length);
//        void addVertex(const gl::Vertex& vertex);
//        gl::Vertex* getVertex();
        
        void addVertexes(GLData* v);
        void addVertexIndex(GLData* idx);
        void addMaterial(Material* mat);
        
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
        std::vector<Material*> materialVec;

        GLuint glBuffer;
        bool glBufferInvalid;
        
        std::string vshader_path;
        std::string fshader_path;
        GLProgram* program;
        
        int cameraObserveTag;
        int cameraViewObserveTag;
        bool cameraMatrixDirty;
        bool moduleMatrixDirty;

        MeshBuffer* meshBuffer;
        int attribute;

        std::map<std::string, MeshBuffer*> meshIndexMap;
    };
}

#endif /* MeshComponent_hpp */
