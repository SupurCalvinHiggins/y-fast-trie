#pragma once

template <typename T>
class XFastTrieNode {
public:
	XFastTrieNode(T key, XFastTrieNode<T>* left, XFastTrieNode<T>* right);
	T key;
	XFastTrieNode<T>* children[2];
}

template <typename T>
XFastTrieNode<T>::XFastTrieNode(T key, XFastTrieNode<T>* left, XFastTrieNode<T>* right) {
	this->key = key;
	this->children[0] = left;
	this->children[1] = right;
}

template <typename T>
class XFastTrie {
private:
	size_t max_bits;
	std::vector<std::unordered_map<T, XFastTrieNode<T>*>> lss;
	T prefix(T key, T level);
	T get_highest_level(T key);
	T get_closest_key(T key);
public:
	XFastTrie();
	bool contains(T key);
	std::optional<T> predecessor(T key);
	std::optional<T> successor(T key);
	std::optional<T> min();
	std::optional<T> max();
	size_t size();
	void insert(T key);
	void remove(T key);
};

template <typename T>
XFastTrie<T>::XFastTrie() {
	max_bits = std::numeric_limits<T>::digits;
	this->lss.reserve(max_bits);
	for (size_t i = 0; i < (max_bits + 1); ++i) {
		this->lss.push_back(std::unordered_map<T, Node<T>*>());
	}
}

template <typename T>
T XFastTrie<T>::prefix(T key, T level) {
	return key >> (this->max_bits - level);
}

template <typename T>
T XFastTrie<T>::get_highest_level(T key) {
	T low_level = 0;
	T high_level = this->max_bits;

	while (low_level <= high_level) {
		T mid_level = (low_level + high_level) >> 1;
		T prefix = this->prefix(key, mid_level);
		if (this->lss[mid_level].count(prefix)) {
			low_level = mid_level + 1;
		}
		else {
			high_level = mid_level - 1;
		}
	}

	return low_level - 1;
}

template <typename T>
T XFastTrie<T>::get_closest_key(T key) {
	T highest_level = this->get_highest_level(key);
	T prefix = this->prefix(key, highest_level);
	Node<T>* node = this->lss[highest_level].at(prefix);

	if (highest_level == this->max_bits) {
		//std::cout << "shortcut: " << key << '\n';
		return node->key;
	}

	std::vector<T> closest_keys;
	closest_keys.reserve(3);

	if (this->lss.back().count(node->left->key) && (this->lss.back().at(node->left->key) == node->left)) {
		//std::cout << int(node->left->key) << '\n';
		closest_keys.push_back(node->left->key);
		if (node->left->left) closest_keys.push_back(node->left->left->key);
		if (node->left->right) closest_keys.push_back(node->left->right->key);
	}
	if (this->lss.back().count(node->right->key) && (this->lss.back().at(node->right->key) == node->right)) {
		//std::cout << int(node->right->key) << '\n';
		closest_keys.push_back(node->right->key);
		if (node->right->left) closest_keys.push_back(node->right->left->key);
		if (node->right->right) closest_keys.push_back(node->right->right->key);
	}
	
	if (closest_keys.size() == 0) {
		assert(false && "bad node");
	}

	return *std::min_element(std::begin(closest_keys), std::end(closest_keys),
		[&key](T a, T b) -> T { return (a > key ? a - key : key - a) < (b > key ? b - key : key - b); });
}

template <typename T>
Node<T>* XFastTrie<T>::predecessor(T key) {
	if (this->lss.back().empty()) return nullptr;

	T closest_key = this->get_closest_key(key);
	if (key < closest_key) {
		return this->lss.back().at(closest_key)->left;
	}
	else {
		return this->lss.back().at(closest_key);
	}
}

template <typename T>
Node<T>* XFastTrie<T>::successor(T key) {
	if (this->lss.back().empty()) return nullptr;

	T closest_key = this->get_closest_key(key);
	if (key > closest_key) {
		return this->lss.back().at(closest_key)->right;
	}
	else {
		return this->lss.back().at(closest_key);
	}
}

