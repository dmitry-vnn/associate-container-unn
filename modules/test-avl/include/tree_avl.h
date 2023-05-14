#pragma once

#include <utility>
#include <string>
#include <algorithm>
#include <memory>
#include <iterator>
#include <stdexcept>

namespace avl {

	template <typename T, typename A = std::allocator<T>>
	class Tree {

	public:
		using AllocatorType = A;

		typedef typename A::value_type value_type;
		typedef typename A::reference reference;
		typedef typename A::const_reference const_reference;
		typedef typename A::difference_type difference_type;
		typedef typename A::size_type size_type;

	private:
		class Node {
		public:
			T data;

			short depth;
			size_type n;

			Node* parent;
			Node* leftChild;
			Node* rightChild;

			Node() noexcept
				: depth(1)
				, n(1)
				, parent(nullptr)
				, leftChild(nullptr)
				, rightChild(nullptr) {}

			explicit Node(const T& t) noexcept
				: data(t)
				, depth(1)
				, n(1)
				, parent(nullptr)
				, leftChild(nullptr)
				, rightChild(nullptr) {}

			explicit Node(T&& t) noexcept
				: data(std::move(t))
				, depth(1)
				, n(1)
				, parent(nullptr)
				, leftChild(nullptr)
				, rightChild(nullptr) {}


			void UpdateDepth() {
				depth = 1 + std::max(leftChild ? leftChild->depth : 0,
					rightChild ? rightChild->depth : 0);
			}

			void UpdateN() {
				n = 1 + (leftChild ? leftChild->n : 0)
					+ (rightChild ? rightChild->n : 0);
			}

			short Imbalance() {
				return (rightChild ? rightChild->depth : 0)
					- (leftChild ? leftChild->depth : 0);
			}
		};

	public:
		class Iterator {

			//template <typename U, typename V>
			//friend class tree::const_iterator;

			friend class Tree;
		public:
			typedef typename A::difference_type difference_type;
			typedef typename A::value_type value_type;
			typedef typename A::reference reference;
			typedef typename A::pointer pointer;
			typedef std::bidirectional_iterator_tag iterator_category;

			Iterator() {
				_nodeCursor = nullptr;
			}

			explicit Iterator(Node* p) {
				_nodeCursor = p;
			}

			Iterator(const Iterator& it) {
				_nodeCursor = it._nodeCursor;
			}

			Iterator& operator=(const Iterator& it) {
				_nodeCursor = it._nodeCursor;
				return *this;
			}

			bool operator==(const Iterator& it) const {
				return _nodeCursor == it._nodeCursor;
			}

			bool operator!=(const Iterator& it) const {
				return _nodeCursor != it._nodeCursor;
			}

			bool operator<(const Iterator& it) const {
				return **this < *it;
			}

			bool operator>(const Iterator& it) const {
				return **this > *it;
			}

			bool operator<=(const Iterator& it) const {
				return **this <= *it;
			}

			bool operator>=(const Iterator& it) const {
				return **this >= *it;
			}

			// pre-increment
			Iterator& operator++() {
				if (_nodeCursor->rightChild) {
					_nodeCursor = _nodeCursor->rightChild;
					while (_nodeCursor->leftChild) {
						_nodeCursor = _nodeCursor->leftChild;
					}
				}
				else {
					Node* before;
					do {
						before = _nodeCursor;
						_nodeCursor = _nodeCursor->parent;
					} while (_nodeCursor && before == _nodeCursor->rightChild);
				}
				return *this;
			}

			// post-increment
			Iterator operator++(int) {
				Iterator old(*this);
				++(*this);
				return old;
			}

			// pre-decrement
			Iterator& operator--() {
				if (_nodeCursor->leftChild) {
					_nodeCursor = _nodeCursor->leftChild;
					while (_nodeCursor->rightChild) {
						_nodeCursor = _nodeCursor->rightChild;
					}
				}
				else {
					Node* before;
					do {
						before = _nodeCursor;
						_nodeCursor = _nodeCursor->parent;
					} while (_nodeCursor && before == _nodeCursor->leftChild);
				}
				return *this;
			}

