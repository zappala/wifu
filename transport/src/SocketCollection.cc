#include "SocketCollection.h"

SocketCollection::SocketCollection() {
    reset();
}

SocketCollection& SocketCollection::operator=(const SocketCollection& other) {
}

SocketCollection::SocketCollection(const SocketCollection& other) {
}

SocketCollection& SocketCollection::instance() {
    static SocketCollection instance_;
    return instance_;
}

SocketCollection::~SocketCollection() {
}

Socket* SocketCollection::get_by_id(int id) {
    assert(id >= 0);
    SocketCollectionGetByIdVisitor visitor(id);
    accept(&visitor);
    return visitor.get_socket();
}

Socket* SocketCollection::get_by_local_ap(AddressPort* local) {
    assert(local != NULL);
    SocketCollectionGetByLocalAddressPortVisitor visitor(local);
    accept(&visitor);
    return visitor.get_socket();
}

Socket* SocketCollection::get_by_local_and_remote_ap(AddressPort* local, AddressPort* remote) {
    assert(local != NULL);
    assert(remote != NULL);

    mutex_->wait();
    itr_ = collection_.find(Socket::make_key(local, remote));
    Socket* value = itr_ == collection_.end() ? NULL : itr_->second;
    mutex_->post();
    return value;
}

void SocketCollection::push(Socket* s) {
    mutex_->wait();
    s->add_observer(this);
    collection_[s->get_key()] = s;
    mutex_->post();
}

void SocketCollection::remove(Socket* s) {
    mutex_->wait();

    // Try to find by key first
    itr_ = collection_.find(s->get_key());

    if (itr_ == collection_.end()) {

        // Find by pointer
        itr_ = collection_.begin();
        for (; itr_ != collection_.end(); ++itr_) {
            if (s == itr_->second) {
                break;
            }
        }
    }

    if (itr_ != collection_.end()) {
        collection_.erase(itr_);
    }
    mutex_->post();
}

int SocketCollection::size() {
    mutex_->wait();
    int val = collection_.size();
    mutex_->post();
    return val;
}

int SocketCollection::clear() {
    mutex_->wait();
    collection_.clear();
    mutex_->post();
}

void SocketCollection::accept(Visitor* v) {
    mutex_->wait();
    for (itr_ = collection_.begin(); itr_ != collection_.end(); ++itr_) {
        v->visit(itr_->second);
        if (v->stop()) {
            break;
        }
    }
    mutex_->post();
}

void SocketCollection::update(Observable* o) {
    update_mutex_->wait();
    if (typeid (*o) == typeid (Socket)) {
        Socket* s = static_cast<Socket*>(o);
        remove(s);
        push(s);
    }
    update_mutex_->post();
}

void SocketCollection::reset() {
    mutex_ = new Semaphore();
    update_mutex_ = new Semaphore();

    mutex_->init(1);
    update_mutex_->init(1);

    collection_.clear();
}
