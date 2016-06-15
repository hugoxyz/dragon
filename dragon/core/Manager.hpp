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
    class GLComponent;
    class Manager : public Module {
    public:
        static Manager* getInstance();
        Manager();
        virtual ~Manager();

        void createMainNode();
        void addChild(Node* n, bool bMain = true);

        void addComponent(Component* comp, const std::string& name = "");
        
        void step();
        void config();
        virtual void onDeinit();

        void run(const std::string& scene = "main.json");
        void exit();
        
        bool loadScene(const std::string& scene);

        void sendMsg(Message* msg);
        void sendMsg(const std::string& modName, Message* msg);
        void postEvent(int event, Object* data);
        
        void afterUpdate();
        
        void setAppName(const std::string& name);
        void setScreenSize(int w, int h);
        void setBgColor(int r, int g, int b);
        
    protected:
        void loadDefaultModule();
        void loadDefaultComponent();
        void dispatchMsg();
        
    protected:
        std::vector<Message*> msgs;
        int fps;
        Module* uiModule;
        EventComponent* eventComponent;
        GLComponent* glComponent;
        Node* mainNode;

        //config
        std::string appName;
        int screenWidth;
        int screenHeight;
        int bgColorR;
        int bgColorG;
        int bgColorB;
        int bgColorA;

    private:
        static Manager* instance;
        bool running;
    };
}


#endif /* Manager_hpp */
