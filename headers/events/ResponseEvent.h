/* 
 * File:   ResponseEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 2:36 PM
 */

#ifndef RESPONSEEVENT_H
#define	RESPONSEEVENT_H

#include <string>
#include <map>

#include "Event.h"
#include "QueryStringParser.h"

using namespace std;

class ResponseEvent : public Event {
public:
    ResponseEvent(int socket, string& name, string& file) : Event(socket), name_(name), file_(file) {
        
        
    }

    virtual ~ResponseEvent() {

    }

    string get_response() {
        m_[SOCKET_STRING] = Utils::itoa(get_socket());
        return QueryStringParser::create(name_, m_);
    }

    // TODO: fix this so we can pass references
    void put(string key, string value) {
        m_[key] = value;
    }

    // TODO: fix this so we can pass references
    void put(const char* key, string value) {
        string k = string(key);
        put(k, value);
    }

    void execute(IModule* m) {
        m->library_response(this);
    }

    string get_write_file() {
        return file_;
    }

private:
    string name_;
    string file_;
    map<string, string> m_;
            
};

#endif	/* RESPONSEEVENT_H */

