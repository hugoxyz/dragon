//
//  RendererData.cpp
//  dragon
//
//  Created by hugo on 16/4/17.
//
//

#include <stdlib.h>

#include "EventRenderer.hpp"


namespace dragon {
    EventRenderer::EventRenderer()
    : vertexes(nullptr)
    , vertexCapacity(0)
    , vertexLength(0)
    , colors(nullptr)
    , colorsCapacity(0)
    , colorsLength(0)
    , normals(nullptr)
    , normalCapacity(0)
    , normalLength(0)
    , uvs(nullptr)
    , uvCapacity(0)
    , uvLength(0)
    , materialAssociate(nullptr)
    , materialLength(0) {
        stepLength = 6;
    }
    
    EventRenderer::~EventRenderer() {
        if (nullptr != vertexes) {
            free(vertexes);
            vertexes = nullptr;
            vertexLength = 0;
        }
        if (nullptr != colors) {
            free(colors);
            colors = nullptr;
            colorsLength = 0;
        }
        if (nullptr != normals) {
            free(normals);
            normals = nullptr;
            normalLength = 0;
        }
        if (nullptr != uvs) {
            free(uvs);
            uvs = nullptr;
            uvLength = 0;
        }
        if (nullptr != materialAssociate) {
            free(materialAssociate);
            materialAssociate = nullptr;
            materialLength = 0;
        }
    }
    
    void EventRenderer::reallocVertex() {
        int capacity = 0;
        if (0 == vertexCapacity) {
            capacity = stepLength;
        } else {
            capacity = vertexCapacity * 2;
        }
        Vertex* p = (Vertex*)realloc(vertexes, capacity * sizeof(Vertex));
        if (nullptr == p) {
            return;
        }
        vertexes = p;
        vertexCapacity = capacity;
    }
    
    void EventRenderer::reallocColor() {
        int capacity = 0;
        if (0 == colorsCapacity) {
            capacity = stepLength;
        } else {
            capacity = colorsCapacity * 2;
        }
        Color* p = (Color*)realloc(colors, capacity * sizeof(Color));
        if (nullptr == p) {
            return;
        }
        colors = p;
        colorsCapacity = capacity;
    }
    
    void EventRenderer::reallocNormal() {
        int capacity = 0;
        if (0 == normalCapacity) {
            capacity = stepLength;
        } else {
            capacity = normalCapacity * 2;
        }
        Normal* p = (Normal*)realloc(normals, capacity * sizeof(Normal));
        if (nullptr == p) {
            return;
        }
        normals = p;
        normalCapacity = capacity;
    }
    
    void EventRenderer::reallocUV() {
        int capacity = 0;
        if (0 == uvCapacity) {
            capacity = stepLength;
        } else {
            capacity = uvCapacity * 2;
        }
        UV* p = (UV*)realloc(uvs, capacity * sizeof(UV));
        if (nullptr == p) {
            return;
        }
        uvs = p;
        uvCapacity = capacity;
    }
    
    void EventRenderer::appendVertex(const Vertex &v) {
        if (vertexLength == vertexCapacity) {
            reallocVertex();
        }
        if (vertexLength < vertexCapacity) {
            memcpy(vertexes + vertexLength, &v, sizeof(Vertex));
            vertexLength++;
        }
    }
    
    void EventRenderer::appendColor(const Color &c) {
        if (colorsLength == colorsCapacity) {
            reallocColor();
        }
        if (colorsLength < colorsCapacity) {
            memcpy(colors + colorsLength, &c, sizeof(Color));
            colorsLength++;
        }
    }
    
    void EventRenderer::appendNormal(const Normal &n) {
        if (normalLength == normalCapacity) {
            reallocNormal();
        }
        if (normalLength < normalCapacity) {
            memcpy(normals + normalLength, &n, sizeof(Normal));
            normalLength++;
        }
    }
    
    void EventRenderer::appendUV(const UV &uv) {
        if (uvLength == uvCapacity) {
            reallocUV();
        }
        if (uvLength < uvCapacity) {
            memcpy(uvs + uvLength, &uv, sizeof(UV));
            uvLength++;
        }
    }

    int* EventRenderer::createMaterialMeshAssociate(int n) {
        if (materialAssociate) {
            free(materialAssociate);
            materialAssociate = nullptr;
            materialLength = 0;
        }
        materialAssociate = (int*)malloc(n * sizeof(int));
        if (nullptr == materialAssociate) {
            return nullptr;
        }
        materialLength = n;
        return materialAssociate;
    }
    
    bool EventRenderer::getVertex(Vertex** ppVertex, int *pSize) {
        *ppVertex = vertexes;
        *pSize = vertexLength;
        
        return true;
    }
}