template <typename T>
void XFastTrie<T>::validate() {
	if (this->lss.back().empty()) return;

	std::vector<T> keys;

	for (auto p : this->lss.back()) {
		keys.push_back(p.first);
	}

	std::sort(std::begin(keys), std::end(keys));

	// validate LL forwards
	Node<T>* cur = this->lss.back().at(keys[0]);
	int idx = 0;
	while (cur) {
		assert(cur->key == keys[idx]);
		cur = cur->right;
		idx += 1;
	}
	assert(idx == keys.size());

	// validate LL backwards
	cur = this->lss.back().at(keys.back());
	idx = keys.size() - 1;
	while (cur) {
		assert(cur->key == keys[idx]);
		cur = cur->left;
		idx -= 1;
	}
	assert(idx == -1);

	// validate existance of structure
	for (T key : keys) {
		for (T level = 0; level <= this->max_bits; ++level) {
			T prefix = this->prefix(key, level);
			assert(this->lss[level].count(prefix));
		}
	}

	// ensure there are no excess nodes
	for (T level = 0; level <= this->max_bits; ++level) {
		std::vector<Node<T>*> nodes;

		for (auto p : this->lss[level]) {
			nodes.push_back(p.second);
		}

		for (T key : keys) {
			T prefix = this->prefix(key, level);
			auto node = this->lss[level].at(prefix);
			assert(std::count(std::begin(nodes), std::end(nodes), node) == 1);
		}
	}

	// validate 1 chains
	for (T key : keys) {
		for (T level = this->max_bits; level <= 1; --level) {
			T prefix = this->prefix(key, level);
			T next_prefix = this->prefix(key, level - 1);
			if (next_prefix & 1) {
				assert(this->lss[level].at(prefix)->right == this->lss[level - 1].at(next_prefix));
			}
			else {
				assert(this->lss[level].at(prefix)->left == this->lss[level - 1].at(next_prefix));
			}
		}
	}

	// validate closest key invarient
	for (auto other_key : keys) {
		//std::cout << int(other_key) << ' ';
	}
	//std::cout << '\n';
	for (int key = 0; key < 256; ++key) {
		std::vector<T> dists;
		for (auto other_key : keys) {
			dists.push_back(std::abs(int(key) - int(other_key)));
		}
		auto closest = this->get_closest_key(key);
		auto closest_dist = std::abs(int(closest) - int(key));
		//std::cout << "Testing: " << key << '\n';
		assert(closest_dist == *std::min_element(std::begin(dists), std::end(dists)));
	}


	// validate pred and succ invarient
	for (int key = 0; key < 256; ++key) {
		auto succ = this->successor(key);
		auto pred = this->predecessor(key);
		auto clos = this->get_closest_key(key);
		assert(!succ || (succ->key >= key));
		assert(!pred || (pred->key <= key));
	}
}

template <typename T>
void XFastTrie<T>::insert(T key) {
	if (this->lss.back().count(key)) return;
	
	if (false && !this->lss.back().empty()) {
		Node<T>* n = this->lss.back().at(this->get_closest_key(0));
		//std::cout << "Forwards LL: ";
		Node<T>* last = nullptr;
		while (n) {
			//std::cout << int(n->key) << ' ';
			last = n;
			n = n->right;
		}
		//std::cout << '\n';

		//assert(this->get_closest_key(255) == last->key);

		n = last;
		last = nullptr;
		//std::cout << "Backward LL: ";
		while (n) {
			//std::cout << int(n->key) << ' ';
			last = n;
			n = n->left;
		}
		//std::cout << '\n';

		//assert(this->get_closest_key(0) == last->key);
	}
	//std::cout << '\n';
	//this->validate();

	Node<T>* pred = this->predecessor(key);
	assert(!pred || pred->key <= key);
	Node<T>* succ = this->successor(key);
	assert(!succ || succ->key >= key);
	Node<T>* node = new Node<T>(key, pred, succ);

	this->lss.back()[key] = node;

	if (pred != nullptr) pred->right = node;
	if (succ != nullptr) succ->left = node;

	Node<T>* pre = node;
	for (int level = this->max_bits - 1; level >= 0; --level) {
		T prefix = this->prefix(key, level);
		if (this->lss[level].count(prefix)) {
			Node<T>* cur = this->lss[level].at(prefix);

			T left_child_prefix = prefix << 1;
			T right_child_prefix = (prefix << 1) + 1;

			if (this->lss[level + 1].count(left_child_prefix)) {
				cur->left = this->lss[level + 1].at(left_child_prefix);
			}
			else if (key < cur->left->key) {
				cur->left = node;
			}

			if (this->lss[level + 1].count(right_child_prefix)) {
				cur->right = this->lss[level + 1].at(right_child_prefix);
			}
			else if (key > cur->right->key) {
				cur->right = node;
			}
		}
		else {
			Node<T>* left_child = pre;
			Node<T>* right_child = pre;

			if ((pre->key & 1) == 0) {
				while (this->lss.back().count(right_child->key) ?
					this->lss.back().at(right_child->key) != right_child : true) {
					right_child = right_child->right;
				}
			}
			else {
				while (this->lss.back().count(left_child->key) ?
					this->lss.back().at(left_child->key) != left_child : true) {
					left_child = left_child->left;
				}
			}

			this->lss[level][prefix] = new Node<T>(prefix, left_child, right_child);
			pre = this->lss[level][prefix];
		}
	}
}