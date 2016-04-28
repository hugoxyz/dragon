//
//  RendererData.cpp
//  dragon
//
//  Created by hugo on 16/4/17.
//
//

#include <stdlib.h>

#include "RendererMessage.hpp"


namespace dragon {
    RendererMessage::RendererMessage()
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
    
    RendererMessage::~RendererMessage() {
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
    
    void RendererMessage::reallocVertex() {
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
    
    void RendererMessage::reallocColor() {
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
    
    void RendererMessage::reallocNormal() {
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
    
    void RendererMessage::reallocUV() {
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
    
    void RendererMessage::appendVertex(const Vertex &v) {
        if (vertexLength == vertexCapacity) {
            reallocVertex();
        }
        if (vertexLength < vertexCapacity) {
            memcpy(vertexes + vertexLength, &v, sizeof(Vertex));
            vertexLength++;
        }
    }
    
    void RendererMessage::appendColor(const Color &c) {
        if (colorsLength == colorsCapacity) {
            reallocColor();
        }
        if (colorsLength < colorsCapacity) {
            memcpy(colors + colorsLength, &c, sizeof(Color));
            colorsLength++;
        }
    }
    
    void RendererMessage::appendNormal(const Normal &n) {
        if (normalLength == normalCapacity) {
            reallocNormal();
        }
        if (normalLength < normalCapacity) {
            memcpy(normals + normalLength, &n, sizeof(Normal));
            normalLength++;
        }
    }
    
    void RendererMessage::appendUV(const UV &uv) {
        if (uvLength == uvCapacity) {
            reallocUV();
        }
        if (uvLength < uvCapacity) {
            memcpy(uvs + uvLength, &uv, sizeof(UV));
            uvLength++;
        }
    }

    int* RendererMessage::createMaterialMeshAssociate(int n) {
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
}