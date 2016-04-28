
#ifndef __ST_FILE_UTILS_H__
#define __ST_FILE_UTILS_H__

#include <string>
#include <vector>

namespace dragon {

    class FileUtils {
    public:
        ~FileUtils();
        static FileUtils* getInstance();

        bool addSearchPath(const std::string& path);
        std::string fullPath(const std::string& fileName);

        bool read(const std::string& fileName, unsigned char** data, int* dataLen, bool isString = false);
        std::string readStringFromFile(const std::string& fileName);
        bool write(std::string& fileName, const unsigned char* data, int dataLen);
        bool createDir(std::string& path);
        
        bool remove(std::string& fileName);
        
        long fileSize(std::string& fileName);
        
        bool isAbsolutePath(const std::string& path) const;
        std::string parentDir(std::string& path);
        std::string getPath(std::string& fileName);
        std::string append(const std::string& dir, const std::string& file);

    protected:

        FileUtils();
        
        bool nativeIsFile(std::string& file);
        
    protected:

        std::vector<std::string> _searchPaths;
        
    private:
        static FileUtils* _instance;
    };

}


#endif    // __ST_FILE_UTILS_H__