			// post-decrement
			Iterator operator--(int) {
				Iterator old(*this);
				--*this;
				return old;
			}

			reference operator*() const {
				return _nodeCursor->data;
			}

			pointer operator->() const {
				return &_nodeCursor->data;
			}
		private:
			Node* _nodeCursor;
		};

		class ConstIterator {
		public:
			typedef typename A::difference_type difference_type;
			typedef typename A::value_type value_type;
			typedef typename A::const_reference const_reference;
			typedef typename A::const_pointer const_pointer;
			typedef std::bidirectional_iterator_tag iterator_category;

			ConstIterator() {
				_nodeCursor = nullptr;
			}

			ConstIterator(const Node* p) {
				_nodeCursor = p;
			}

			ConstIterator(const ConstIterator& it) {
				_nodeCursor = it._nodeCursor;
			}

			ConstIterator(const Iterator& it) {
				_nodeCursor = it._nodeCursor;
			}

			ConstIterator& operator=(const ConstIterator& it) {
				_nodeCursor = it._nodeCursor;
				return *this;
			}

			bool operator==(const ConstIterator& it) const {
				return _nodeCursor == it._nodeCursor;
			}

			bool operator!=(const ConstIterator& it) const {
				return _nodeCursor != it._nodeCursor;
			}

			bool operator<(const ConstIterator& it) const {
				return **this < *it;
			}

			bool operator>(const ConstIterator& it) const {
				return **this > *it;
			}

			bool operator<=(const ConstIterator& it) const {
				return **this <= *it;
			}

			bool operator>=(const ConstIterator& it) const {
				return **this >= *it;
			}

			// pre-increment
			ConstIterator& operator++() {
				if (_nodeCursor->rightChild) {
					_nodeCursor = _nodeCursor->rightChild;
					while (_nodeCursor->leftChild) {
						_nodeCursor = _nodeCursor->leftChild;
					}
				}
				else {
					const Node* before;
					do {
						before = _nodeCursor;
						_nodeCursor = _nodeCursor->parent;
					} while (_nodeCursor && before == _nodeCursor->rightChild);
				}
				return *this;
			}

			// post-increment
			ConstIterator operator++(int) {
				ConstIterator old(*this);
				++(*this);
				return old;
			}

			// pre-decrement
			ConstIterator& operator--() {
				if (_nodeCursor->leftChild) {
					_nodeCursor = _nodeCursor->leftChild;
					while (_nodeCursor->rightChild) {
						_nodeCursor = _nodeCursor->rightChild;
					}
				}
				else {
					const Node* before;
					do {
						before = _nodeCursor;
						_nodeCursor = _nodeCursor->parent;
					} while (_nodeCursor && before == _nodeCursor->leftChild);
				}
				return *this;
			}

			// post-decrement
			ConstIterator operator--(int) {
				ConstIterator old(*this);
				--(*this);
				return old;
			}

			const_reference operator*() const {
				return static_cast<const_reference>(_nodeCursor->data);
			}

			const_pointer operator->() const {
				return &(_nodeCursor->data);
			}
		private:
			Node const* _nodeCursor;
		};

		Tree() noexcept {
			root = alloc.allocate(1);
			alloc.construct(root);
			root->n = 0;
		}

		Tree(const Tree& t) noexcept {
			*this = t;
		}

		Tree(Tree&& t) noexcept {
			root = t.root;
			t.root = alloc.allocate(1);
			alloc.construct(t.root);
			t.root->n = 0;
		}

		~Tree() noexcept {
			clear_node(root);
			alloc.destroy(root);
			alloc.deallocate(root, 1);
		}

		Tree& operator=(const Tree& t) noexcept {
			root = deep_copy_node(t.root);
			return *this;
		}

		Tree& operator=(Tree&& t) noexcept {
			Clear();
			std::swap(root, t.root);
		}

