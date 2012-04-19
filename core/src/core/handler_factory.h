#ifndef _handler_factory_h
#define _handler_factory_h

#include <string>

#include "handler.h"
#include "test/test_handler.h"

// Returns an instance of a handler assiciated with the supplied name
//
// name: name of handler
//
// returns an instance of Handler, or one of its derived classes
Handler* create_handler(std::string name);

#endif // _handler_factory_h
