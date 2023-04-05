#include <span>

namespace engine {

template<class T>
auto singular_span(T& t) -> std::span<T, 1> {
    return std::span<T, 1> {std::addressof(t), 1};
}

}  // namespace engine