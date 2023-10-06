
#ifndef __array_h__
#define __array_h__

#include <stdexcept>
#include <algorithm>

namespace toy {

template <typename T, size_t len>
struct array {
        array() = default;

        array(const array& rhs) {
            std::copy(std::begin(rhs.data_), std::end(rhs.data_), data_);
        }

        array(T rhs[len]) {
            std::copy(rhs[0], rhs[len], data_);
        }

        array(array&& rhs) = delete;

        array(T value) {
            for (size_t i = 0; i < len; ++i) {
                data_[i] = value;
            }
        }

        array& operator=(const array& rhs) {
            if (this != rhs) {
                std::copy(std::begin(rhs.data_), std::end(rhs.data_), data_);
                return *this;
            }
        }
    
        array& operator=(T rhs[len]) {
            std::copy(rhs[0], rhs[len], data_);
        }
        
        array& operator=(array&& rhs) = delete;

        ~array() = default;

        T& operator[](size_t id) {
            return data_[id];
        }

        T& at(size_t id) {
            if (id < len) {
                return data_[id];
            } else {
                throw std::invalid_argument{ "array bounds violition" };
            }
        }

    private:
        T data_[len];
};

}  // namespace toy

#endif