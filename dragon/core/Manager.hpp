//
//  Manager.hpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#ifndef Manager_hpp
#define Manager_hpp

#include <vector>
#include "Module.hpp"
#include "Message.hpp"

namespace dragon {
    class EventComponent;
    class Manager : public Module {
    public:
        static Manager* getInstance();
        Manager();
        virtual ~Manager();
        
        void addChild(Node* n);
        void addComponent(Component* comp, const std::string& name = "");
        
        virtual void onInit();

        void run();
        void exit();

        void sendMsg(Message* msg);
        void sendMsg(const std::string& modName, Message* msg);
        void postEvent(int event, Object* data);
        
    protected:
        void loadDefaultModule();
        void loadDefaultComponent();
        void dispatchMsg();
        
    protected:
        std::vector<Message*> msgs;
        bool running;
        int fps;
        Module* uiModule;
        EventComponent* eventComponent;

    private:
        static Manager* instance;
    };
}


#endif /* Manager_hpp */
