#include "ExportManager.hpp"
#include "formatters/Json.hpp"
#include "formatters/XML.hpp"
#include <initializer_list>
#include <limits>
#include <map>

int main()
{
    Data data = Data::Array(std::initializer_list{ Data::Int(-6), Data::UInt(67), Data::Null(), Data::Bool(false), Data::Bool(true),
                                                   Data::String("Args &&args..."), Data::Array(),
                                                   Data::Array(std::initializer_list{ Data::Object(std::map<std::string, Data>{
                                                       { "a", Data::String("a") },
                                                       { "b", Data::Double(std::numeric_limits<double>::infinity()) },
                                                       { "c", Data::Double(std::numeric_limits<double>::quiet_NaN()) } }) }),
                                                   Data::Object() });

    ExportManager manager;

    manager.RegisterFormatter(JSON{ false }, ".json");
    manager.RegisterFormatter(JSON{ true }, "_pretty.json");
    manager.RegisterFormatter(XML{ false }, ".xml");
    manager.RegisterFormatter(XML{ true }, "_pretty.xml");

    manager.ExportToFile(data, "customData");
}
