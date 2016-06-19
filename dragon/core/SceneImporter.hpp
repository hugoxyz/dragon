//
//  SceneImporter.hpp
//  dragon
//
//  Created by hugo on 16/6/11.
//
//

#ifndef SceneImporter_hpp
#define SceneImporter_hpp

#include <vector>
#include "glm.hpp"

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
        std::vector<Component*> parserComponent(const std::string& name, const rapidjson::Value& json);
        glm::vec3 transToVec3(const rapidjson::Value& json);
        Component* parserTransform(const rapidjson::Value& json);
        Component* parserMesh(const rapidjson::Value& json);
        Component* parserMaterial(const rapidjson::Value& json);
        Component* parserRenderer(const rapidjson::Value& json);

        Component* parserModule(const rapidjson::Value& json);

    protected:
        std::string file;
    };
}

#endif /* SceneImporter_hpp */
