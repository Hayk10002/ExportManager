#pragma once
#include "formatters/Formatter.hpp"
#include <memory>
#include <string>


class AnyFormatter {
    struct FormatterConcept
    {
        virtual ~FormatterConcept() = default;
        virtual std::string Format(const Data& data) = 0;
    };

    template <FormatType Type>
    struct FormatterModel : public FormatterConcept
    {
        Type mFormatter;
        FormatterModel(Type formatter) : mFormatter(std::move(formatter)) {}
        std::string Format(const Data& data) override
        {
            return mFormatter.Format(data);
        }
    };

    std::unique_ptr<FormatterConcept> mFormatter;
public:

    template<FormatType Type>
    AnyFormatter(Type formatter) : mFormatter(std::make_unique<FormatterModel<Type>>(std::move(formatter))) {}

    std::string Format(const Data& data)
    {
        return mFormatter->Format(data);
    }
};