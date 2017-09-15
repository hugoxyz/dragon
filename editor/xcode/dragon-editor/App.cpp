//
//  App.cpp
//  dragon-editor
//
//  Created by hugo on 16/4/13.
//
//

#include "App.hpp"
#include "dragon.h"

class b {
protected:
    static int id_counter;
};

class a {
};

template <typename Derived>
class BaseT : public a, b {
    
public:
    static int f() {
        static int id = id_counter++;
        
        return id;
    }

};


class BaseTA : public BaseT<BaseTA> {

};

class BaseTB : public BaseT<BaseTB> {
    
};

int b::id_counter = 0;
int run_app() {

    int i = 100;
    i = BaseTA::f();
    i = BaseTB::f();
    i = BaseTA::f();
    
    dragon::Manager::getInstance()->run();

    return 0;
}
