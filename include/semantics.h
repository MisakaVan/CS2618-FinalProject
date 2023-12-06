//
// Created on 2023/12/7.
//

#ifndef FINALPROJECT_SEMANTICS_H
#define FINALPROJECT_SEMANTICS_H

// Implement of std::pair
template <typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;

    constexpr Pair(T1 first, T2 second) : first(first), second(second) {}
};

// implement std::move
template <typename T>
constexpr T &&move(T &arg) noexcept
{
    return static_cast<T &&>(arg);
}


// implement std::remove_reference
template <typename T>
struct remove_reference {
    using type = T;
};

// implement std::forward
template <typename T>
constexpr T &&forward(typename remove_reference<T>::type &arg) noexcept
{
    return static_cast<T &&>(arg);
}

#endif //FINALPROJECT_SEMANTICS_H
