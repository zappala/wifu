#include "TCPTahoeIContextContainerFactory.h"

BasicIContextContainer* TCPTahoeIContextContainerFactory::get_icontext_container() {
    return new TCPTahoeIContextContainer();
}
