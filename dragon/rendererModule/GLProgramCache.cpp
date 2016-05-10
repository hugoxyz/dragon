//
//  GLProgramCache.cpp
//  dragon
//
//  Created by hugo on 16/5/10.
//
//

#include <stdlib.h>
#include "GLProgramCache.hpp"
#include "../core/Utils.hpp"
#include "../core/FileUtils.hpp"

namespace dragon {
    
    GLProgramCache* GLProgramCache::instance = nullptr;
    GLProgramCache* GLProgramCache::getInstance() {
        if (nullptr == instance) {
            instance = new GLProgramCache();
        }
        
        return instance;
    }
    
    GLProgramCache::GLProgramCache() {
        cacheProgramMap.clear();
    }
    
    GLProgramCache::~GLProgramCache() {
    }
    
    void GLProgramCache::addProgram(GLProgram* program, const std::string& name) {
        program->retain();
        auto ret = cacheProgramMap.insert(std::make_pair(name, program));
        if (!ret.second) {
            ret.first->second->release();
        }
    }
    
    GLProgram* GLProgramCache::getProgram(const std::string& name) {
        auto ret = cacheProgramMap.find(name);
        if (ret == cacheProgramMap.end()) {
            return nullptr;
        }
        return ret->second;
    }
    
    void GLProgramCache::addProgram(GLProgram* program, const std::string& vsh, const std::string& fsh) {
        addProgram(program, genName(vsh, fsh));
    }
    
    GLProgram* GLProgramCache::getProgram(const std::string& vsh, const std::string& fsh) {
        std::string key = genName(vsh, fsh);
        GLProgram* program = getProgram(key);
        
        if (nullptr == program) {
            program = createGLProgram(vsh, fsh);
            if (nullptr != program) {
                addProgram(program, key);
            }
        }
        return program;
    }

    std::string GLProgramCache::genName(const std::string& vsh, const std::string& fsh) {
        std::string fName = Utils::split(fsh, '/').back();
        fName = Utils::split(fName, '\\').back();
        
        std::string vName = Utils::split(vsh, '/').back();
        vName = Utils::split(vName, '\\').back();
        
        return vName + fName;
    }
    
    GLProgram* GLProgramCache::createGLProgram(const std::string& vsh, const std::string& fsh) {
        char* vs = nullptr;
        char* fs = nullptr;
        int vsLen = 0;
        int fsLen = 0;
        bool ret = false;
        
        ret = FileUtils::getInstance()->read(vsh, (unsigned char**)&vs, &vsLen, true);
        ret = ret && FileUtils::getInstance()->read(fsh, (unsigned char**)&fs, &fsLen, true);
        if (!ret) {
            if (nullptr != vs) {
                free(vs);
            }
            if (nullptr != fs) {
                free(fs);
            }
            return nullptr;
        }
        
        GLProgram* program = new GLProgram();
        program->compile(vs, fs);
        if (nullptr != vs) {
            free(vs);
        }
        if (nullptr != fs) {
            free(fs);
        }
        
        return program;
    }
}
