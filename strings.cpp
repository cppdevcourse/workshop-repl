#include <iostream>

#include "repl.hpp"

namespace repl
{
namespace strings
{

class Concat : public ::repl::BaseCommand<Concat, std::string, std::string, std::string>
{
public:
    const std::string_view longName = "concat";
    const std::string_view shortName = "ct";
    Concat() = default;
    std::string operator()(const std::string& s1, const std::string& s2) const
    {
        return s1 + s2;
    }
};

class Find : public ::repl::BaseCommand<Find, std::size_t, std::string, char>
{
public:
    const std::string_view longName = "find";
    const std::string_view shortName = "fd";
    Find() = default;
    std::size_t operator()(const std::string& str, const char c) const
    {
        for (std::size_t i = 0; i < str.size(); i++)
        {
            if (str[i] == c)
                return i;
        }
        throw std::invalid_argument("Character not found");
    }
};

class Length : public ::repl::BaseCommand<Length, std::size_t, std::string>
{
public:
    const std::string_view longName = "length";
    const std::string_view shortName = "ln";
    Length() = default;
    std::size_t operator()(const std::string& str) const
    {
        return str.size();
    }
};

class AtIndex : public ::repl::BaseCommand<AtIndex, char, std::string, std::size_t>
{
public:
    const std::string_view longName = "atindex";
    const std::string_view shortName = "at";
    AtIndex() = default;
    std::size_t operator()(const std::string& str, const std::size_t index) const
    {
        if (index >= str.size())
        {
            throw std::invalid_argument("Index out of bounds");
        }

        return str[index];
    }
};
// TODO: добавьте классы команд

using Repl = ::repl::Repl<Concat, Find, Length, AtIndex>;

} // namespace strings
} // namespace repl

int main()
{
    using namespace repl::strings;

    try
    {
        Repl repl{std::cin, std::cout, "str"};
        repl.Run();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& err)
    {
        std::cerr << "Error: " << err.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Error: unknown\n";
    }
    return EXIT_FAILURE;
}
