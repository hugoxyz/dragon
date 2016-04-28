//
//  FileModule.hpp
//  dragon
//
//  Created by hugo on 16/4/20.
//
//

#ifndef FileModule_hpp
#define FileModule_hpp

#include "../core/Module.hpp"

namespace dragon {
    class FileModule : public Module {
    public:
        virtual void onMessage(Message* msg);
        
    protected:
        int readFile(const std::string& file, void** ppData);
    };
}

#endif /* FileModule_hpp */
