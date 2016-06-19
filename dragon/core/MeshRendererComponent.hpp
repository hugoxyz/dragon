//
//  MeshRendererComponent.hpp
//  dragon
//
//  Created by hugo on 16/6/17.
//
//

#ifndef MeshRendererComponent_hpp
#define MeshRendererComponent_hpp

#include "RendererComponent.hpp"
#include "MeshComponent.hpp"

namespace dragon {
    class MeshRendererComponent : public RendererComponent {
    public:
        MeshRendererComponent();
        ~MeshRendererComponent();
        
        static MeshRendererComponent* parserFromFile(const std::string& file);
        static MeshRendererComponent* parserFromJosn(const rapidjson::Value& json);

        
        void addPart(std::string meshIdxName, std::string materialName);
        virtual void onUpdate() override;
        
    protected:
        void parser(const rapidjson::Value& json);
        
    protected:
        std::map<std::string, std::string> parts;
    };
}

#endif /* MeshRendererComponent_hpp */
