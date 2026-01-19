#include <cmath>
#include <cstdlib>
#include <exception>
#include <string_view>
#include <iostream>

#include "command.hpp"
#include "repl.hpp"

namespace repl
{
namespace calculator
{

class Multiply : public repl::BaseCommand<Multiply, float, float, float>
{
public:
    const std::string_view longName = "multiply";
    const std::string_view shortName = "mul";
    Multiply() = default;
    float operator()(const float lhs, const float rhs) const
    {
        return lhs * rhs;
    }
};

class SquareRoot : public repl::BaseCommand<SquareRoot, float, float>
{
public:
    const std::string_view longName = "squareroot";
    const std::string_view shortName = "sqrt";
    SquareRoot() = default;
    float operator()(const float number) const
    {
        return std::sqrt(number);
    }
};

using Repl = ::repl::Repl<Multiply, SquareRoot>;

} // namespace calculator
} // namespace repl

int main()
{
    using namespace repl::calculator;

    try
    {
        Repl repl{std::cin, std::cout, "calc"};
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
