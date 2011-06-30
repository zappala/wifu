#include "TCPFewIContextContainerFactory.h"

BasicIContextContainer* TCPFewIContextContainerFactory::get_icontext_container() {
    return new TCPFewIContextContainer();
}
