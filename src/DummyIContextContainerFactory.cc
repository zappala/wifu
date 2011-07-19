#include "DummyIContextContainerFactory.h"

BasicIContextContainer* DummyIContextContainerFactory::get_icontext_container() {
    return new DummyProtocolIContextContainer();
}
