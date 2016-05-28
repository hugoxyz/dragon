//
//  G3DJImporter.hpp
//  dragon
//
//  Created by hugo on 16/5/24.
//
//

#ifndef G3DJImporter_hpp
#define G3DJImporter_hpp

#include "Importer.hpp"

namespace dragon {
    class G3DJImporter : public Importer {
    public:
        static Node* import(const std::string& file);
    };
}

#endif /* G3DJImporter_hpp */
