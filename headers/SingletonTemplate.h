/* 
 * File:   SingletonTemplate.h
 * Author: rbuck
 *
 * Created on October 23, 2010, 9:03 AM
 */

#ifndef _SINGLETONTEMPLATE_H
#define	_SINGLETONTEMPLATE_H

#include <stdlib.h>
#include <assert.h>
#include <iostream>

using namespace std;

template <class T>
class Singleton {
public:

    static T* instance() {
        if (!instance_) {
            instance_ = new T;
        }
        cout << "Grabbing instance" << endl;

        assert(instance_ != NULL);
        return instance_;

    }

    void destroy() {
        delete instance();
        instance_ = NULL;
    }

protected:

    Singleton() {

    }

    virtual ~Singleton() {

    }

private:

    Singleton(Singleton const&) {

    }

    Singleton & operator=(Singleton const&) {

    }

    static T* instance_;
};

template <class T> T* Singleton<T>::instance_ = NULL;

#endif	/* _SINGLETONTEMPLATE_H */

