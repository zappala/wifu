#include "events/ResponseEvent.h"

ResponseEvent::ResponseEvent(Socket* socket, string& name, string& file) : Event(socket), name_(name), file_(file) {}

ResponseEvent::~ResponseEvent() {}

string ResponseEvent::get_response() {
	m_[SOCKET_STRING] = Utils::itoa(get_socket()->get_socket_id());
	return QueryStringParser::create(name_, m_);
}

// TODO: fix this so we can pass references
void ResponseEvent::put(string key, string value) {
	m_[key] = value;
}

// TODO: fix this so we can pass references
void ResponseEvent::put(const char* key, string value) {
	string k = string(key);
	put(k, value);
}

void ResponseEvent::execute(IModule* m) {
	m->library_response(this);
}

string ResponseEvent::get_write_file() {
	return file_;
}
