//
//  FileModule.cpp
//  dragon
//
//  Created by hugo on 16/4/20.
//
//

#include "FileModule.hpp"
#include "Logger.hpp"
#include "FileUtils.hpp"
#include "Manager.hpp"

namespace dragon {
    void FileModule::onMessage(dragon::Message *msg) {
        if (nullptr == msg) {
            return;
        }
        if (Message::Type::StringValue != msg->getType()) {
            LOGD("FileModule", "just support string type message");
            return;
        }
        std::string path = msg->getStringValue();
        msg->clearValue();

        unsigned char* data = nullptr;
        int dataLen = 0;
        if (FileUtils::getInstance()->read(path, &data, &dataLen, false)) {
            msg->setValue(data, dataLen);
        } else {
            std::string error = "open file fail:" + path;
            msg->setValue(error);
        }
        
        int tid = msg->sender;
        msg->sender = msg->receiver;
        msg->receiver = tid;
        Manager::getInstance()->sendMsg(msg);
    }
}
