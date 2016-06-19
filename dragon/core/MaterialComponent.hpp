//
//  MaterialComponent.hpp
//  dragon
//
//  Created by hugo on 16/6/16.
//
//

#ifndef MaterialComponent_hpp
#define MaterialComponent_hpp

#include "glm.hpp"
#include "rapidjson/document.h"
#include "Component.hpp"

namespace dragon {
    class MaterialComponent : public Component {
    public:
        MaterialComponent();
        ~MaterialComponent();
        
        static MaterialComponent* parserFromFile(const std::string& file);
        static MaterialComponent* parserFromJosn(const rapidjson::Value& json);

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

#endif /* MaterialComponent_hpp */