		bool operator==(const Tree& t) const {
			ConstIterator it1, it2;
			for (it1 = cbegin(), it2 = t.cbegin();
				it1 != cend() && it2 != t.cend();
				++it1, ++it2) {
				if (*it1 != *it2)
					return false;
			}
			if (it1 == cend() && it2 == t.cend()) {
				return true;
			}
			else {
				return false;
			}
		}

		bool operator!=(const Tree& t) const {
			return !(*this == t);
		}

		Iterator begin() {
			Node* ptr = root;
			while (ptr->leftChild) {
				ptr = ptr->leftChild;
			}
			return Iterator(ptr);
		}

		ConstIterator begin() const {
			const Node* ptr = root;
			while (ptr->leftChild) {
				ptr = ptr->leftChild;
			}
			return ConstIterator(ptr);
		}

		ConstIterator cbegin() const {
			const Node* ptr = root;
			while (ptr->leftChild) {
				ptr = ptr->leftChild;
			}
			return ConstIterator(ptr);
		}

		Iterator end() {
			return Iterator(root);
		}

		ConstIterator end() const {
			return ConstIterator(root);
		}

		ConstIterator cend() const {
			return ConstIterator(root);
		}

		reference front() {
			Iterator b = begin();
			return *b;
		}

		const_reference front() const {
			ConstIterator b = begin();
			return *b;
		}

		reference back() {
			Iterator b = end();
			return *(--b);
		}

		const_reference back() const {
			ConstIterator b = end();
			return *(--b);
		}

		Iterator insert(const T& t) {
			Iterator res;

			// descent the search tree
			Node* parent = root;
			while (true) {
				++parent->n;
				if (parent == root || t < parent->data) {
					if (parent->leftChild) {
						parent = parent->leftChild;
					}
					else {
						parent->leftChild = alloc.allocate(1);
						alloc.construct(parent->leftChild, t);
						parent->leftChild->parent = parent;
						res = Iterator(parent->leftChild);
						break;
					}
				}
				else {
					if (parent->rightChild) {
						parent = parent->rightChild;
					}
					else {
						parent->rightChild = alloc.allocate(1);
						alloc.construct(parent->rightChild, t);
						parent->rightChild->parent = parent;
						res = Iterator(parent->rightChild);
						break;
					}
				}
			}
			short branch_depth = 1;
			do {
				if (parent->depth > branch_depth)
					break;
				parent->depth = 1 + branch_depth;
				if (parent == root)
					break;
				if (parent->Imbalance() < -1) {
					// check for double-rotation case
					if (parent->leftChild->Imbalance() > 0) {
						rotate_left(parent->leftChild);
					}
					rotate_right(parent);
					break;
				}
				else if (parent->Imbalance() > 1) {
					// check for double-rotation case
					if (parent->rightChild->Imbalance() < 0) {
						rotate_right(parent->rightChild);
					}
					rotate_left(parent);
					break;
				}
				branch_depth = parent->depth;
				parent = parent->parent;
			} while (parent);
			return res;
		}

		Iterator insert(T&& t) {
			Iterator res;

			// descent the search tree
			Node* parent = root;
			while (true) {
				++parent->n;
				if (parent == root || t < parent->data) {
					if (parent->leftChild) {
						parent = parent->leftChild;
					}
					else {
						parent->leftChild = alloc.allocate(1);
						alloc.construct(parent->leftChild, std::move(t));
						parent->leftChild->parent = parent;
						res = Iterator(parent->leftChild);
						break;
					}
				}
				else {
					if (parent->rightChild) {
						parent = parent->rightChild;
					}
					else {
						parent->rightChild = alloc.allocate(1);
						alloc.construct(parent->rightChild, std::move(t));
						parent->rightChild->parent = parent;
						res = Iterator(parent->rightChild);
						break;
					}
				}
			}
			short branch_depth = 1;
			do {
				if (parent->depth > branch_depth)
					break;
				parent->depth = 1 + branch_depth;
				if (parent == root)
					break;
				if (parent->Imbalance() < -1) {
					// check for double-rotation case
					if (parent->leftChild->Imbalance() > 0) {
						rotate_left(parent->leftChild);
					}
					rotate_right(parent);
					break;
				}
				else if (parent->Imbalance() > 1) {
					// check for double-rotation case
					if (parent->rightChild->Imbalance() < 0) {
						rotate_right(parent->rightChild);
					}
					rotate_left(parent);
					break;
				}
				branch_depth = parent->depth;
				parent = parent->parent;
			} while (parent);
			return res;
		}

