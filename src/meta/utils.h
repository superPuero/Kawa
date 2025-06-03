#pragma once
namespace kawa
{
	namespace meta
	{
		template<typename tuple, size_t Offset, size_t...I>
		constexpr auto slice_tuple_impl(std::index_sequence<I...>) -> std::tuple<std::tuple_element_t<Offset + I, tuple>...> {};

		template<typename tuple>
		struct slice_tuple
		{
			template<size_t I>
			using at = decltype(slice_tuple_impl<tuple, I>(std::make_index_sequence<std::tuple_size_v<tuple> - I>{}));
		};

		template<typename tuple>
		struct rev_slice_tuple
		{
			template<size_t I>
			using at = decltype(slice_tuple_impl<tuple, 0>(std::make_index_sequence<std::tuple_size_v<tuple> - I>{}));
		};
	}
}