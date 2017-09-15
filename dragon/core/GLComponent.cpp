//
//  GLComponent.cpp
//  dragon
//
//  Created by hugo on 16/6/12.
//
//

#include <assert.h>
#include <vector>

#include "GLComponent.hpp"
#include "Logger.hpp"
#include "Importer.hpp"
#include "Manager.hpp"

#include "InputKeyEvent.hpp"
#include "InputTouchEvent.hpp"

#include "EventComponent.hpp"

namespace dragon {

    static GLComponent* glComponentInstance = nullptr;
    
    struct GLFWKeyCodeItem {
        int glfwKeyCode;
        InputKeyEvent::Key keyCode;
    };
    
    static std::map<int, InputKeyEvent::Key> g_keyCodeMap;
    
    static GLFWKeyCodeItem g_keyCodeStructArray[] = {
        /* The unknown key */
        { GLFW_KEY_UNKNOWN         , InputKeyEvent::Key::KEY_NONE          },
        
        /* Printable keys */
        { GLFW_KEY_SPACE           , InputKeyEvent::Key::KEY_SPACE         },
        { GLFW_KEY_APOSTROPHE      , InputKeyEvent::Key::KEY_APOSTROPHE    },
        { GLFW_KEY_COMMA           , InputKeyEvent::Key::KEY_COMMA         },
        { GLFW_KEY_MINUS           , InputKeyEvent::Key::KEY_MINUS         },
        { GLFW_KEY_PERIOD          , InputKeyEvent::Key::KEY_PERIOD        },
        { GLFW_KEY_SLASH           , InputKeyEvent::Key::KEY_SLASH         },
        { GLFW_KEY_0               , InputKeyEvent::Key::KEY_0             },
        { GLFW_KEY_1               , InputKeyEvent::Key::KEY_1             },
        { GLFW_KEY_2               , InputKeyEvent::Key::KEY_2             },
        { GLFW_KEY_3               , InputKeyEvent::Key::KEY_3             },
        { GLFW_KEY_4               , InputKeyEvent::Key::KEY_4             },
        { GLFW_KEY_5               , InputKeyEvent::Key::KEY_5             },
        { GLFW_KEY_6               , InputKeyEvent::Key::KEY_6             },
        { GLFW_KEY_7               , InputKeyEvent::Key::KEY_7             },
        { GLFW_KEY_8               , InputKeyEvent::Key::KEY_8             },
        { GLFW_KEY_9               , InputKeyEvent::Key::KEY_9             },
        { GLFW_KEY_SEMICOLON       , InputKeyEvent::Key::KEY_SEMICOLON     },
        { GLFW_KEY_EQUAL           , InputKeyEvent::Key::KEY_EQUAL         },
        { GLFW_KEY_A               , InputKeyEvent::Key::KEY_A             },
        { GLFW_KEY_B               , InputKeyEvent::Key::KEY_B             },
        { GLFW_KEY_C               , InputKeyEvent::Key::KEY_C             },
        { GLFW_KEY_D               , InputKeyEvent::Key::KEY_D             },
        { GLFW_KEY_E               , InputKeyEvent::Key::KEY_E             },
        { GLFW_KEY_F               , InputKeyEvent::Key::KEY_F             },
        { GLFW_KEY_G               , InputKeyEvent::Key::KEY_G             },
        { GLFW_KEY_H               , InputKeyEvent::Key::KEY_H             },
        { GLFW_KEY_I               , InputKeyEvent::Key::KEY_I             },
        { GLFW_KEY_J               , InputKeyEvent::Key::KEY_J             },
        { GLFW_KEY_K               , InputKeyEvent::Key::KEY_K             },
        { GLFW_KEY_L               , InputKeyEvent::Key::KEY_L             },
        { GLFW_KEY_M               , InputKeyEvent::Key::KEY_M             },
        { GLFW_KEY_N               , InputKeyEvent::Key::KEY_N             },
        { GLFW_KEY_O               , InputKeyEvent::Key::KEY_O             },
        { GLFW_KEY_P               , InputKeyEvent::Key::KEY_P             },
        { GLFW_KEY_Q               , InputKeyEvent::Key::KEY_Q             },
        { GLFW_KEY_R               , InputKeyEvent::Key::KEY_R             },
        { GLFW_KEY_S               , InputKeyEvent::Key::KEY_S             },
        { GLFW_KEY_T               , InputKeyEvent::Key::KEY_T             },
        { GLFW_KEY_U               , InputKeyEvent::Key::KEY_U             },
        { GLFW_KEY_V               , InputKeyEvent::Key::KEY_V             },
        { GLFW_KEY_W               , InputKeyEvent::Key::KEY_W             },
        { GLFW_KEY_X               , InputKeyEvent::Key::KEY_X             },
        { GLFW_KEY_Y               , InputKeyEvent::Key::KEY_Y             },
        { GLFW_KEY_Z               , InputKeyEvent::Key::KEY_Z             },
        { GLFW_KEY_LEFT_BRACKET    , InputKeyEvent::Key::KEY_LEFT_BRACKET  },
        { GLFW_KEY_BACKSLASH       , InputKeyEvent::Key::KEY_BACK_SLASH    },
        { GLFW_KEY_RIGHT_BRACKET   , InputKeyEvent::Key::KEY_RIGHT_BRACKET },
        { GLFW_KEY_GRAVE_ACCENT    , InputKeyEvent::Key::KEY_GRAVE         },
        { GLFW_KEY_WORLD_1         , InputKeyEvent::Key::KEY_GRAVE         },
        { GLFW_KEY_WORLD_2         , InputKeyEvent::Key::KEY_NONE          },
        
        /* Function keys */
        { GLFW_KEY_ESCAPE          , InputKeyEvent::Key::KEY_ESCAPE        },
        { GLFW_KEY_ENTER           , InputKeyEvent::Key::KEY_ENTER      },
        { GLFW_KEY_TAB             , InputKeyEvent::Key::KEY_TAB           },
        { GLFW_KEY_BACKSPACE       , InputKeyEvent::Key::KEY_BACKSPACE     },
        { GLFW_KEY_INSERT          , InputKeyEvent::Key::KEY_INSERT        },
        { GLFW_KEY_DELETE          , InputKeyEvent::Key::KEY_DELETE        },
        { GLFW_KEY_RIGHT           , InputKeyEvent::Key::KEY_RIGHT_ARROW   },
        { GLFW_KEY_LEFT            , InputKeyEvent::Key::KEY_LEFT_ARROW    },
        { GLFW_KEY_DOWN            , InputKeyEvent::Key::KEY_DOWN_ARROW    },
        { GLFW_KEY_UP              , InputKeyEvent::Key::KEY_UP_ARROW      },
        { GLFW_KEY_PAGE_UP         , InputKeyEvent::Key::KEY_PG_UP      },
        { GLFW_KEY_PAGE_DOWN       , InputKeyEvent::Key::KEY_PG_DOWN    },
        { GLFW_KEY_HOME            , InputKeyEvent::Key::KEY_HOME       },
        { GLFW_KEY_END             , InputKeyEvent::Key::KEY_END           },
        { GLFW_KEY_CAPS_LOCK       , InputKeyEvent::Key::KEY_CAPS_LOCK     },
        { GLFW_KEY_SCROLL_LOCK     , InputKeyEvent::Key::KEY_SCROLL_LOCK   },
        { GLFW_KEY_NUM_LOCK        , InputKeyEvent::Key::KEY_NUM_LOCK      },
        { GLFW_KEY_PRINT_SCREEN    , InputKeyEvent::Key::KEY_PRINT         },
        { GLFW_KEY_PAUSE           , InputKeyEvent::Key::KEY_PAUSE         },
        { GLFW_KEY_F1              , InputKeyEvent::Key::KEY_F1            },
        { GLFW_KEY_F2              , InputKeyEvent::Key::KEY_F2            },
        { GLFW_KEY_F3              , InputKeyEvent::Key::KEY_F3            },
        { GLFW_KEY_F4              , InputKeyEvent::Key::KEY_F4            },
        { GLFW_KEY_F5              , InputKeyEvent::Key::KEY_F5            },
        { GLFW_KEY_F6              , InputKeyEvent::Key::KEY_F6            },
        { GLFW_KEY_F7              , InputKeyEvent::Key::KEY_F7            },
        { GLFW_KEY_F8              , InputKeyEvent::Key::KEY_F8            },
        { GLFW_KEY_F9              , InputKeyEvent::Key::KEY_F9            },
        { GLFW_KEY_F10             , InputKeyEvent::Key::KEY_F10           },
        { GLFW_KEY_F11             , InputKeyEvent::Key::KEY_F11           },
        { GLFW_KEY_F12             , InputKeyEvent::Key::KEY_F12           },
        { GLFW_KEY_F13             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F14             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F15             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F16             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F17             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F18             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F19             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F20             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F21             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F22             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F23             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F24             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_F25             , InputKeyEvent::Key::KEY_NONE          },
        { GLFW_KEY_KP_0            , InputKeyEvent::Key::KEY_0             },
        { GLFW_KEY_KP_1            , InputKeyEvent::Key::KEY_1             },
        { GLFW_KEY_KP_2            , InputKeyEvent::Key::KEY_2             },
        { GLFW_KEY_KP_3            , InputKeyEvent::Key::KEY_3             },
        { GLFW_KEY_KP_4            , InputKeyEvent::Key::KEY_4             },
        { GLFW_KEY_KP_5            , InputKeyEvent::Key::KEY_5             },
        { GLFW_KEY_KP_6            , InputKeyEvent::Key::KEY_6             },
        { GLFW_KEY_KP_7            , InputKeyEvent::Key::KEY_7             },
        { GLFW_KEY_KP_8            , InputKeyEvent::Key::KEY_8             },
        { GLFW_KEY_KP_9            , InputKeyEvent::Key::KEY_9             },
        { GLFW_KEY_KP_DECIMAL      , InputKeyEvent::Key::KEY_PERIOD        },
        { GLFW_KEY_KP_DIVIDE       , InputKeyEvent::Key::KEY_KP_DIVIDE     },
        { GLFW_KEY_KP_MULTIPLY     , InputKeyEvent::Key::KEY_KP_MULTIPLY   },
        { GLFW_KEY_KP_SUBTRACT     , InputKeyEvent::Key::KEY_KP_MINUS      },
        { GLFW_KEY_KP_ADD          , InputKeyEvent::Key::KEY_KP_PLUS       },
        { GLFW_KEY_KP_ENTER        , InputKeyEvent::Key::KEY_KP_ENTER      },
        { GLFW_KEY_KP_EQUAL        , InputKeyEvent::Key::KEY_EQUAL         },
        { GLFW_KEY_LEFT_SHIFT      , InputKeyEvent::Key::KEY_LEFT_SHIFT    },
        { GLFW_KEY_LEFT_CONTROL    , InputKeyEvent::Key::KEY_LEFT_CTRL     },
        { GLFW_KEY_LEFT_ALT        , InputKeyEvent::Key::KEY_LEFT_ALT      },
        { GLFW_KEY_LEFT_SUPER      , InputKeyEvent::Key::KEY_HYPER         },
        { GLFW_KEY_RIGHT_SHIFT     , InputKeyEvent::Key::KEY_RIGHT_SHIFT   },
        { GLFW_KEY_RIGHT_CONTROL   , InputKeyEvent::Key::KEY_RIGHT_CTRL    },
        { GLFW_KEY_RIGHT_ALT       , InputKeyEvent::Key::KEY_RIGHT_ALT     },
        { GLFW_KEY_RIGHT_SUPER     , InputKeyEvent::Key::KEY_HYPER         },
        { GLFW_KEY_MENU            , InputKeyEvent::Key::KEY_MENU          },
        { GLFW_KEY_LAST            , InputKeyEvent::Key::KEY_NONE          }
    };
    
