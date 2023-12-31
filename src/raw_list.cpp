
module;

#include <memory>

export module toy_std.raw_list;

import toy_std.iterator_traits;

namespace toy {

struct list_node_base {
        void swap(list_node_base& x, list_node_base& y) noexcept {
            if (x.next_ != &x) {
                if (y.next_ != &y) {
                    // Both x and y are not empty.
                    std::swap(x.next_, y.next_);
                    std::swap(x.prev_, y.prev_);
                    x.next_->prev_ = x.prev_->next_ = &x;
                    y.next_->prev_ = y.prev_->next_ = &y;
                } else {
                    // x is not empty, y is empty.
                    y.next_ = x.next_;
                    y.prev_ = x.prev_;
                    y.next_->prev_ = y.prev_->next_ = &y;
                    x.next_ = x.prev_ = &x;
                }
            } else if (y.next_ != &y) {
                // x is empty, y is not empty.
                x.next_ = y.next_;
                x.prev_ = y.prev_;
                x.next_->prev_ = x.prev_->next_ = &x;
                y.next_ = y.prev_ = &y;
            }
        }

        void transfer(list_node_base* const first, list_node_base* const last) {
            // assert(first != last);
            if (this != last) {
                // Remove [first, last) from its old position.
                last->prev_->next_ = this;
                first->prev_->next_ = last;
                this->prev_->next_ = first;

                // Splice [first, last) into its new position.
                list_node_base* const tmp = this->prev_;
                this->prev_ = last->prev_;
                last->prev_ = first->prev_;
                first->prev_ = tmp;
            }
        }

        void reverse() {
            list_node_base* tmp = this;
            do {
                std::swap(tmp->next_, tmp->prev_);

                // Old next node is now prev.
                tmp = tmp->prev_;
            } while (tmp != this);
        }

        void hook(list_node_base* const position) {
            this->next_ = position;
            this->prev_ = position->prev_;
            position->prev_->next_ = this;
            position->prev_ = this;
        }

        void unhook() {
            list_node_base* const next_node = this->next_;
            list_node_base* const prev_node = this->prev_;
            prev_node->next_ = next_node;
            next_node->prev_ = prev_node;
        }

        list_node_base* next_;
        list_node_base* prev_;
};

struct list_node_header : public list_node_base {
        list_node_header() {
            init();
        }

        void init() noexcept {
            this->next_ = this->prev_ = this;
        }

        void size_inc(size_t value) {
            size_ += value;
        }

        void size_dec(size_t value) {
            size_ -= value;
        }

        size_t size_{};
};

template <typename T>
struct list_node : public list_node_base {
        list_node() = default;

        list_node(const T& data) : data_{ data } {
        }

        T* valptr() {
            return std::addressof(data_);
        }

        const T* valptr() const {
            return std::addressof(data_);
        }

    private:
        T data_;
};

template <typename T>
struct list_iterator {
        using self = list_iterator<T>;
        using node_type = list_node<T>;

        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = toy::bidirectional_iterator_tag;

        list_iterator() = default;

        explicit list_iterator(list_node_base* ptr) : node_ptr_{ ptr } {};

        reference operator*() const {
            return *static_cast<node_type*>(node_ptr_)->valptr();
        };

        pointer operator->() const {
            return std::addressof(static_cast<node_type*>(node_ptr_)->valptr());
        }

        // pre-increment
        self& operator++() {
            node_ptr_ = node_ptr_->next_;
            return *this;
        }

        // post-increment
        self operator++(int) {
            auto tmp{ *this };
            ++(*this);
            return tmp;
        }

        // pre-decrement
        self& operator--() {
            node_ptr_ = node_ptr_->prev_;
            return *this;
        }

        // post-decrement
        self operator--(int) {
            auto tmp{ *this };
            --(*this);
            return tmp;
        }

        friend bool operator==(const self& a, const self& b) {
            return a.node_ptr_ == b.node_ptr_;
        };

        friend bool operator!=(const self& a, const self& b) {
            return !(a == b);
        };