		Iterator at(size_type i) {
			// bounds checking
			if (i >= Size()) {
				throw std::out_of_range("tree::at out-of-range");
			}

			size_type j = i;
			Node* ptr = root->leftChild;
			while (true) {
				if (ptr->leftChild) {
					if (j == ptr->leftChild->n) {
						break;
					}
					else if (j < ptr->leftChild->n) {
						ptr = ptr->leftChild;
					}
					else {
						j -= 1 + ptr->leftChild->n;
						ptr = ptr->rightChild;
					}
				}
				else {
					if (j == 0) {
						break;
					}
					else {
						--j;
						ptr = ptr->rightChild;
					}
				}
			}
			return Iterator(ptr);
		}

		ConstIterator at(size_type i) const {
			// bounds checking
			if (i >= Size()) {
				throw std::out_of_range("tree[] out-of-range");
			}

			size_type j = i;
			const Node* ptr = root->leftChild;
			while (true) {
				if (ptr->leftChild) {
					if (j == ptr->leftChild->n) {
						break;
					}
					else if (j < ptr->leftChild->n) {
						ptr = ptr->leftChild;
					}
					else {
						j -= 1 + ptr->leftChild->n;
						ptr = ptr->rightChild;
					}
				}
				else {
					if (j == 0) {
						break;
					}
					else {
						--j;
						ptr = ptr->rightChild;
					}
				}
			}
			return ConstIterator(ptr);
		}

		reference operator[](size_type i) {
			return *at(i);
		}

		const_reference operator[](size_type i) const {
			return *at(i);
		}

		Iterator erase(Iterator it) {
			Iterator itn(it);
			++itn;
			Node* ptr = it._nodeCursor;
			Node* q;
			if (!ptr->leftChild || !ptr->rightChild) {
				q = ptr;
			}
			else {
				q = itn._nodeCursor;
			}
			Node* s;
			if (q->leftChild) {
				s = q->leftChild;
				q->leftChild = nullptr;
			}
			else {
				s = q->rightChild;
				q->rightChild = nullptr;
			}
			if (s) {
				s->parent = q->parent;
			}
			if (q == q->parent->leftChild) {
				q->parent->leftChild = s;
			}
			else {
				q->parent->rightChild = s;
			}
			Node* q_parent = q->parent;
			if (q != ptr) {
				q->parent = ptr->parent;
				if (q->parent->leftChild == ptr) {
					q->parent->leftChild = q;
				}
				else {
					q->parent->rightChild = q;
				}
				q->leftChild = ptr->leftChild;
				if (q->leftChild)
					q->leftChild->parent = q;
				q->rightChild = ptr->rightChild;
				if (q->rightChild)
					q->rightChild->parent = q;
				q->n = ptr->n;
				q->depth = ptr->depth;
				ptr->leftChild = nullptr;
				ptr->rightChild = nullptr;
			}
			if (q_parent == ptr) {
				q_parent = q;
			}
			Node* parent;
			for (parent = q_parent; parent; parent = parent->parent) {
				--parent->n;
			}
			for (parent = q_parent; parent; parent = parent->parent) {
				parent->UpdateDepth();
				if (parent == root)
					break;
				if (parent->Imbalance() < -1) {
					// check for double-rotation case
					if (parent->leftChild->Imbalance() > 0) {
						rotate_left(parent->leftChild);
					}
					rotate_right(parent);
					break;
				}
				else if (parent->Imbalance() > 1) {
					// check for double-rotation case
					if (parent->rightChild->Imbalance() < 0) {
						rotate_right(parent->rightChild);
					}
					rotate_left(parent);
					break;
				}
			}
			alloc.destroy(ptr);
			alloc.deallocate(ptr, 1);
			return itn;
		}

