#include "SocketCollection.h"

// functions used in sorting

bool id_cmp(Socket* a, Socket* b) {
    return a->get_socket_id() < b->get_socket_id();
}

bool ap_cmp(Socket* a, Socket* b) {
    string astring = a->get_local_address_port()->to_s();
    string bstring = b->get_local_address_port()->to_s();

    if (astring < bstring) {
        return true;
    } else if (astring == bstring) {
        return a->get_remote_address_port()->to_s() < b->get_remote_address_port()->to_s();
    }

    return false;
}

// functions used in binary search

int bsearch_id_cmp(int& value, Socket* s) {
    return value - s->get_socket_id();
}

int b_search_local_cmp(AddressPort* local, Socket* s) {
    return local->to_s().compare(s->get_local_address_port()->to_s());
}

// first element in pair is local, second is remote

int b_search_local_remote_cmp(pair<AddressPort*, AddressPort*>& aps, Socket* s) {
    AddressPort* local = aps.first;
    AddressPort* remote = aps.second;

    int value = local->to_s().compare(s->get_local_address_port()->to_s());
    if (value < 0) {
        return -1;
    } else if (value > 0) {
        return 1;
    }

    // locals are the same, compare remote
    return remote->to_s().compare(s->get_remote_address_port()->to_s());

}



SocketCollection::SocketCollection() : how_sorted_(RE_SORT) {
	mutex_.init(1);
}

SocketCollection& SocketCollection::operator=(const SocketCollection& other) {}

SocketCollection::SocketCollection(const SocketCollection& other) {}

template<typename Type1, typename Type2>
Socket* SocketCollection::binary_search(Type1 target, int (*compare)(Type2, Socket*)) {
	typedef vector<Socket*>::iterator itr;
	itr begin = collection_.begin();
	itr end = collection_.end();

	while (begin < end) {
		itr middle = begin + (end - begin - 1) / 2;
		int value = compare(target, *middle);

		if (value < 0) {
			end = middle;
		} else if (value > 0) {
			begin = middle + 1;
		} else {
			return *middle;
		}
	}
	return NULL;
}

template
Socket* SocketCollection::binary_search<int, int&>(int, int(*)(int&, Socket*));
template
Socket* SocketCollection::binary_search<AddressPort*, AddressPort*>(AddressPort*, int (*)(AddressPort*, Socket*));
template
Socket* SocketCollection::binary_search<pair<AddressPort*, AddressPort*>, pair<AddressPort*, AddressPort*>&>(pair<AddressPort*, AddressPort*>, int (*)(pair<AddressPort*, AddressPort*>&, Socket*));

void SocketCollection::socket_sort(bool(*f)(Socket*, Socket*)) {
	sort(collection_.begin(), collection_.end(), f);
}

void SocketCollection::mark_dirty() {
	how_sorted_ = RE_SORT;
}

SocketCollection& SocketCollection::instance() {
	static SocketCollection instance_;
	return instance_;
}

SocketCollection::~SocketCollection() {
}

Socket* SocketCollection::get_by_id(int id) {
	assert(id >= 0);

	mutex_.wait();
	if (how_sorted_ != ID) {
		socket_sort(id_cmp);
		how_sorted_ = ID;
	}
	Socket* value = binary_search(id, bsearch_id_cmp);
	mutex_.post();
	return value;
}

Socket* SocketCollection::get_by_local_ap(AddressPort* local) {
	assert(local != NULL);

	mutex_.wait();
	if (how_sorted_ != AP) {
		socket_sort(ap_cmp);
		how_sorted_ = AP;
	}

	Socket* value = binary_search(local, b_search_local_cmp);
	mutex_.post();
	return value;
}

Socket* SocketCollection::get_by_local_and_remote_ap(AddressPort* local, AddressPort* remote) {
	assert(local != NULL);
	assert(remote != NULL);

	mutex_.wait();
	if (how_sorted_ != AP) {
		socket_sort(ap_cmp);
		how_sorted_ = AP;
	}

	pair<AddressPort*, AddressPort*> p = make_pair(local, remote);
	Socket* value = binary_search(p, b_search_local_remote_cmp);
	mutex_.post();
	return value;
}

void SocketCollection::push(Socket* s) {
	mutex_.wait();
	s->add_observer(this);
	collection_.push_back(s);
	how_sorted_ = RE_SORT;
	mutex_.post();
}

int SocketCollection::size() {
	mutex_.wait();
	int val = collection_.size();
	mutex_.post();
	return val;
}

int SocketCollection::clear() {
	mutex_.wait();
	collection_.clear();
	mutex_.post();
}

void SocketCollection::shuffle() {
	mutex_.wait();
	random_shuffle(collection_.begin(), collection_.end());
	mutex_.post();
}

void SocketCollection::accept(Visitor* v) {
	mutex_.wait();
	int size = collection_.size();
	for (int i = 0; i < size; i++) {
		v->visit(collection_[i]);
		if (v->stop()) {
			break;
		}
	}
	mutex_.post();
}

void SocketCollection::update(Observable* o) {
	// TODO: assert that o is an instance of Socket*

	// We simply want to mark the collection dirty so we will re-sort it
	mark_dirty();
}
