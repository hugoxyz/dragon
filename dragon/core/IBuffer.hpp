//
//  IBuffer.hpp
//  dragon
//
//  Created by hugo on 16/6/16.
//
//

#ifndef IBuffer_hpp
#define IBuffer_hpp

namespace dragon {
    class IBuffer {
    public:
        IBuffer();
        ~IBuffer();

        void* increase(int unit = 1);
        
        /*
         * pos:  0, return begin pos
         *      -1, return current pos
         */
        void* getBufferPoint(int pos);

    protected:
        void *buffer;
        int unitSize;
        int capacity;
        int length;
        float step;
    };
}

#endif /* IBuffer_hpp */
