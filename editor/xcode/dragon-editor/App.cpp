//
//  App.cpp
//  dragon-editor
//
//  Created by hugo on 16/4/13.
//
//

#include "App.hpp"
#include "dragon.h"

int run_app() {
    dragon::Manager::getInstance()->run();

    return 0;
}
