/****************************************************************************
**
** This file is part of the KDAlgorithms
**
** SPDX-FileCopyrightText: 2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#if __cplusplus >= 202002L

#include "../src/kdalgorithms.h"
#include <list>
#include <string>
#include <vector>

using namespace std;

// clang-format off
#define FAIL_2_ARG(name, function, arg1, arg2) \
template <typename T, typename S>\
    void err_##name() { \
        static_assert(!requires(T t, S s){kdalgorithms::function(t, s);});\
}\
\
void test_##name() \
{\
        err_##name<arg1, arg2>();\
}

#define FAIL_3_ARG(name, function, arg1, arg2, arg3) \
template <typename T, typename S, typename U>\
    void err_##name() { \
        static_assert(!requires(T t, S s, U u){kdalgorithms::function(t, s, u);});\
}\
\
void test_##name() \
{\
        err_##name<arg1, arg2, arg3>();\
}
// clang-format on

FAIL_2_ARG(copy, copy, vector<int>, vector<string>)
FAIL_2_ARG(filtered, filtered, vector<int>, function<string(string)>)
FAIL_2_ARG(filtered2, filtered<list>, vector<int>, function<string(string)>)
FAIL_2_ARG(filter, filter, vector<int>, function<string(string)>)
FAIL_2_ARG(contains, contains, vector<int>, string)
FAIL_2_ARG(count, count, vector<int>, string)
FAIL_2_ARG(get_first_match, get_match, vector<int>, function<bool(string)>)
FAIL_2_ARG(get_first_match_or_default, get_match_or_default, vector<int>, function<bool(string)>)
FAIL_2_ARG(count_if_wong_type, count_if, vector<int>, std::function<bool(string)>)
FAIL_2_ARG(count_if_no_return, count_if, vector<int>, std::function<void(int)>)
FAIL_2_ARG(max_element_wrong_predicate, max_element, vector<int>, function<bool(string, string)>)
FAIL_2_ARG(max_element_no_return_value, max_element, vector<int>, function<void(int, int)>)
FAIL_2_ARG(max_element_only_one_arg, max_element, vector<int>, function<bool(int)>)
FAIL_2_ARG(max_value_less_than_wrong_needle, max_value_less_than, vector<int>, string)
FAIL_3_ARG(max_value_less_than_wrong_predicate, max_value_less_than, vector<int>, int,
           function<bool(string, string)>)
FAIL_3_ARG(max_value_less_than_returns_nothing, max_value_less_than, vector<int>, int,
           function<void(int, int)>)
FAIL_3_ARG(max_value_less_than_one_arg, max_value_less_than, vector<int>, int, function<bool(int)>)
FAIL_2_ARG(is_permutation, is_permutation, vector<int>, vector<string>)
FAIL_2_ARG(accumulate_wrong_arg_count, accumulate, vector<int>, function<int(int)>)
FAIL_2_ARG(accumulate_wrong_return_type, accumulate, vector<int>, function<string(int, int)>)
FAIL_3_ARG(accumulate_if_accumulate_fn_return_type, accumulate_if, vector<int>,
           function<string(int, int)>, function<int(int)>)
FAIL_3_ARG(accumulate_if_accumulate_fn_arg, accumulate_if, vector<int>, function<int(string, int)>,
           function<int(int)>)
FAIL_3_ARG(accumulate_if_predicate, accumulate_if, vector<int>, function<int(int, int)>,
           function<string(int)>)
FAIL_3_ARG(generate_n_wrong_generator_input_type, generate_n, vector<string>, int,
           function<int(int)>)
FAIL_3_ARG(generate_n_wrong_generator_return_type, generate_n, vector<string>, int,
           function<string(string)>)
FAIL_3_ARG(transformed_if_wrong_predicate, transformed_if, vector<int>, std::function<string(int)>,
           function<bool(string)>)
FAIL_3_ARG(transformed_if_wrong_transform, transformed_if, vector<int>,
           std::function<string(int, int)>, function<bool(int)>)
FAIL_2_ARG(transform_wrong_predicate, transform, vector<int>, std::function<bool(int, int)>)
FAIL_2_ARG(transformed_wrong_predicate, transformed, vector<int>, std::function<string()>)

template <typename ResultContainer, typename Container, typename UnaryPredicate>
void err_partitioned()
{
    static_assert(!requires(Container container, UnaryPredicate pred) {
                       kdalgorithms::partitioned<ResultContainer>(container, pred);
                   });
}

template <typename Container, typename Generator>

void err_generate()
{
    static_assert(
        !requires(Generator generator) { kdalgorithms::generate_until<Container>(generator); });
}

template <template <typename...> class ResultContainer, typename InputContainer, typename Transform,
          typename UnaryPredicate>
void err_transformed_if1()
{
    static_assert(!requires(InputContainer inputContainer, Transform transform,
                            UnaryPredicate unaryPredicate) {
                       kdalgorithms::transformed_if<ResultContainer>(inputContainer, transform,
                                                                     unaryPredicate);
                   });
}

template <typename ResultContainer, typename InputContainer, typename Transform,
          typename UnaryPredicate>
void err_transformed_if2()
{
    static_assert(!requires(InputContainer inputContainer, Transform transform,
                            UnaryPredicate unaryPredicate) {
                       kdalgorithms::transformed_if<ResultContainer>(inputContainer, transform,
                                                                     unaryPredicate);
                   });
}

template <template <typename...> class ResultContainer, typename InputContainer, typename Transform>
void err_transformed1()
{
    static_assert(!requires(InputContainer inputContainer, Transform transform) {
                       kdalgorithms::transformed<ResultContainer>(inputContainer, transform);
                   });
}

template <typename ResultContainer, typename InputContainer, typename Transform>
void err_transformed2()
{
    static_assert(!requires(InputContainer inputContainer, Transform transform) {
                       kdalgorithms::transformed<ResultContainer>(inputContainer, transform);
                   });
}

void test_constraints()
{
    // wrong type of items of result vector
    err_partitioned<vector<string>, vector<int>, function<bool(int)>>();

    // wrong predicate argument type
    err_partitioned<vector<int>, vector<int>, function<bool(string)>>();

    // wrong type in the optional
    err_generate<vector<int>, function<optional<string>()>>();

    // Forgot the std::optional in the generator
    err_generate<vector<int>, function<int()>>();

    // Generator shouldn't take any parameter
    err_generate<vector<int>, function<optional<int>(int)>>();

    // wrong Transform
    err_transformed_if1<list, vector<int>, std::function<char(string)>, std::function<bool(int)>>();
    err_transformed_if2<vector<string>, vector<int>, std::function<char(int)>,
                        std::function<bool(int)>>();
    err_transformed1<list, vector<int>, std::function<char(string)>>();
    err_transformed2<vector<string>, vector<int>, std::function<string()>>();

    // wrong predicate
    err_transformed_if1<list, vector<int>, std::function<string(int)>,
                        std::function<string(int)>>();
    err_transformed_if1<list, vector<int>, std::function<string(int)>,
                        std::function<bool(string)>>();
    err_transformed_if2<vector<string>, vector<int>, std::function<string(int)>,
                        std::function<string(int)>>();
    err_transformed_if2<vector<string>, vector<int>, std::function<string(int)>,
                        std::function<bool(string)>>();
}

#endif // C++ 20