		Iterator find(const_reference t) {
			Node* ptr = root->leftChild;
			while (ptr) {
				if (t == ptr->data) {
					return Iterator(ptr);
				}
				else if (t < ptr->data) {
					ptr = ptr->leftChild;
				}
				else {
					ptr = ptr->rightChild;
				}
			}
			return end();
		}

		void Remove(const_reference t) {
			Iterator it = find(t);
			if (it == end())
				return;
			do {
				it = erase(it);
			} while (*it == t);
		}

		void Clear() noexcept {
			clear_node(root);
			root->leftChild = nullptr;
			root->n = 0;
			root->depth = 1;
		}

		void Swap(Tree& t) {
			std::swap(root, t.root);
		}

		size_type Size() const {
			return root->n;
		}

		size_type MaxSize();

		bool Empty() const {
			return root->leftChild == nullptr;
		}

		A get_allocator() {
			return alloc;
		}

	private:
		void rotate_left(Node* n) {
			Node* tmp = n->rightChild->leftChild;
			if (n == n->parent->leftChild) {
				n->parent->leftChild = n->rightChild;
			}
			else {
				n->parent->rightChild = n->rightChild;
			}
			n->rightChild->parent = n->parent;
			n->rightChild->leftChild = n;
			n->parent = n->rightChild;
			n->rightChild = tmp;
			if (tmp)
				tmp->parent = n;

			// update ns
			n->UpdateN();
			n->parent->UpdateN();

			// update depths
			do {
				n->UpdateDepth();
				n = n->parent;
			} while (n);
		}

		void rotate_right(Node* n) {
			Node* tmp = n->leftChild->rightChild;
			if (n == n->parent->leftChild) {
				n->parent->leftChild = n->leftChild;
			}
			else {
				n->parent->rightChild = n->leftChild;
			}
			n->leftChild->parent = n->parent;
			n->leftChild->rightChild = n;
			n->parent = n->leftChild;
			n->leftChild = tmp;
			if (tmp)
				tmp->parent = n;

			// update ns
			n->UpdateN();
			n->parent->UpdateN();

			// update depths
			do {
				n->UpdateDepth();
				n = n->parent;
			} while (n);
		}

		Node* deep_copy_node(const Node* nd) {
			Node* cp_nd = alloc.allocate(1);
			alloc.construct(cp_nd, nd->data);
			cp_nd->n = nd->n;
			cp_nd->depth = nd->depth;
			if (nd->leftChild) {
				cp_nd->leftChild = deep_copy_node(nd->leftChild);
				cp_nd->leftChild->parent = cp_nd;
			}
			if (nd->rightChild) {
				cp_nd->rightChild = deep_copy_node(nd->rightChild);
				cp_nd->rightChild->parent = cp_nd;
			}
			return cp_nd;
		}

		void clear_node(Node* nd) noexcept {
			if (nd->leftChild) {
				clear_node(nd->leftChild);
				alloc.destroy(nd->leftChild);
				alloc.deallocate(nd->leftChild, 1);
			}
			if (nd->rightChild) {
				clear_node(nd->rightChild);
				alloc.destroy(nd->rightChild);
				alloc.deallocate(nd->rightChild, 1);
			}
		}

		using NodeAlloc = typename std::allocator_traits<A>
			::template rebind_alloc<Node>;
		NodeAlloc alloc;
		Node* root;

	};

	template <typename T, typename A = std::allocator<T> >
	void Swap(Tree<T, A>& t1, Tree<T, A>& t2) {
		t1.swap(t2);
	}

}