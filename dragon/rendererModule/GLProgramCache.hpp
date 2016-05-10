//
//  GLProgramCache.hpp
//  dragon
//
//  Created by hugo on 16/5/10.
//
//

#ifndef GLProgramCache_hpp
#define GLProgramCache_hpp

#include "../core/Object.hpp"
#include "GLProgram.hpp"

namespace dragon {
    class GLProgramCache : public Object {
    public:
        
        static GLProgramCache* getInstance();
        GLProgramCache();
        ~GLProgramCache();

        void addProgram(GLProgram* program, const std::string& name);
        GLProgram* getProgram(const std::string& name);
        
        void addProgram(GLProgram* program, const std::string& vsh, const std::string& fsh);
        GLProgram* getProgram(const std::string& vsh, const std::string& fsh);
        
    protected:
        std::string genName(const std::string& vsh, const std::string& fsh);
        GLProgram* createGLProgram(const std::string& vsh, const std::string& fsh);
        
        std::map<std::string, GLProgram*> cacheProgramMap;

    private:
        static GLProgramCache* instance;
    };
}

#endif /* GLProgramCache_hpp */
