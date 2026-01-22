#pragma once

#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace repl
{

template<class T>
struct ArgParser;

template<>
struct ArgParser<float>
{
    static float parse(std::string_view str)
    {
        try
        {
            return std::stof(std::string{str});
        }
        catch (const std::invalid_argument& err)
        {
            // std::stof message is kind of stupid,
            // we will replace it
            std::ostringstream msg{};
            msg << "failed to parse float from '" << str << '\'';
            throw std::invalid_argument(msg.str());
        }
    }
};

template<>
struct ArgParser<std::string>
{
    static std::string parse(std::string_view str)
    {
        return std::string{str};
    }
};

template<>
struct ArgParser<char>
{
    static char parse(std::string_view str)
    {
        if (str.size() != 1)
        {
            throw std::invalid_argument("Argument should be single char");
        }
        return str[0];
    }
};

template<>
struct ArgParser<std::size_t>
{
    static std::size_t parse(std::string_view str)
    {
        try
        {
            return std::stoull(std::string{str});
        }
        catch (const std::invalid_argument& err)
        {
            // std::stof message is kind of stupid,
            // we will replace it
            std::ostringstream msg{};
            msg << "failed to parse size_t from '" << str << '\'';
            throw std::invalid_argument(msg.str());
        }
    }
};

// TODO: добавьте парсеры

} // namespace repl
