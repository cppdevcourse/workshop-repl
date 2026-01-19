#pragma once

#include <algorithm>
#include <exception>
#include <functional>
#include <ostream>
#include <istream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "command.hpp"

namespace repl
{
namespace builtin
{

template<class Repl>
class Help : public BaseCommand<Help<Repl>, std::string>
{
public:
    const std::string_view longName = "help";
    const std::string_view shortName = "h";

    Help() = default;

    std::string operator()() const
    {
        typename Repl::CommandsTuple commands{};
        std::set<std::string> lines{};
        auto func = [&lines](auto&... cmd)
        { ((lines.insert(command_to_line(cmd))), ...); };
        std::apply(std::move(func), commands);

        std::string output{};
        for (auto it = lines.cbegin(); it != lines.cend(); ++it)
        {
            if (it != lines.cbegin())
            {
                output.push_back('\n');
            }
            output += *it;
        }
        return output;
    }

    template<class Command>
    static std::string command_to_line(const Command& command)
    {
        return std::string{command.longName} + ", " +
               std::string{command.shortName};
    }
};

class Exit : public BaseCommand<Exit, std::string>
{
public:
    const std::string_view longName = "exit";
    const std::string_view shortName = "e";
    Exit() = default;

    template<class T>
    void SetAction(T&& action)
    {
        action_ = action;
    }

    std::string operator()() const
    {
        if (action_)
        {
            action_();
        }
        return {};
    }

private:
    std::function<void()> action_;
};

} // namespace builtin

template<typename C>
concept Command = requires(const C c) {
    { c.longName } -> std::convertible_to<std::string_view>;
    { c.shortName } -> std::convertible_to<std::string_view>;
} && requires(const C c, const std::vector<std::string_view>& tokens) {
    { c.Execute(tokens) } -> std::same_as<decltype(c.Execute(tokens))>;
};

template<class... Commands>
    requires(Command<Commands> && ...)
class Repl
{
    struct Expression
    {
        std::string_view command;
        std::vector<std::string_view> args;
    };

public:
    using CommandsTuple = std::tuple<builtin::Exit,
        builtin::Help<Repl<Commands...>>, Commands...>;

    Repl(std::istream& input, std::ostream& output, std::string_view name = "")
        : input_(input), output_(output), prompt_(std::string{name} + "> ")
    {
        FindNamesConflicts();
        std::get<0>(commands_).SetAction([this]() { continue_ = false; });
    }
    void Run() noexcept
    {
        while (continue_)
        {
            try
            {
                std::string line{};
                PrintPrompt();
                std::getline(input_, line);
                const auto expression = parse(line);
                Evaluate(expression);
            }
            catch (const std::exception& err)
            {
                output_ << "Error: " << err.what() << '\n';
            }
            catch (...)
            {
                output_ << "Error: unknown\n";
            }
        }
    }

private:
    void Evaluate(const Expression& expr)
    {
        bool evaluated{false};
        auto func = [&](auto&... cmd)
        {
            (((!evaluated && (cmd.longName == expr.command ||
                                 cmd.shortName == expr.command))
                     ? (evaluated = true,
                           output_ << cmd.Execute(expr.args) << '\n')
                     : output_),
                ...);
        };
        std::apply(std::move(func), commands_);

        if (!evaluated)
        {
            std::ostringstream msg{};
            msg << "command '" << expr.command << "' not found";
            throw std::runtime_error(msg.str());
        }
    }

    void PrintPrompt() noexcept
    {
        output_ << prompt_;
    }

    static Expression parse(std::string_view str)
    {
        Expression expression{};
        std::size_t start = 0;

        size_t pos = str.find(' ', start);
        if (pos == std::string_view::npos)
        {
            expression.command = str;
            return expression;
        }
        expression.command = str.substr(start, pos - start);
        start = pos + 1;

        while (true)
        {
            pos = str.find(' ', start);
            if (pos == std::string_view::npos)
            {
                expression.args.emplace_back(str.substr(start));
                break;
            }
            expression.args.emplace_back(str.substr(start, pos - start));
            start = pos + 1;
        }

        return expression;
    }

    void FindNamesConflicts()
    {
        constexpr std::size_t commandsCount = sizeof...(Commands);
        std::set<std::string_view> names{};
        auto func = [&names](auto&... cmd)
        { ((names.insert(cmd.longName), names.insert(cmd.shortName)), ...); };
        std::apply(std::move(func), commands_);

        if (names.size() < commandsCount * 2)
        {
            throw std::runtime_error("commands names conflict");
        }
    }

private:
    std::istream& input_;
    std::ostream& output_;
    std::string prompt_;
    CommandsTuple commands_;
    bool continue_ = true;
};

} // namespace repl
