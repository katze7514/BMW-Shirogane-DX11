// C++17 removed items compatibility shim for old boost code
#pragma once
#include <functional>
#include <memory>

namespace std {
    // std::unary_function removed in C++17
    template<class Arg, class Result>
    struct unary_function {
        typedef Arg argument_type;
        typedef Result result_type;
    };
    // std::binary_function removed in C++17
    template<class Arg1, class Arg2, class Result>
    struct binary_function {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };
    // std::auto_ptr removed in C++17
    template<class T>
    class auto_ptr {
        T* p_;
    public:
        typedef T element_type;
        explicit auto_ptr(T* p = nullptr) noexcept : p_(p) {}
        auto_ptr(auto_ptr& r) noexcept : p_(r.release()) {}
        template<class U> auto_ptr(auto_ptr<U>& r) noexcept : p_(r.release()) {}
        ~auto_ptr() { delete p_; }
        auto_ptr& operator=(auto_ptr& r) noexcept { reset(r.release()); return *this; }
        T& operator*() const noexcept { return *p_; }
        T* operator->() const noexcept { return p_; }
        T* get() const noexcept { return p_; }
        T* release() noexcept { T* t = p_; p_ = nullptr; return t; }
        void reset(T* p = nullptr) noexcept { if (p != p_) { delete p_; p_ = p; } }
    };
    // std::bind2nd / std::mem_fun removed in C++17
    template<class Op, class T>
    auto bind2nd(Op op, const T& val) {
        return [op, val](auto&& x) { return op(std::forward<decltype(x)>(x), val); };
    }
    template<class Fn, class T>
    auto mem_fun(Fn (T::*fn)()) {
        return [fn](T* p) { return (p->*fn)(); };
    }
    template<class Fn, class T>
    auto mem_fun(Fn (T::*fn)() const) {
        return [fn](const T* p) { return (p->*fn)(); };
    }
}
using std::unary_function;
using std::binary_function;
