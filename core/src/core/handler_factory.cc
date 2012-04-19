#include "handler_factory.h"

Handler* create_handler(std::string name) {
    if (name == "default") {
        return new Handler();
    }
    if (name == "test") {
        return new TestHandler();
    }
}
