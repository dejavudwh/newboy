#ifndef __NEWBOY_SINGLETON_H__
#define __NEWBOY_SINGLETON_H__

namespace newboy {

template<class T, class X = void, int N = 0>
class Singleton {
    public:
        static T* GetInstance() {
            static T v;
            return &v;
        }
};

}

#endif