#include "events/framework_events/ResponseEvent.h"

ResponseEvent::ResponseEvent(Socket* socket, gcstring& name, gcstring& file) : FrameworkEvent(socket), name_(name), file_(file) {

}

ResponseEvent::~ResponseEvent() {

}

gcstring ResponseEvent::get_response() {
    m_[SOCKET_STRING] = Utils::itoa(get_socket()->get_socket_id());
    return QueryStringParser::create(name_, m_);
}

// TODO: fix this so we can pass references

void ResponseEvent::put(gcstring key, gcstring value) {
    m_[key] = value;
}

// TODO: fix this so we can pass references

void ResponseEvent::put(const char* key, gcstring value) {
    gcstring k = gcstring(key);
    put(k, value);
}

void ResponseEvent::execute(IModule* m) {
    m->imodule_library_response(this);
}

gcstring ResponseEvent::get_write_file() {
    return file_;
}
