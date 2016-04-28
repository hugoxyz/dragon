//
//  GLProgram.hpp
//  dragon
//
//  Created by hugo on 16/4/22.
//
//

#ifndef GLProgram_hpp
#define GLProgram_hpp

#include <map>
#include <OpenGL/gl.h>
#include "../core/Object.hpp"

namespace dragon {
    class GLProgram : public Object {
    public:
        
        enum class GLVariableType {
            GL_ATTRIBUTE,
            GL_UNIFORM,
        } ;
        
        struct GLVariable {
            //location of attribute
            GLint location;
            GLint size;
            GLenum type;
            std::string name;
            GLVariableType variableType;
        };

    public:
        GLProgram();
        ~GLProgram();
        
        bool compile(const char* vs, const char* fs);
        bool setUnifrom(const std::string& name, GLfloat* value, int size);
        bool getAttributeLocation(const std::string& name, GLuint* loc);
        void use();

    protected:
        bool compileShader(const char* source, GLenum type, GLuint* pshader);
        bool createProgram(GLuint* pProgram, GLuint vs, GLuint fs);
        void updateVertexLocation();
        void updateUniformLocation();
        
    protected:
        GLuint program;
        
        std::map<std::string, GLVariable> glVariables;
    };
}

#endif /* GLProgram_hpp */
