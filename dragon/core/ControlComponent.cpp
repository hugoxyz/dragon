//
//  ControlComponent.cpp
//  dragon
//
//  Created by hugo on 16/5/18.
//
//

#include "ControlComponent.hpp"
#include "Manager.hpp"
#include "EventComponent.hpp"
#include "CameraComponent.hpp"
#include "TransformComponent.hpp"
#include "InputKeyEvent.hpp"

namespace dragon {
    
    ControlComponent::ControlComponent() {
        registerKeyEvent();
    }
    
    ControlComponent::~ControlComponent() {
        unregisterKeyEvent();
    }
    
    void ControlComponent::registerKeyEvent() {
        EventComponent* comp = Manager::getInstance()->getComponent<EventComponent>();
        comp->addObserver(static_cast<int>(EventComponent::Event::EVENT_INPUT),
                          std::bind(&ControlComponent::onKeyEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
    
    void ControlComponent::unregisterKeyEvent() {
    }
    
    void ControlComponent::onKeyEvent(int e, Object* data, Object* userData) {
        EventComponent::Event event = static_cast<EventComponent::Event>(e);
        switch (event) {
            case EventComponent::Event::EVENT_INPUT: {
                InputKeyEvent *key = dynamic_cast<InputKeyEvent*>(data);
                if (nullptr == key) {
                    break;
                }
                switch (key->getAction()) {
                    case InputKeyEvent::Action::Release: {
                        Node* node = Manager::getInstance()->getChild("__camera_node");
                        CameraComponent* camera = node->getComponent<CameraComponent>();
                        TransformComponent * trans = node->getComponent<TransformComponent>();
                        if (nullptr == node || nullptr == camera || nullptr == trans) {
                            break;
                        }
                        glm::vec3 step = glm::vec3(0, 0, 0);
                        switch (key->getKey()) {
                            case InputKeyEvent::Key::KEY_LEFT_ARROW : {
                                step = glm::vec3(-1, 0, 0);
                                //move left
                                break;
                            }
                            case InputKeyEvent::Key::KEY_RIGHT_ARROW: {
                                step = glm::vec3(1, 0, 0);
                                //move right
                                break;
                            }
                            case InputKeyEvent::Key::KEY_UP_ARROW: {
                                step = glm::vec3(0, 0, 1);
                                //move forword
                                break;
                            }
                            case InputKeyEvent::Key::KEY_DOWN_ARROW: {
                                step = glm::vec3(0, 0, -1);
                                //move back
                                break;
                            }
                            case InputKeyEvent::Key::KEY_SPACE: {
                                //trans->move(glm::vec3(0, 0, 0));
                                //jump
                                break;
                            }
                            case InputKeyEvent::Key::KEY_W:
                            case InputKeyEvent::Key::KEY_CAPITAL_W: {
                                step = glm::vec3(0, 1, 0);
                                //move forword
                                break;
                            }
                            case InputKeyEvent::Key::KEY_S:
                            case InputKeyEvent::Key::KEY_CAPITAL_S: {
                                step = glm::vec3(0, -1, 0);
                                //move back
                                break;
                            }
                            default: {
                                break;
                            }
                        }
                        if (0 != glm::dot(step, step)) {
                            trans->move(step);
                            camera->moveFocus(step);
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