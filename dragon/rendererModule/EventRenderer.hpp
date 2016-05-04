//
//  RendererData.hpp
//  dragon
//
//  Created by hugo on 16/4/17.
//
//

#ifndef RendererCommand_hpp
#define RendererCommand_hpp

#include <functional>

#include "../core/Object.hpp"

namespace dragon {
    class EventRenderer : public Object {
    public:
        typedef std::function<bool(Object*)> RendererFunction;

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
        EventRenderer();
        ~EventRenderer();

        void appendVertex(const Vertex& v);
        void appendColor(const Color& c);
        void appendNormal(const Normal& n);
        void appendUV(const UV& uv);

        int* createMaterialMeshAssociate(int n);
        
        bool getVertex(Vertex** ppVertex, int *pSize);
        
        void setPreRenderer(const RendererFunction& func);
        void setRenderer(const RendererFunction& func);
        void setAfterRenderer(const RendererFunction& func);
        
        const RendererFunction& getPreRenderer();
        const RendererFunction& getRenderer();
        const RendererFunction& getAfterRenderer();

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
        
        RendererFunction preRendererFun;
        RendererFunction rendererFun;
        RendererFunction afterRendererFun;
    };
}

#endif /* RendererData_hpp */
