/* MIT License

Copyright (C) 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#include <functional>

namespace kdalgorithms {
namespace detail {
    template <typename Function, typename Object, typename DontCare1, typename DontCare2,
              typename... Args>
    decltype(auto) invoke_helper(Function function, Object &&object,
                                 std::false_type /* is member pointer*/, DontCare1, DontCare2,
                                 Args &&...args)
    {
        return function(object, std::forward<Args>(args)...);
    }

    template <typename Member, typename Object, typename... Args>
    decltype(auto) invoke_helper(Member member, Object &&object,
                                 std::true_type /* is member pointer */,
                                 std::true_type /* is member function */,
                                 std::true_type /* is object a pointer */, Args &&...args)
    {
        return (std::forward<Object>(object)->*member)(std::forward<Args>(args)...);
    }

    template <typename Member, typename Object>
    decltype(auto)
    invoke_helper(Member member, Object &&object, std::true_type /* is member pointer */,
                  std::false_type /* is member function */, std::true_type /* is object a pointer*/)
    {
        return std::forward<Object>(object)->*member;
    }

    template <typename Member, typename Object, typename... Args>
    decltype(auto) invoke_helper(Member member, Object &&object,
                                 std::true_type /* is member pointer */,
                                 std::true_type /* is member function */,
                                 std::false_type /* is object a pointer */, Args &&...args)
    {
        return (std::forward<Object>(object).*member)(std::forward<Args>(args)...);
    }

    template <typename Member, typename Object>
    decltype(auto) invoke_helper(Member member, Object &&object,
                                 std::true_type /* is member pointer */,
                                 std::false_type /* is member function */,
                                 std::false_type /* is object a pointer */)
    {
        return std::forward<Object>(object).*member;
    }

    template <typename Function, typename Object, typename... Args>
    decltype(auto) invoke(Function function, Object &&object, Args &&...args)
    {
        using Type = std::remove_reference_t<Object>;
        return invoke_helper(function, std::forward<Object>(object),
                             std::is_member_pointer<Function>(),
                             std::is_member_function_pointer<Function>(), std::is_pointer<Type>(),
                             std::forward<Args>(args)...);
    }

} // namespace detail
} // kdalgorithms
