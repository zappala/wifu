#include "TCPDelayedACKIContextContainerFactory.h"

BasicIContextContainer* TCPDelayedACKIContextContainerFactory::get_icontext_container() {
    return new TCPDelayedACKIContextContainer();
}