    class glCallbacks {
    public:
        static void drop_cb(GLFWwindow* w, int count, const char** path) {
            if (nullptr == glComponentInstance) {
                return;
            }
            glComponentInstance->onGLFWDrop(w, count, path);
        }

        static void key_cb(GLFWwindow* w, int key , int code , int action, int mod) {
            if (nullptr == glComponentInstance) {
                return;
            }
            glComponentInstance->onGLFWKey(w, key, code, action, mod);
        }
        
        static void mouse_button_cb(GLFWwindow* w, int btn, int action, int mod) {
            if (nullptr == glComponentInstance) {
                return;
            }
            glComponentInstance->onGLFWMouseButton(w, btn, action, mod);
        }
        
        static void mouse_move_cb(GLFWwindow* w, double x, double y) {
            if (nullptr == glComponentInstance) {
                return;
            }
            glComponentInstance->onGLFWMouseMove(w, x, y);
        }
        
    };
    
    
    
    GLComponent::GLComponent() {
        assert(nullptr == glComponentInstance);
        screenSize = glm::vec2(960, 640);
        clearColor = glm::vec4(0, 0, 0, 255);
    }
    
    GLComponent::~GLComponent() {
    }

    void GLComponent::setSize(const glm::vec2& size) {
        screenSize = size;
    }
    
