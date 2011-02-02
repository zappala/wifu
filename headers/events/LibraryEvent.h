/* 
 * File:   LibraryEvent.h
 * Author: rbuck
 *
 * Created on December 27, 2010, 1:43 PM
 */

#ifndef LIBRARYEVENT_H
#define	LIBRARYEVENT_H

#include <string>
#include <map>
#include <stdlib.h>

#include "defines.h"
#include "Event.h"
#include "QueryStringParser.h"
#include "Utils.h"

using namespace std;

class LibraryEvent : public Event {
public:

    LibraryEvent(string& message, string& file, Socket* socket) : Event(socket), file_(file) {
        QueryStringParser::parse(message, m_);
        name_ = m_[NAME_STRING];

        m_[SOCKET_STRING] = Utils::itoa(socket->get_socket_id());
    }

    virtual ~LibraryEvent() {

    }

    string& get_file() {
        return file_;
    }

    string& get_name() {
        return name_;
    }

    map<string, string>& get_map() {
        return m_;
    }

private:
    map<string, string> m_;
    string name_;
    string file_;
};

#endif	/* LIBRARYEVENT_H */

