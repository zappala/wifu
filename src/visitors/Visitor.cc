#include "Visitor.h"

Visitor::~Visitor() {}

bool Visitor::stop() {
	return false;
}
