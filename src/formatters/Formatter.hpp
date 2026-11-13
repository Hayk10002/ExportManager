
#pragma once
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

struct Data : public std::variant<std::monostate, int64_t, uint64_t, double, bool, std::string, std::vector<Data>,
                                  std::map<std::string, Data>>
{
    using Base =
        std::variant<std::monostate, int64_t, uint64_t, double, bool, std::string, std::vector<Data>, std::map<std::string, Data>>;

    template <typename... Args>
    Data(Args &&... args) : Base(std::forward<Args>(args)...) {};

    static Data Null() { return Data{}; }

    template <typename... Args>
    static Data Int(Args &&... args)
    {
        return Base{ std::in_place_index<1>, std::forward<Args>(args)... };
    };
    template <typename... Args>
    static Data UInt(Args &&... args)
    {
        return Base{ std::in_place_index<2>, std::forward<Args>(args)... };
    };
    template <typename... Args>
    static Data Double(Args &&... args)
    {
        return Base{ std::in_place_index<3>, std::forward<Args>(args)... };
    };
    template <typename... Args>
    static Data Bool(Args &&... args)
    {
        return Base{ std::in_place_index<4>, std::forward<Args>(args)... };
    };
    template <typename... Args>
    static Data String(Args &&... args)
    {
        return Base{ std::in_place_index<5>, std::forward<Args>(args)... };
    };
    template <typename... Args>
    static Data Array(Args &&... args)
    {
        return Base{ std::in_place_index<6>, std::forward<Args>(args)... };
    };
    template <typename... Args>
    static Data Object(Args &&... args)
    {
        return Base{ std::in_place_index<7>, std::forward<Args>(args)... };
    };
};

enum class SeparaterPlacement
{
    BeforeFirstElement,
    BetweenElements,
    AfterLastElement
};

template <typename T>
concept FormatType = requires(T format) {
    { format.FormatNull() } -> std::same_as<std::string>;
    { format.FormatInt(int64_t()) } -> std::same_as<std::string>;
    { format.FormatUInt(uint64_t()) } -> std::same_as<std::string>;
    { format.FormatDouble(double()) } -> std::same_as<std::string>;
    { format.FormatBool(bool()) } -> std::same_as<std::string>;
    { format.FormatString(std::string_view()) } -> std::same_as<std::string>;
    { format.FormatArrayElement(size_t() /*index*/, std::string_view() /*value*/) } -> std::same_as<std::string>;
    { format.FormatKeyValuePair(std::string_view() /*key*/, std::string_view() /*value*/) } -> std::same_as<std::string>;
    { format.FormatArrayStart() } -> std::same_as<std::string>;
    { format.FormatArrayEnd() } -> std::same_as<std::string>;
    { format.FormatObjectStart() } -> std::same_as<std::string>;
    { format.FormatObjectEnd() } -> std::same_as<std::string>;
    { format.FormatSeparator(SeparaterPlacement()) } -> std::same_as<std::string>;
    { format.FormatIndent(size_t() /*depth*/) } -> std::same_as<std::string>;
};

template <typename Type>
class Formatter
{
public:
    std::string Format(const Data & data)
        requires FormatType<Type> // enfore the type here because at the class template level the `Type` class is still incomplete
    {
        Type & format = *static_cast<Type *>(this);
        if (std::holds_alternative<std::monostate>(data))
        {
            return format.FormatNull();
        }

        if (std::holds_alternative<int64_t>(data))
        {
            return format.FormatInt(std::get<int64_t>(data));
        }

        if (std::holds_alternative<uint64_t>(data))
        {
            return format.FormatUInt(std::get<uint64_t>(data));
        }

        if (std::holds_alternative<double>(data))
        {
            return format.FormatDouble(std::get<double>(data));
        }

        if (std::holds_alternative<bool>(data))
        {
            return format.FormatBool(std::get<bool>(data));
        }

        if (std::holds_alternative<std::string>(data))
        {
            return format.FormatString(std::get<std::string>(data));
        }

        if (std::holds_alternative<std::vector<Data>>(data))
        {
            std::string result;
            const auto & arr = std::get<std::vector<Data>>(data);
            if (arr.empty())
            {
                return format.FormatArrayStart() + format.FormatArrayEnd();
            }
            result += format.FormatArrayStart();
            result += format.FormatSeparator(SeparaterPlacement::BeforeFirstElement);
            bool first = true;
            currentDepth++;
            for (size_t i = 0; i < arr.size(); i++)
            {
                const auto & element = arr[i];
                if (!first)
                {
                    result += format.FormatSeparator(SeparaterPlacement::BetweenElements);
                }

                result += format.FormatIndent(currentDepth);
                std::string elementStr = Format(element);
                result += format.FormatArrayElement(i, elementStr);
                first = false;
            }
            result += format.FormatSeparator(SeparaterPlacement::AfterLastElement);

            currentDepth--;
            result += format.FormatIndent(currentDepth);
            result += format.FormatArrayEnd();
            return result;
        }

        if (std::holds_alternative<std::map<std::string, Data>>(data))
        {
            std::string result;
            const auto & obj = std::get<std::map<std::string, Data>>(data);
            if (obj.empty())
            {
                return format.FormatObjectStart() + format.FormatObjectEnd();
            }
            result += format.FormatObjectStart();
            result += format.FormatSeparator(SeparaterPlacement::BeforeFirstElement);
            bool first = true;
            currentDepth++;
            for (const auto & [key, value] : obj)
            {
                if (!first)
                {
                    result += format.FormatSeparator(SeparaterPlacement::BetweenElements);
                }

                std::string keyStr   = format.FormatString(key);
                std::string valueStr = Format(value);

                result += format.FormatIndent(currentDepth);
                result += format.FormatKeyValuePair(keyStr, valueStr);
                first = false;
            }
            result += format.FormatSeparator(SeparaterPlacement::AfterLastElement);

            currentDepth--;
            result += format.FormatIndent(currentDepth);
            result += format.FormatObjectEnd();
            return result;
        }
        std::unreachable();
    }

private:
    size_t currentDepth = 0;
    // enforce that the Type inherits only from Formatter<Type>, so casting to it is not ub.
    Formatter() {};
    friend Type;
};
