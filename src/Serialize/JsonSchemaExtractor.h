
#ifndef THORS_ANVIL_SERIALIZE_JSON_SCHEMA_EXTRACTOR_H
#define THORS_ANVIL_SERIALIZE_JSON_SCHEMA_EXTRACTOR_H

#include "Serialize.h"

namespace ThorsAnvil
{
    namespace Serialize
    {


class JsonSchemaSerializer;

template<TraitType type, typename T, typename I>
class JsonSchemaApplyActionToParent
{
    public:
        // Default do nothing.
        void printParentMembers(JsonSchemaSerializer&, T const&)            {}
};


template<typename T, typename M, TraitType type = Traits<typename std::remove_cv<M>::type>::type>
class JsonSchemaSerializeMember
{
    public:
        JsonSchemaSerializeMember(PrinterInterface& printer, T const& object, std::pair<char const*, M T::*> const& memberInfo);
};

class JsonSchemaSerializer
{
    PrinterInterface& printer;
    bool              root;

    template<typename T, typename Members, std::size_t... Seq>
    void printEachMember(T const& object, Members const& member, std::index_sequence<Seq...> const&);

    template<typename T, typename... Members>
    void printMembers(T const& object, std::tuple<Members...> const& members);

    template<typename T, typename Action>
    void printMembers(T const& object, Action action);

    public:
        JsonSchemaSerializer(PrinterInterface& printer, bool root = true);
        ~JsonSchemaSerializer();

        template<typename T>
        void print(T const& object);

        template<typename T>
        void printObjectMembers(T const& object);
};

template<typename Format, typename T>
class JsonSchemaExtractor
{
    using OutputType = PrinterInterface::OutputType;
    T const&    value;
    OutputType  characteristics;

    public:
        JsonSchemaExtractor(T const& value, OutputType characteristics)
            : value(value)
            , characteristics(characteristics)
        {}
        friend std::ostream& operator<<(std::ostream& stream, JsonSchemaExtractor const& data)
        {
            typename Format::Printer    printer(stream, data.characteristics);
            JsonSchemaSerializer                  serializer(printer);

            serializer.print(data.value);

            return stream;
        }
};

inline JsonSchemaSerializer::JsonSchemaSerializer(PrinterInterface& printer, bool root)
    : printer(printer)
    , root(root)
{
    if (root)
    {
        printer.openDoc();
    }
}
inline JsonSchemaSerializer::~JsonSchemaSerializer()
{
    if (root)
    {
        printer.closeDoc();
    }
}

    }
}

#ifndef COVERAGE_TEST
#include "JsonSchemaExtractor.tpp"
#endif

#endif

