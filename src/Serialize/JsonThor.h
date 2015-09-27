
#ifndef THORS_ANVIL_SERIALIZE_JSON_H
#define THORS_ANVIL_SERIALIZE_JSON_H
/*
 * Defines the Json Serialization interface
 *      ThorsAnvil::Serialize::Json
 *      ThorsAnvil::Serialize::jsonExport
 *      ThorsAnvil::Serialize::jsonImport
 *      ThorsAnvil::Serialize::jsonSchemaExport
 *
 * Usage:
 *      std::cout << jsonExport(object); // converts object to Json on an output stream
 *      std::cin  >> jsonImport(object); // converts Json to a C++ object from an input stream
 *      std::cout  >> jsonSchemaExtract(object); // converts object to Json Schema on an output stream
 *      std::cout  >> jsonSchemaExtract<Type>(); // converts Type to Json Schema on an output stream
 */

#include "JsonParser.h"
#include "JsonPrinter.h"
#include "Exporter.h"
#include "Importer.h"
#include "JsonSchemaExtractor.h"

namespace ThorsAnvil
{
    namespace Serialize
    {

struct Json
{
    typedef JsonParser      Parser;
    typedef JsonPrinter     Printer;
};

template<typename T>
Exporter<Json, T> jsonExport(T const& value, PrinterInterface::OutputType characteristics = PrinterInterface::OutputType::Default)
{
    return Exporter<Json, T>(value, characteristics);
}
template<typename T>
Importer<Json, T> jsonImport(T& value)
{
    return Importer<Json, T>(value);
}
template<typename T>
JsonSchemaExtractor<Json, T> jsonSchemaExtract(T const& value, PrinterInterface::OutputType characteristics = PrinterInterface::OutputType::Default)
{
    return JsonSchemaExtractor<Json, T>(value, characteristics);
}
template<typename T>
JsonSchemaExtractor<Json, T> jsonSchemaExtract(PrinterInterface::OutputType characteristics = PrinterInterface::OutputType::Default)
{
    return JsonSchemaExtractor<Json, T>(T(), characteristics);
}
    }
}

#endif