    void GLComponent::setClearColor(const glm::vec4& clr) {
        clearColor = clr;
    }
    
    void GLComponent::initGLFW() {
        assert(nullptr != host);
        if (GL_TRUE != glfwInit()) {
            return ;
        }
        
        window = glfwCreateWindow(screenSize.x, screenSize.y, host->getName().c_str(), nullptr, nullptr);
        if (nullptr == window) {
            glfwTerminate();
            return ;
        }

        glfwMakeContextCurrent(window);
        glfwSetDropCallback(window, glCallbacks::drop_cb);
        glfwSetKeyCallback(window, glCallbacks::key_cb);
        glfwSetMouseButtonCallback(window, glCallbacks::mouse_button_cb);
        glfwSetCursorPosCallback(window, glCallbacks::mouse_move_cb);
        
        glEnable(GL_DEPTH_TEST);
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    }
    
    void GLComponent::clearGL() {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }
    
    void GLComponent::swap() {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    void GLComponent::check() {
        GLenum error = glGetError();
        if (GL_NO_ERROR == error) {
            return;
        }
        LOGD("GLComponent", "error");
    }
    
    void GLComponent::onEnter() {
    }

    void GLComponent::onGLFWDrop(GLFWwindow* w, int count, const char** path) {
        const char* file = nullptr;
        std::vector<std::string> paths;

        for (auto i = 0; i < count; i ++) {
            file = *(path + i);
            paths.push_back(file);
            Importer::import(file);
        }
    }

    void GLComponent::onGLFWKey(GLFWwindow* w, int key , int code , int action, int mod) {
        InputKeyEvent::Action act = InputKeyEvent::Action::Press;
        switch (action) {
            case GLFW_PRESS: {
                act = InputKeyEvent::Action::Press;
                break;
            }
                
            case GLFW_REPEAT: {
                act = InputKeyEvent::Action::Repeat;
                break;
            }

            case GLFW_RELEASE: {
                act = InputKeyEvent::Action::Release;
                break;
            }
                
            default: {
                break;
            }
        }
        InputKeyEvent *keyEvent = new InputKeyEvent(InputBaseEvent::InputType::Key);
        keyEvent->setAction(act);
        keyEvent->setKey(g_keyCodeMap[key]);
        
        Manager::getInstance()->postEvent(static_cast<int>(EventComponent::Event::EVENT_INPUT_KEY), keyEvent);
    }
    
    void GLComponent::onGLFWMouseButton(GLFWwindow* w, int button, int action, int mod) {
        InputTouchEvent::Action act = InputTouchEvent::Action::Press;
        switch (action) {
            case GLFW_PRESS: {
                act = InputTouchEvent::Action::Press;
                break;
            }
                
            case GLFW_RELEASE: {
                act = InputTouchEvent::Action::Release;
                break;
            }
                
            default: {
                return;
            }
        }
        InputTouchEvent *touch = new InputTouchEvent(InputBaseEvent::InputType::Touch);
        touch->setAction(act);
        
        double x = 0;
        double y = 0;
        glfwGetCursorPos(w, &x, &y);
        touch->setPosition(x, y);
        
        Manager::getInstance()->postEvent(static_cast<int>(EventComponent::Event::EVENT_INPUT_TOUCH), touch);
    }
    
    void GLComponent::onGLFWMouseMove(GLFWwindow* w, double x, double y) {
        if (GLFW_PRESS == glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT)) {
            InputTouchEvent *touch = new InputTouchEvent(InputBaseEvent::InputType::Touch);
            touch->setAction(InputTouchEvent::Action::Move);
            touch->setPosition(x, y);
            
            Manager::getInstance()->postEvent(static_cast<int>(EventComponent::Event::EVENT_INPUT_TOUCH), touch);
        }
    }
}