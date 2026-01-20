#pragma once

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <vector>
#include <utility>

#include "argument_parsers.hpp"

namespace repl
{

template<class Derived, class ReturnType, class... ParameterTypes>
class BaseCommand
{
public:
    using Args = std::tuple<ParameterTypes...>;
    using Return = ReturnType;
    static constexpr std::size_t arity = sizeof...(ParameterTypes);

    ReturnType Execute(const std::vector<std::string_view>& tokens) const
    {
        if (tokens.size() != arity)
        {
            std::ostringstream msg{};
            msg << "invalid arguments number, expected: " << arity
                << ", actual: " << tokens.size();
            throw std::runtime_error(msg.str());
        }

        const Args args{
            parse_args(tokens, std::index_sequence_for<ParameterTypes...>{})};

        auto func = [&](auto&&... a)
        { return static_cast<const Derived*>(this)->operator()(a...); };

        return std::apply(std::move(func), args);
    }

    template<std::size_t... Is>
    static Args parse_args(const std::vector<std::string_view>& tokens,
        std::index_sequence<Is...>)
    {
        return Args{ArgParser<ParameterTypes>::parse(tokens[Is])...};
    }

protected:
    BaseCommand() = default;
};

} // namespace repl
