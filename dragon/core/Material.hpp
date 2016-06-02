//
//  Material.hpp
//  dragon
//
//  Created by htl on 5/29/16.
//
//

#ifndef Material_hpp
#define Material_hpp

#include "rapidjson/document.h"

#include "Object.hpp"
#include "glm/glm.hpp"
#include "../rendererModule/GLProgram.hpp"

namespace dragon {
    class Material : public Object {
    public:
        Material();
        ~Material();
        
        static Material* parserFromFile(const std::string& file);
        static Material* parserFromJosn(const rapidjson::Value& json);

        void apply(GLProgram* program);

    protected:
        void parser(const rapidjson::Value& json);
        
    protected:
        glm::vec4 emissive; //发射光
        glm::vec4 ambient; //环境光
        glm::vec4 diffuse; //漫反射
        glm::vec4 specular; //镜面反射
        float shininess; //光泽度
    };
}

#endif /* Material_hpp */
