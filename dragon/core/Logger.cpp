//
//  Logger.cpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#include "Logger.hpp"

namespace dragon {
#define LOG_IMPL(level) \
    va_list argList; \
    va_start(argList, format); \
    log(level, tag, format, argList); \
    va_end(argList)

    
    Logger* Logger::instance = nullptr;
    
    Logger* Logger::getInstance() {
        if (nullptr == instance) {
            instance = new Logger();
        }
        return instance;
    }
    
    void Logger::d(const char* tag, const char* format, ...) {
        LOG_IMPL(Level::LOG_DEBUG);
    }

    void Logger::i(const char* tag, const char* format, ...) {
        LOG_IMPL(Level::LOG_INFO);
    }
    
    void Logger::w(const char* tag, const char* format, ...) {
        LOG_IMPL(Level::LOG_WARN);
    }
    
    void Logger::e(const char* tag, const char* format, ...) {
        LOG_IMPL(Level::LOG_ERROR);
    }

    void Logger::log(Level l, const char* tag, const char* format, va_list argList) {
        char buf[4096];
        int pos= 0;
        pos = snprintf(buf, sizeof(buf), "%s: ", tag);
        vsnprintf(buf+pos, sizeof(buf)-pos , format, argList);
        fprintf(stdout, "%s\n", buf);
        fflush(stdout);
    }
}