        list_node_base* node_ptr_{};
};

template <typename T>
struct list_const_iterator {
        using self = list_const_iterator<T>;
        using node_type = const list_node<T>;
        using iterator = list_iterator<T>;

        using value_type = T;
        using reference = const T&;
        using pointer = const T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = toy::bidirectional_iterator_tag;

        list_const_iterator() = default;

        explicit list_const_iterator(list_node_base* ptr) : node_ptr_{ ptr } {};

        explicit list_const_iterator(const iterator& x) : node_ptr_{ x.node_ptr_ } {};

        reference operator*() const {
            return *static_cast<node_type*>(node_ptr_)->valptr();
        };

        pointer operator->() const {
            return std::addressof(static_cast<node_type*>(node_ptr_)->valptr());
        }

        // pre-increment
        self& operator++() {
            node_ptr_ = node_ptr_->next_;
            return *this;
        }

        // post-increment
        self operator++(int) {
            auto tmp{ *this };
            ++(*this);
            return tmp;
        }

        // pre-decrement
        self& operator--() {
            node_ptr_ = node_ptr_->prev_;
            return *this;
        }

        // post-decrement
        self operator--(int) {
            auto tmp{ *this };
            --(*this);
            return tmp;
        }

        friend bool operator==(const self& a, const self& b) {
            return a.node_ptr_ == b.node_ptr_;
        };

        friend bool operator!=(const self& a, const self& b) {
            return !(a == b);
        };

        const list_node_base* node_ptr_{};
};

export template <typename T>
struct raw_list {
        using iterator = list_iterator<T>;
        using const_iterator = list_const_iterator<T>;

        using value_type = T;
        using reference = T&;
        using const_reference = const T&;

        using node_type = list_node<T>;

        raw_list() = default;

        raw_list(std::initializer_list<value_type> rhs) {
            for (auto&& elem : rhs) {
                push_back(elem);
            }
        }

        raw_list(const raw_list& rhs) = delete;
        raw_list(raw_list&& rhs) = delete;

        raw_list& operator=(const raw_list& rhs) = delete;
        raw_list& operator=(raw_list&& rhs) = delete;

        // OMG!!!
        ~raw_list() {
            clear();
        }

        [[nodiscard]] iterator begin() {
            return iterator{ head_.next_ };
        }

        [[nodiscard]] iterator end() {
            return iterator{ &head_ };
        }

        [[nodiscard]] iterator cbegin() {
            return const_iterator{ head_.next_ };
        }

        [[nodiscard]] iterator cend() {
            return const_iterator{ &head_ };
        }

        void insert(iterator position, const value_type& rhs) {
            auto* next = new node_type{ rhs };
            next->hook(position.node_ptr_);
            head_.size_inc(1);
        }

        void erase(iterator position) {
            head_.size_dec(1);
            position.node_ptr_->unhook();
            auto* n = static_cast<node_type*>(position.node_ptr_);
            // delete n->valptr();
            delete n;
        }

        [[nodiscard]] reference front() {
            return *begin();
        }

        [[nodiscard]] reference back() {
            iterator tmp = end();
            --tmp;
            return *tmp;
        }

        [[nodiscard]] const_reference front() const {
            return *begin();
        }

        [[nodiscard]] const_reference back() const {
            iterator tmp = end();
            --tmp;
            return *tmp;
        }

        void push_back(const value_type& rhs) {
            insert(this->end(), rhs);
        }

        void push_front(const value_type& rhs) {
            insert(this->begin(), rhs);
        }

        void pop_back() {
            erase(iterator{ head_.prev_ });
        }

        void pop_front() {
            erase(begin());
        }

        [[nodiscard]] bool empty() const noexcept {
            return (head_.next_ == &head_);
        }

        [[nodiscard]] size_t size() const noexcept {
            return head_.size_;
        }

    private:
        void clear() {
            list_node_base* cur = this->head_.next_;
            while (cur != &this->head_) {
                auto* tmp = static_cast<node_type*>(cur);
                cur = tmp->next_;
                // std::_Destroy(&__tmp->_M_data);
                delete tmp;
            }
        }

        list_node_header head_;
};

}  // namespace toy
