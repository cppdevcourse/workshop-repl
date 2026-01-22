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
// TODO: добавьте классы команд

using Repl = ::repl::Repl<Concat>;

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
