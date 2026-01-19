#include <iostream>

#include "repl.hpp"

namespace repl
{
namespace strings
{

// TODO: добавьте классы команд

using Repl = ::repl::Repl<>;

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
