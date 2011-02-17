#include "Message.h"

Message::Message(int size, char* message) {
	strcpy(c_string_, message);
}

Message::Message(int size, string& message) {
	string_ = message;
	c_string_ = 0;
}

Message::~Message() {}

string Message::to_s() {
	if (c_string_)
		return string(c_string_);
	else
		return string_;
}

char* Message::c_str() {
	if (!c_string_)
		strcpy(c_string_, string_.c_str());

	return c_string_;
}
