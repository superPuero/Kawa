#pragma once
#include <type_traits>
#include <tuple>

namespace ssr
{

    template<typename Tuple>
    struct tuple_rest {};

    template<typename T, typename...Rest>
    struct tuple_rest<std::tuple<T, Rest...>>
    {
        using type = std::tuple<Rest...>;
    };



    template<typename T, typename ListHead, typename...List>
    constexpr size_t variadic_index()
    {
        if constexpr (std::is_same<T, ListHead>::value)
            return 0;
        else
            return 1 + variadic_index<T, List...>();
    };

    template<typename T, typename...Args>
    constexpr bool is_present()
    {
        return (std::is_same_v<T, Args> || ...);
    };

    template<typename T, typename...Args>
    constexpr bool is_present_remove_ref()
    {
        return is_present<std::remove_reference_t<T>, std::remove_reference_t<Args>...>();
    };


    template<size_t at, size_t...I>
    constexpr auto slice_helper(std::index_sequence<I...>)
    {
        return std::index_sequence<(I + at)...>{};
    }

    template<size_t at, size_t...I>
    constexpr auto slice_index_seq(std::index_sequence<I...> seq)
    {
        return slice_helper<at>(std::make_index_sequence<seq.size() - at>{});
    }


    template<typename...Args>
    struct all_in
    {
        template<typename...TArgs>
        static constexpr bool value = (is_present<Args, TArgs...>() && ...);
    };

    template<typename...Args>
    struct all_in_remove_ref
    {
        template<typename...TArgs>
        static constexpr bool value = all_in<std::remove_reference_t<Args>...>::template value<std::remove_reference_t<TArgs>...>;
    };

    template<typename...Args>
    struct all_in<std::tuple<Args...>> : all_in<Args...> {};

    template<typename RetTy, typename...ArgTy>
    struct function_traits {};

    template<typename RetTy, typename...ArgTy>
    struct function_traits<RetTy(*)(ArgTy...)>
    {
        using return_type = typename RetTy;
        using args_type_tuple = typename std::tuple<std::remove_reference_t<ArgTy>...>;
    };

    template<typename RetTy, typename...ArgTy>
    struct function_traits<RetTy(&)(ArgTy...)>
    {
        using return_type = typename RetTy;
        using args_type_tuple = typename std::tuple<std::remove_reference_t<ArgTy>...>;
    };

    template<typename T>
    struct function_traits<T> : function_traits<decltype(&T::operator())> {};

    template<typename RetTy, typename ObjTy, typename...ArgTy>
    struct function_traits<RetTy(ObjTy::*)(ArgTy...) const>
    {
        using return_type = typename RetTy;
        using args_type_tuple = typename std::tuple<std::remove_reference_t<ArgTy>...>;
    };
}