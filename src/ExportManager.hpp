#pragma once
#include "formatters/AnyFormatter.hpp"
#include <memory>
#include <string>


class ExportManager
{
public:
    ExportManager();
    ~ExportManager();
    ExportManager(ExportManager&& other);
    ExportManager& operator=(ExportManager&& other);

    void RegisterFormatter(AnyFormatter formatter, std::string outFileNamePostfix);
    void ExportToFile(const Data& data, std::string outFileName);

private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};