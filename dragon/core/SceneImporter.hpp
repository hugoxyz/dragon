//
//  SceneImporter.hpp
//  dragon
//
//  Created by hugo on 16/6/11.
//
//

#ifndef SceneImporter_hpp
#define SceneImporter_hpp

#include "Importer.hpp"
#include "Component.hpp"

namespace dragon {
    class SceneImporter : public Importer {
    public:
        static Node* import(const std::string& file);
        
        SceneImporter();
        Node* parser(const std::string& file);
        
    protected:
        Node* parserNode(const rapidjson::Value& json);
        Component* parserComponent(const std::string& name, const rapidjson::Value& json);

    protected:
        std::string file;
    };
}

#endif /* SceneImporter_hpp */
