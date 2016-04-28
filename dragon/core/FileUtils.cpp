
#include <stdlib.h>

#include "FileUtils.hpp"
#include "../core/Logger.hpp"

namespace dragon {

    FileUtils* FileUtils::_instance = nullptr;

    FileUtils::FileUtils() {
        _searchPaths.clear();
        _searchPaths.push_back("");
    }

    FileUtils::~FileUtils() {
    }

    FileUtils* FileUtils::getInstance() {
        if (nullptr == _instance) {
            _instance = new FileUtils();
        }
        
        return _instance;
    }

    bool FileUtils::addSearchPath(const std::string& path) {
        _searchPaths.push_back(path);
        return true;
    }

    std::string FileUtils::fullPath(const std::string& fileName) {
        for (auto dir : _searchPaths) {
            std::string path = append(dir, fileName);
            if (nativeIsFile(path)) {
                return path;
            }
        }
        
        return "";
    }

    bool FileUtils::read(const std::string& fileName, unsigned char** data, int* dataLen, bool isString) {
        std::string path = fullPath(fileName);
        if (0 == path.length()) {
            LOGE("FileUtils", "FileUtils read fail");
            return false;
        }

        bool bRet = false;
        do {
            const char* mode = nullptr;
            if (isString) {
                mode = "rt";
            } else {
                mode = "rb";
            }
            FILE *fp = fopen(path.c_str(), mode);
            if (nullptr == fp) {
                return false;
            }
            fseek(fp,0,SEEK_END);
            long size = ftell(fp);
            fseek(fp,0,SEEK_SET);
            
            unsigned char* buffer = nullptr;
            buffer = (unsigned char*)malloc(sizeof(unsigned char) * (size + 1));
            if (nullptr == buffer) {
                fclose(fp);
                fp = nullptr;
                return false;
            }

            memset(buffer, 0, sizeof(unsigned char) * (size + 1));
            
            long readsize = fread(buffer, sizeof(unsigned char), size, fp);
            fclose(fp);
            if (readsize < size) {
                buffer[readsize] = '\0';
            }
            
            *data = buffer;
            *dataLen = (int)readsize;
            bRet = true;
        } while (0);
        
        return bRet;
    }

    std::string FileUtils::readStringFromFile(const std::string& fileName) {
        unsigned char* pData = nullptr;
        int dataLen = 0;
        if (!read(fileName, &pData, &dataLen, true)) {
            return "";
        }
        
        std::string content = (char*)pData;
        free(pData);
        return content;
    }

    bool FileUtils::write(std::string& fileName, const unsigned char* data, int dataLen) {
        std::string path = fullPath(fileName);
        if (0 == path.length() || nullptr == data || 0 == dataLen) {
            LOGE("FileUtils", "FileUtils read fail");
            return false;
        }
        
        bool bRet = false;
        do {
            FILE *fp = fopen(path.c_str(), "wb");
            if (nullptr == fp) {
                return false;
            }
            size_t wSize = fwrite(data, sizeof(unsigned char), dataLen, fp);
            fclose(fp);
            fp = nullptr;
            if (wSize != dataLen) {
                return false;
            }
            bRet = true;
        } while (0);
        
        return bRet;
    }

    bool FileUtils::createDir(std::string& path) {
        return true;
    }

    bool FileUtils::remove(std::string& fileName) {
        return true;
    }

    long FileUtils::fileSize(std::string& fileName) {
        return 0;
    }

    bool FileUtils::isAbsolutePath(const std::string& path) const {
        return (path[0] == '/');
    }

    std::string FileUtils::parentDir(std::string& path) {
        if (0 == path.length()) {
            return "";
        }
        size_t len = path.length();
        if ('/' == path[path.length() - 1]) {
            len--;
        }
        size_t pos = path.rfind("/", 0, len);
        if (std::string::npos == pos) {
            return "";
        }
        
        return path.substr(0, pos + 1);
    }

    std::string FileUtils::getPath(std::string& fileName) {
        if (0 == fileName.length()) {
            return "";
        }
        if ('/' == fileName[fileName.length() - 1]) {
            return fileName.substr(0, fileName.length() - 1);
        } else {
            return parentDir(fileName);
        }
    }

    std::string FileUtils::append(const std::string& dir, const std::string& file) {
        if (0 == dir.length()) {
            return file;
        }
        
        std::string newDir;
        if ('/' ==  dir[dir.length() - 1]) {
            newDir = dir + file;
            return newDir;
        } else {
            newDir = dir + '/' + file;
            return newDir;
        }
    }

}
