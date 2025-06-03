#pragma once

namespace kawa
{
	namespace meta
	{
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
}