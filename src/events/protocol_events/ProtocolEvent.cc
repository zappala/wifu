#include "events/protocol_events/ProtocolEvent.h"

ProtocolEvent::ProtocolEvent(Socket* s) : Event(s, HIGH) {

}

ProtocolEvent::~ProtocolEvent() {
    
}
