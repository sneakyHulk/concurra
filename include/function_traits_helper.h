#pragma once

#include <cstdint>
#include <tuple>

template <class F>
struct function_traits : function_traits<decltype(&F::operator())> {};

// Free function type: R(Args...)
template <class R, class... Args>
struct function_traits<R(Args...)> {
	using return_type = R;
	using args_tuple = std::tuple<Args...>;
	static constexpr std::size_t arity = sizeof...(Args);
	template <std::size_t N>
	using arg = std::tuple_element_t<N, args_tuple>;
};

// Function pointer / reference
template <class R, class... Args>
struct function_traits<R (*)(Args...)> : function_traits<R(Args...)> {};
template <class R, class... Args>
struct function_traits<R (&)(Args...)> : function_traits<R(Args...)> {};

// Member function pointers (covers most lambdas: const operator())
template <class C, class R, class... Args>
struct function_traits<R (C::*)(Args...)> : function_traits<R(Args...)> {};
template <class C, class R, class... Args>
struct function_traits<R (C::*)(Args...) const> : function_traits<R(Args...)> {};