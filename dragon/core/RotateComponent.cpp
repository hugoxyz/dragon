//
//  RotateComponent.cpp
//  dragon
//
//  Created by hugo on 16/6/2.
//
//

#include "RotateComponent.hpp"
#include "Manager.hpp"
#include "EventComponent.hpp"
#include "CameraComponent.hpp"
#include "TransformComponent.hpp"
#include "InputKeyEvent.hpp"

namespace dragon {
    
    RotateComponent::RotateComponent()
    : eventKeyTag(0) {
        registerKeyEvent();
    }
    
    RotateComponent::~RotateComponent() {
        unregisterKeyEvent();
    }
    
    void RotateComponent::registerKeyEvent() {
        EventComponent* comp = Manager::getInstance()->getComponent<EventComponent>();
        eventKeyTag = comp->addObserver(static_cast<int>(EventComponent::Event::EVENT_INPUT_KEY),
                                        std::bind(&RotateComponent::onKeyEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
    
    void RotateComponent::unregisterKeyEvent() {
        EventComponent* comp = Manager::getInstance()->getComponent<EventComponent>();
        comp->removeObserver(static_cast<int>(EventComponent::Event::EVENT_INPUT_KEY), eventKeyTag);
        eventKeyTag = 0;
    }
    
    void RotateComponent::onKeyEvent(int e, Object* data, Object* userData) {
        EventComponent::Event event = static_cast<EventComponent::Event>(e);
        switch (event) {
            case EventComponent::Event::EVENT_INPUT_KEY: {
                InputKeyEvent *key = dynamic_cast<InputKeyEvent*>(data);
                if (nullptr == key) {
                    break;
                }
                switch (key->getAction()) {
                    case InputKeyEvent::Action::Repeat:
                    case InputKeyEvent::Action::Release: {
                        TransformComponent * trans = host->getComponent<TransformComponent>();
                        if (nullptr == trans) {
                            break;
                        }
                        glm::vec3 step = glm::vec3(0, 0, 0);
                        switch (key->getKey()) {
                            case InputKeyEvent::Key::KEY_X : {
                                trans->rotate(glm::vec3(1, 0, 0));
                                break;
                            }
                            case InputKeyEvent::Key::KEY_C: {
                                trans->rotate(glm::vec3(0, 1, 0));
                                break;
                            }
                            case InputKeyEvent::Key::KEY_Z: {
                                trans->rotate(glm::vec3(0, 0, 1));
                                break;
                            }
                            default: {
                                break;
                            }
                        }
                        break;
                    }
                    default: {
                        break;
                    }
                }
                
                break;
            }
                
            default: {
                break;
            }
        }
        
    }
    
}
