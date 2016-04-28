#import <Foundation/Foundation.h>

#include "FileUtils.hpp"

namespace dragon {

    bool FileUtils::nativeIsFile(std::string &filePath) {
        if (filePath.empty()) {
            return false;
        }
        
        bool ret = false;
        
        if ('/' != filePath[0]) {
            std::string path;
            std::string file;
            size_t pos = filePath.find_last_of("/");
            if (pos != std::string::npos) {
                file = filePath.substr(pos+1);
                path = filePath.substr(0, pos+1);
            }
            else
            {
                file = filePath;
            }
            NSBundle* bundle = [NSBundle mainBundle];
            NSString* fullpath = [bundle pathForResource:[NSString stringWithUTF8String:file.c_str()]
                                                       ofType:nil
                                                  inDirectory:[NSString stringWithUTF8String:path.c_str()]];
            if (fullpath != nil) {
                ret = true;
            }
        } else {
            // Search path is an absolute path.
            NSFileManager* s_fileManager = [NSFileManager defaultManager];
            if ([s_fileManager fileExistsAtPath:[NSString stringWithUTF8String:filePath.c_str()]]) {
                ret = true;
            }
        }
        
        return ret;
    }

}
