//
//  GLShader.hpp
//  dragon
//
//  Created by hugo on 16/4/20.
//
//

#ifndef GLShader_hpp
#define GLShader_hpp

#include "../core/Object.hpp"

namespace dragon {
    class GLShader : public Object {
    public:
        GLShader();
        
        void compile(const std::string& vsh, const std::string& fsh);
        
    private:
        const char* vsh_content;
        const char* fsh_content;
    };
}

#endif /* GLShader_hpp */
