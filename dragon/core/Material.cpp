//
//  Material.cpp
//  dragon
//
//  Created by htl on 5/29/16.
//
//

#include <fstream>
#include "rapidjson/istreamwrapper.h"
#include "glm/ext.hpp"

#include "Utils.hpp"
#include "Material.hpp"

namespace dragon {

    Material::Material()
    : shininess(0) {
        ambient = glm::vec4(0.2, 0.2, 0.2, 1);
    }
    
    Material::~Material() {
    }
    
    Material* Material::parserFromFile(const std::string& file) {
        std::ifstream ifs(file);
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document d;
        d.ParseStream(isw);
        
        Material* mat = new Material();
        mat->parser(d);

        return mat;
    }
    
    Material* Material::parserFromJosn(const rapidjson::Value &json) {
        Material* mat = new Material();
        mat->parser(json);
        return mat;
    }

    void Material::apply(GLProgram* program) {
        //program->setUnifrom("uv4Emissive", glm::value_ptr(emissive), 4);
        program->setUnifrom("uv4Ambient", glm::value_ptr(ambient), 4);
        program->setUnifrom("uv4Diffuse", glm::value_ptr(diffuse), 4);
        program->setUnifrom("uv4Specular", glm::value_ptr(specular), 4);
        program->setUnifrom("ufShininess", &shininess, 1);
    }
    
    void Material::parser(const rapidjson::Value& json) {
        for (auto it = json.MemberBegin(); it != json.MemberEnd(); it++) {
            if (0 == Utils::compare(it->name.GetString(), "emissive")) {
                if (it->value.Size() < 3) {
                    continue;
                }
                emissive.x = it->value[0].GetFloat();
                emissive.y = it->value[1].GetFloat();
                emissive.z = it->value[2].GetFloat();
                if (4 == it->value.Size()) {
                    emissive.w = it->value[3].GetFloat();
                } else {
                    emissive.w = 1;
                }
            } else if (0 == Utils::compare(it->name.GetString(), "ambient")) {
                if (it->value.Size() < 3) {
                    continue;
                }
                ambient.x = it->value[0].GetFloat();
                ambient.y = it->value[1].GetFloat();
                ambient.z = it->value[2].GetFloat();
                if (4 == it->value.Size()) {
                    ambient.w = it->value[3].GetFloat();
                } else {
                    ambient.w = 1;
                }
            } else if (0 == Utils::compare(it->name.GetString(), "diffuse")) {
                if (it->value.Size() < 3) {
                    continue;
                }
                diffuse.x = it->value[0].GetFloat();
                diffuse.y = it->value[1].GetFloat();
                diffuse.z = it->value[2].GetFloat();
                if (4 == it->value.Size()) {
                    diffuse.w = it->value[3].GetFloat();
                } else {
                    diffuse.w = 1;
                }
            } else if (0 == Utils::compare(it->name.GetString(), "specular")) {
                if (it->value.Size() < 3) {
                    continue;
                }
                specular.x = it->value[0].GetFloat();
                specular.y = it->value[1].GetFloat();
                specular.z = it->value[2].GetFloat();
                if (4 == it->value.Size()) {
                    specular.w = it->value[3].GetFloat();
                } else {
                    specular.w = 1;
                }
            } else if (0 == Utils::compare(it->name.GetString(), "shininess")) {
                shininess = it->value.GetFloat();
            }
        }
    }
}