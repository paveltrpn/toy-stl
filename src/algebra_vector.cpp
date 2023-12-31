
module;

#include <cstddef>

export module toy_std.algebra:vector;

import toy_std.concepts;
import toy_std.array;

export {
    namespace toy::algebra {

    template <toy::Arithmetical T, size_t size>
    struct vector;

    template <toy::Arithmetical T>
    struct vector<T, 2> {
            vector() = default;

            ~vector() = default;

        private:
            toy::array<T, 2> data_;
    };

    template <toy::Arithmetical T>
    struct vector<T, 3> {
            vector() = default;

            ~vector() = default;

        private:
            toy::array<T, 3> data_;
    };

    template <toy::Arithmetical T>
    struct vector<T, 4> {
            vector() = default;

            ~vector() = default;

        private:
            toy::array<T, 4> data_;
    };

    template <typename T>
    using vector2 = vector<T, 2>;

    template <typename T>
    using vector3 = vector<T, 3>;

    template <typename T>
    using vector4 = vector<T, 4>;

    }  // namespace toy::math
}
