//
//  Widget.cpp
//  dragon
//
//  Created by hugo on 16/6/11.
//
//

#include "Widget.hpp"
#include "../core/EventComponent.hpp"
#include "../core/Manager.hpp"
#include "../core/InputKeyEvent.hpp"
#include "../core/InputTouchEvent.hpp"

namespace dragon {
    namespace ui {
        
        Widget::Widget()
        : visible(false)
        , tagTouchEvent(0)
        , tagKeyEvent(0)
        , swallowed(false) {
            registerTouchEvent();
            registerKeyEvent();
        }
        
        Widget::~Widget() {
            unregisterTouchEvent();
            unregisterKeyEvent();
        }
        
        bool Widget::isVisible() {
            return visible;
        }
        
        void Widget::setVisible(bool v) {
            visible = v;
        }

        bool Widget::dispatchEvent(InputBaseEvent *e) {
            bool consume = false;
            InputTouchEvent* touch = nullptr;
            InputKeyEvent* key = nullptr;
            
            touch = dynamic_cast<InputTouchEvent*>(e);
            key = dynamic_cast<InputKeyEvent*>(e);
            
            if (swallowed) {
                if (touch) {
                    consume = dispatchTouchEvent(touch);
                } else if (key) {
                    consume = dispatchKeyEvent(key);
                }
                
                return consume;
            } else {
                for (auto child : children) {
                    Widget *w = dynamic_cast<Widget*>(child);
                    if (w) {
                        consume = w->dispatchEvent(e);
                    }
                    if (consume) {
                        break;
                    }
                }
                if (!consume) {
                    if (touch) {
                        consume = dispatchTouchEvent(touch);
                    } else if (key) {
                        consume = dispatchKeyEvent(key);
                    }
                }
                
                return consume;
            }
        }
        
        bool Widget::dispatchTouchEvent(InputTouchEvent *e) {
            bool consume = false;
            switch (e->getAction()) {
                case InputTouchEvent::Action::Press : {
                    consume = onTouchBegin(e);
                    break;
                }
                case InputTouchEvent::Action::Move : {
                    consume = onTouchMove(e);
                    break;
                }
                case InputTouchEvent::Action::Release : {
                    consume = onTouchEnd(e);
                    break;
                }
                default: {
                    break;
                }
            }
            return consume;
        }
        
        bool Widget::dispatchKeyEvent(InputKeyEvent *e) {
            bool consume = false;
            switch (e->getAction()) {
                case InputKeyEvent::Action::Press : {
                    consume = onKeyBegin(e);
                    break;
                }
                case InputKeyEvent::Action::Repeat : {
                    consume = onKeyRepeat(e);
                    break;
                }
                case InputKeyEvent::Action::Release : {
                    consume = onKeyEnd(e);
                    break;
                }
                default: {
                    break;
                }
            }
            return consume;
        }
        
        bool Widget::onTouchBegin(InputTouchEvent *e) {
            return false;
        }
        
        bool Widget::onTouchMove(InputTouchEvent *e) {
            return false;
        }
        
        bool Widget::onTouchEnd(InputTouchEvent *e) {
            return false;
        }

        bool Widget::onKeyBegin(InputKeyEvent *e) {
            return false;
        }
        
        bool Widget::onKeyRepeat(InputKeyEvent *e) {
            return false;
        }
        
        bool Widget::onKeyEnd(InputKeyEvent *e) {
            return false;
        }
        
        void Widget::registerTouchEvent() {
            EventComponent* comp = Manager::getInstance()->getComponent<EventComponent>();
            tagTouchEvent = comp->addObserver(static_cast<int>(EventComponent::Event::EVENT_INPUT_TOUCH),
                                            std::bind(&Widget::onTouchEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        }
        
        void Widget::unregisterTouchEvent() {
            EventComponent* comp = Manager::getInstance()->getComponent<EventComponent>();
            comp->removeObserver(static_cast<int>(EventComponent::Event::EVENT_INPUT_TOUCH), tagTouchEvent);
            tagTouchEvent = 0;
        }
        
        void Widget::onTouchEvent(int e, Object* data, Object* userData) {
            if (!visible) {
                return;
            }
            EventComponent::Event event = static_cast<EventComponent::Event>(e);
            switch (event) {
                case EventComponent::Event::EVENT_INPUT_TOUCH: {
                    InputTouchEvent *touch = dynamic_cast<InputTouchEvent*>(data);
                    if (nullptr == touch) {
                        break;
                    }
                    dispatchEvent(touch);

                    break;
                }
                    
                default: {
                    break;
                }
            }
        }
        
        void Widget::registerKeyEvent() {
            EventComponent* comp = Manager::getInstance()->getComponent<EventComponent>();
            tagKeyEvent = comp->addObserver(static_cast<int>(EventComponent::Event::EVENT_INPUT_KEY),
                                            std::bind(&Widget::onKeyEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        }
        
        void Widget::unregisterKeyEvent() {
            EventComponent* comp = Manager::getInstance()->getComponent<EventComponent>();
            comp->removeObserver(static_cast<int>(EventComponent::Event::EVENT_INPUT_KEY), tagKeyEvent);
            tagKeyEvent = 0;
        }
        
        void Widget::onKeyEvent(int e, Object* data, Object* userData) {
            if (!visible) {
                return;
            }
            EventComponent::Event event = static_cast<EventComponent::Event>(e);
            switch (event) {
                case EventComponent::Event::EVENT_INPUT_KEY: {
                    InputKeyEvent *key = dynamic_cast<InputKeyEvent*>(data);
                    if (nullptr == key) {
                        break;
                    }
                    dispatchEvent(key);
                    
                    break;
                }
                    
                default: {
                    break;
                }
            }
        }
    }
}