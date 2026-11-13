#pragma once
#include "Formatter.hpp"
#include <cmath>
#include <format>
#include <string>

class XML : public Formatter<XML>
{
public:
    XML(bool pretty = true, size_t indentWidth = 4) : mPretty(pretty), mIndentWidth(indentWidth) {};
    std::string FormatNull() { return ""; }

    std::string FormatInt(int64_t val) { return std::to_string(val); }

    std::string FormatUInt(uint64_t val) { return std::to_string(val); }

    std::string FormatDouble(double val)
    {
        if (std::isnan(val) | std::isinf(val))
        {
            return FormatNull();
        }
        return std::to_string(val);
    }

    std::string FormatBool(bool val) { return val ? "true" : "false"; }

    std::string FormatString(std::string_view str) { return std::string(str); }

    std::string FormatArrayElement(size_t i, std::string_view value) { return FormatKeyValuePair(std::to_string(i), value); }

    std::string FormatKeyValuePair(std::string_view key, std::string_view value)
    {
        return std::format("<{0}>{1}</{0}>", key, value);
    }

    std::string FormatArrayStart() { return ""; }

    std::string FormatArrayEnd() { return ""; }

    std::string FormatObjectStart() { return ""; }

    std::string FormatObjectEnd() { return ""; }

    std::string FormatSeparator(SeparaterPlacement placement)
    {
        switch (placement)
        {
        case SeparaterPlacement::BeforeFirstElement:
        case SeparaterPlacement::AfterLastElement:
        case SeparaterPlacement::BetweenElements:
        return (mPretty ? "\n" : "");
        }
        std::unreachable();
    }
    std::string FormatIndent(size_t depth) { return mPretty ? std::string(mIndentWidth * depth, ' ') : ""; }

private:
    bool mPretty;
    size_t mIndentWidth;
};

static_assert(FormatType<XML>);
