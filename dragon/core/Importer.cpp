//
//  Importer.cpp
//  dragon
//
//  Created by hugo on 16/5/24.
//
//

#include "Importer.hpp"
#include "G3DJImporter.hpp"
#include "Utils.hpp"
#include "Logger.hpp"

namespace dragon {
    Node* Importer::import(const std::string &file) {
        Node* n = nullptr;
        std::string suffix = Utils::getSuffixName(file);
        
        if (0 == suffix.compare("g3dj")) {
            n = G3DJImporter::import(file);
        } else {
            LOGD("Importer", "not support file:%s", file.c_str());
        }
        return n;
    }
}