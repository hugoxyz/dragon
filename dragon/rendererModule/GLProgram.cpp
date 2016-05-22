//
//  GLProgram.cpp
//  dragon
//
//  Created by hugo on 16/4/22.
//
//

#include <stdlib.h>

#include "GLProgram.hpp"
#include "../core/Logger.hpp"


namespace dragon {
    GLProgram::GLProgram()
    : program(0) {
        glVariables.clear();
    }
    
    GLProgram::~GLProgram() {
        glDeleteProgram(program);
    }
    
    bool GLProgram::compile(const char *vs, const char *fs) {
        bool ret = false;
        GLuint vsShader = 0;
        ret = compileShader(vs, GL_VERTEX_SHADER, &vsShader);
        if (!ret) {
            return ret;
        }

        GLuint fsShader = 0;
        ret = compileShader(fs, GL_FRAGMENT_SHADER, &fsShader);
        if (!ret) {
            glDeleteShader(vsShader);
            return ret;
        }

        if (!createProgram(&program, vsShader, fsShader)) {
            glDeleteShader(vsShader);
            glDeleteShader(fsShader);
            return false;
        }

        glVariables.clear();
        updateVertexLocation();
        updateUniformLocation();
        glDeleteShader(vsShader);
        glDeleteShader(fsShader);

        return true;
    }

