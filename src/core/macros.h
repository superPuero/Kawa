#pragma once
#include <string>

#define TO_STRING(x) #x

#define BIT(x) (1 << x)

#define PARENS ()

#define EXPAND(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) __VA_ARGS__

#define FOR_EACH(macro, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))


#define FOR_EACH_HELPER(macro, v1, ...) \
macro(v1)\
__VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))\

#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define ENUM_STR_CASE(val)	\
case val: 					\
	return #val;			\


#define BETTER_ENUM(name, type, ...)					\
enum name : type										\
{														\
	__VA_ARGS__											\
};														\
														\
														\
inline static std::string to_string(const enum name& val)		\
{														\
	switch (val)										\
	{													\
	FOR_EACH(ENUM_STR_CASE, __VA_ARGS__)				\
	default:											\
		break;											\
	}													\
}														\
														\
	
