//
//  MaterialComponent.cpp
//  dragon
//
//  Created by hugo on 16/6/16.
//
//

#include "Utils.hpp"
#include "FileUtils.hpp"
#include "MaterialComponent.hpp"
#include "GLProgramCache.hpp"
#include "glm/ext.hpp"

namespace dragon {
    MaterialComponent::MaterialComponent()
    : vsh("")
    , fsh("") {
    }
    
    MaterialComponent::~MaterialComponent() {
    }
    
    MaterialComponent* MaterialComponent::parserFromFile(const std::string& f) {
        std::string file = FileUtils::getInstance()->fullPath(f);
        unsigned char* data = nullptr;
        int dataLen = 0;
        FileUtils::getInstance()->read(file, &data, &dataLen);
        rapidjson::Document d;
        d.Parse((const char*)data, dataLen);
        
        MaterialComponent* mat = new MaterialComponent();
        mat->parser(d);
        
        return mat;
    }
    
    MaterialComponent* MaterialComponent::parserFromJosn(const rapidjson::Value& json) {
        MaterialComponent* mat = new MaterialComponent();
        mat->parser(json);
        return mat;
    }

    void MaterialComponent::apply(GLProgram* program) {
        program->setUnifrom("uv4Ambient", glm::value_ptr(ambient), 4);
        program->setUnifrom("uv4Diffuse", glm::value_ptr(diffuse), 4);
        program->setUnifrom("uv4Specular", glm::value_ptr(specular), 4);
        program->setUnifrom("ufShininess", &shininess, 1);
    }
    
    GLProgram* MaterialComponent::getGLProgram() {
        return GLProgramCache::getInstance()->getProgram(vsh, fsh);
    }

    void MaterialComponent::parser(const rapidjson::Value& json) {
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
            } else if (0 == Utils::compare(it->name.GetString(), "vsh")) {
                vsh = it->value.GetString();
                if (std::string::npos == vsh.find('.')) {
                    vsh += ".vsh";
                }
            } else if (0 == Utils::compare(it->name.GetString(), "fsh")) {
                fsh = it->value.GetString();
                if (std::string::npos == fsh.find('.')) {
                    fsh += ".fsh";
                }
            } else if (0 == Utils::compare(it->name.GetString(), "name")) {
                setName(it->value.GetString());
            }
        }
    }
}