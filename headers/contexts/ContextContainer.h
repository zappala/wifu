/* 
 * File:   ContextContainer.h
 * Author: rbuck
 *
 * Created on December 31, 2010, 9:52 AM
 */

#ifndef CONTEXTCONTAINER_H
#define	CONTEXTCONTAINER_H

#include <vector>
#include "Context.h"
#include "GarbageCollector.h"

using namespace std;


class ContextContainer : public gc {
public:
    ContextContainer() {

    }

    virtual ~ContextContainer() {

    }

    void add_context(Context* c) {
        v_.push_back(c);
    }

    vector<Context*>& get_contexts() {
        return v_;
    }


private:
    vector<Context*> v_;
};

#endif	/* CONTEXTCONTAINER_H */