    bool GLProgram::compileShader(const char* source, GLenum type, GLuint* pshader) {
        GLuint shader = glCreateShader(type);
        const char* shaderSource[] = {
            source,
        };
        
        glShaderSource(shader, sizeof(shaderSource)/sizeof(*shaderSource), shaderSource, nullptr);
        glCompileShader(shader);
        
        GLint ret = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ret);
        if (GL_FALSE == ret) {
            ret = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &ret);
            GLchar* log = nullptr;
            log = (GLchar*)malloc((ret + 1) * sizeof(GLchar));
            if (nullptr != log) {
                memset(log, 0, (ret + 1) * sizeof(GLchar));
                GLsizei logLength = 0;
                glGetShaderInfoLog(shader, ret * sizeof(GLchar), &logLength, log);
                LOGE("GL", "compile shader fail:%s", log);
                free(log);
            } else {
                LOGE("GL", "malloc shader log buffer fail");
            }
            return  false;
        }
        
        *pshader = shader;
        return true;
    }

    bool GLProgram::createProgram(GLuint* pProgram, GLuint vs, GLuint fs) {
        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        
        GLint ret = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &ret);
        if (GL_FALSE == ret) {
            ret = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &ret);
            GLchar *log = nullptr;
            log = (GLchar*)malloc((ret + 1) * sizeof(GLchar));
            if (nullptr != log) {
                memset(log, 0, (ret + 1) * sizeof(GLchar));
                GLsizei logLength = 0;
                glGetProgramInfoLog(program, ret * sizeof(GLchar), &logLength, log);
                LOGE("GL", "link program fail:%s", log);
                free(log);
            } else {
                LOGE("GL", "malloc program link log buffer fail");
            }
            return false;
        }

        *pProgram = program;
        return true;
    }
    
    void GLProgram::updateVertexLocation() {
        GLint count = 0;

        glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
        if (count <= 0) {
            return;
        }
        GLint length = 0;
        GLchar *buffer = nullptr;
        glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length);
        buffer = (GLchar*)malloc((length + 1) * sizeof(GLchar));
        if (nullptr == buffer) {
            LOGE("GL", "malloc attribue name fail:%d", length);
            return;
        }
        GLsizei attrLength = 0;
        GLVariable variable;

        variable.variableType = GLVariableType::GL_ATTRIBUTE;
        for (int i = 0; i < count; i++) {
            memset(buffer, 0, (length + 1) * sizeof(GLchar));
            glGetActiveAttrib(program, i, length, &attrLength, &variable.size, &variable.type, buffer);
            variable.location = glGetAttribLocation(program, buffer);
            variable.name = buffer;
            glVariables[variable.name] = variable;
        }
    }
    
    void GLProgram::updateUniformLocation() {
        GLint count = 0;
        
        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
        if (count <= 0) {
            return;
        }
        GLint length = 0;
        GLchar *buffer = nullptr;
        glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
        buffer = (GLchar*)malloc((length + 1) * sizeof(GLchar));
        if (nullptr == buffer) {
            LOGE("GL", "malloc unifrom name fail:%d", length);
            return;
        }
        GLsizei uniformLength = 0;
        GLVariable variable;
        variable.variableType = GLVariableType::GL_UNIFORM;
        for (int i = 0; i < count; i++) {
            memset(buffer, 0, (length + 1) * sizeof(GLchar));
            glGetActiveUniform(program, i, length, &uniformLength, &variable.size, &variable.type, buffer);
            variable.location = glGetUniformLocation(program, buffer);
            variable.name = buffer;
            glVariables[variable.name] = variable;
        }
    }
    
    bool GLProgram::setUnifrom(const std::string &name, GLfloat *value, int size) {
        const auto it = glVariables.find(name);
        if (it == glVariables.end()) {
            LOGD("GL", "can not find variable: %s", name.c_str());
            return false;
        }
        const auto& variable = it->second;
        if (GLVariableType::GL_UNIFORM != variable.variableType) {
            LOGD("GL", "it is not a unifrom:%s", name.c_str());
            return false;
        }

        switch (variable.type) {
            case GL_FLOAT: {
                if (size == variable.size) {
                    glUniform1fv(variable.location, variable.size, value);
                } else {
                    LOGE("GL", "uniform float size not the same");
                }
                break;
            }
            case GL_FLOAT_VEC2: {
                if (size == variable.size * 2) {
                    glUniform2fv(variable.location, variable.size, value);
                } else {
                    LOGE("GL", "uniform vec2 size not the same");
                }
                break;
            }
            case GL_FLOAT_VEC3: {
                if (size == variable.size * 3) {
                    glUniform3fv(variable.location, variable.size, value);
                } else {
                    LOGE("GL", "uniform vec3 size not the same");
                }
                break;
            }
            case GL_FLOAT_VEC4: {
                if (size == variable.size * 4) {
                    glUniform4fv(variable.location, variable.size, value);
                } else {
                    LOGE("GL", "uniform vec4 size not the same");
                }
                break;
            }
            case GL_FLOAT_MAT2: {
                if (size == variable.size * 4) {
                    glUniformMatrix2fv(variable.location, variable.size, GL_FALSE, value);
                } else {
                    LOGE("GL", "uniform mat2 size not the same");
                }
                break;
            }
            case GL_FLOAT_MAT3: {
                if (size == variable.size * 9) {
                    glUniformMatrix3fv(variable.location, variable.size, GL_FALSE, value);
                } else {
                    LOGE("GL", "uniform mat3 size not the same");
                }
                break;
            }
            case GL_FLOAT_MAT4: {
                if (size == variable.size * 16) {
                    glUniformMatrix4fv(variable.location, variable.size, GL_FALSE, value);
                } else {
                    LOGE("GL", "uniform mat4 size not the same");
                }
                break;
            }
            default: {
                LOGE("GL", "unkow unifrom type:%d", variable.type);
                break;
            }
        }
        
        return true;
    }
    
    bool GLProgram::getAttributeLocation(const std::string &name, GLuint* loc) {
        if (0 == glVariables.size()) {
            return false;
        }
        for (auto it : glVariables) {
            if (0 == name.compare(it.first)) {
                *loc = it.second.location;
                return true;
            }
        }

        return false;
    }

    bool GLProgram::enableAttribute(const std::string& attr) {
        GLuint loc;
        if (!getAttributeLocation(attr, & loc)) {
            return false;
        }
        glEnableVertexAttribArray(loc);
        return true;
    }
    
    bool GLProgram::attributePointer(const std::string& attr, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
        GLuint loc;
        if (!getAttributeLocation(attr, & loc)) {
            return false;
        }
        glVertexAttribPointer(loc, size, type, normalized, stride, pointer);
        return true;
    }

    void GLProgram::use() {
        glUseProgram(program);
    }
}
