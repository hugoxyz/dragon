//
//  Logger.hpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#ifndef Logger_hpp
#define Logger_hpp

#include <string>

namespace dragon {

    #define LOGD(tag, ...) dragon::Logger::getInstance()->d(tag, __VA_ARGS__);
    #define LOGI(tag, ...) dragon::Logger::getInstance()->i(tag, __VA_ARGS__);
    #define LOGW(tag, ...) dragon::Logger::getInstance()->w(tag, __VA_ARGS__);
    #define LOGE(tag, ...) dragon::Logger::getInstance()->e(tag, __VA_ARGS__);

    class Logger {
    public:
        enum class Level {
            LOG_DEBUG,
            LOG_INFO,
            LOG_WARN,
            LOG_ERROR
        };
        
        static Logger* getInstance();

        void d(const char* tag, const char* format, ...);
        void i(const char* tag, const char* format, ...);
        void w(const char* tag, const char* format, ...);
        void e(const char* tag, const char* format, ...);
        
        void setLevel(Level l) { level = l; };
        
    protected:
        void log(Level l, const char* tag, const char* format, va_list va);

    protected:
        Level level;
        
    private:
        static Logger* instance;        
    };
}

#endif /* Logger_hpp */
