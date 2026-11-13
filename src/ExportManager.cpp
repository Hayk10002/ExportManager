#include "ExportManager.hpp"
#include <fstream>
#include <memory>
#include <vector>

struct ExportManager::Impl
{
    std::vector<std::pair<AnyFormatter, std::string>> formatters;
};

ExportManager::ExportManager() : pImpl(std::make_unique<Impl>()) {}

ExportManager::~ExportManager() = default;
ExportManager::ExportManager(ExportManager&& other) = default;
ExportManager& ExportManager::operator=(ExportManager&& other) = default;

void ExportManager::RegisterFormatter(AnyFormatter formatter, std::string outFileNamePostfix)
{
    pImpl->formatters.push_back({std::move(formatter), outFileNamePostfix});
}

void ExportManager::ExportToFile(const Data &data, std::string outFileName)
{
    for (auto & [formatter, postfix] : pImpl->formatters)
    {
        std::string outFile = outFileName + postfix;

        std::ofstream file(outFile);
        if (file)
        {
            file << formatter.Format(data);
        }
    }
}