//
//  Reference.hpp
//  dragon
//
//  Created by hugo on 16/4/12.
//
//

#ifndef Reference_hpp
#define Reference_hpp

namespace dragon {
    class Reference {
    public:
        Reference();
        virtual ~Reference();

        void retain();
        void release();
        unsigned int getReferenceCount() const;

    protected:
        unsigned int referenceCount;
    };
}

#endif /* Reference_hpp */
