#include "LibraryEvent.h"

LibraryEvent::LibraryEvent(string& message, string& file, Socket* socket) : Event(socket), file_(file) {
	QueryStringParser::parse(message, m_);
	name_ = m_[NAME_STRING];

	m_[SOCKET_STRING] = Utils::itoa(socket->get_socket_id());
}

LibraryEvent::~LibraryEvent() {}

string& LibraryEvent::get_file() {
	return file_;
}

string& LibraryEvent::get_name() {
	return name_;
}

map<string, string>& LibraryEvent::get_map() {
	return m_;
}
