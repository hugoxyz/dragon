//
//  RendererData.hpp
//  dragon
//
//  Created by hugo on 16/4/17.
//
//

#ifndef RendererCommand_hpp
#define RendererCommand_hpp

#include "../core/Message.hpp"

namespace dragon {
    class RendererMessage : public Message {
    public:
        typedef struct {
            float x;
            float y;
            float z;
        } Vertex;

        typedef struct {
            float r;
            float g;
            float b;
            float a;
        } Color;

        typedef struct {
            float x;
            float y;
            float z;
        } Normal;

        typedef struct {
            float u;
            float v;
        } UV;

    public:
        RendererMessage();
        ~RendererMessage();

        void appendVertex(const Vertex& v);
        void appendColor(const Color& c);
        void appendNormal(const Normal& n);
        void appendUV(const UV& uv);

        int* createMaterialMeshAssociate(int n);

    protected:
        void reallocVertex();
        void reallocColor();
        void reallocNormal();
        void reallocUV();

    private:
        Vertex* vertexes;
        int vertexCapacity;
        int vertexLength;

        Color* colors;
        int colorsCapacity;
        int colorsLength;

        Normal* normals;
        int normalCapacity;
        int normalLength;

        UV* uvs;
        int uvCapacity;
        int uvLength;

        int stepLength;
        
        int* materialAssociate;
        int materialLength;
    };
}

#endif /* RendererData_hpp */
