//
//  Singleton.h
//  Othello
//
//  Created by Snow on 1/19/14.
//
//

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <typename T>
class Singleton {
public:
    template<typename... Args>
    static T* getInstance(Args... args) {
        if(_m_pInstance==nullptr)
            _m_pInstance = new T(args...);
        
        return _m_pInstance;
    }
    
    static void destroyInstance() {
        delete _m_pInstance;
        _m_pInstance = nullptr;
    }
    
private:
    Singleton(void);
    virtual ~Singleton(void);
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);
    
private:
    static T* _m_pInstance;
};

template<typename T> T* Singleton<T>::_m_pInstance = nullptr;

#endif
