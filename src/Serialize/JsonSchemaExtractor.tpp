
#ifndef THORS_ANVIL_SERIALIZE_JSON_SCHEMA_EXTRACTOR_TPP
#define THORS_ANVIL_SERIALIZE_JSON_SCHEMA_EXTRACTOR_TPP

namespace ThorsAnvil
{
    namespace Serialize
    {

/* ------------ JsonSchemaApplyActionToParent ------------------------- */

template<typename T, typename I>
class JsonSchemaApplyActionToParent<TraitType::Parent, T, I>
{
    public:
        void printParentMembers(JsonSchemaSerializer& serializer, T const& object)
        {
            serializer.printObjectMembers(static_cast<typename Traits<T>::Parent const&>(object));
        }
};

/* ------------ JsonSchemaSerializerForBlock ------------------------- */

template<TraitType traitType, typename T>
class JsonSchemaSerializerForBlock
{
    static_assert(
        traitType != TraitType::Invalid,
        "Invalid Serialize TraitType. This usually means you have not define ThorsAnvil::Serialize::Traits<Your Type>"
    );

    JsonSchemaSerializer&         parent;
    PrinterInterface&   printer;
    T const&            object;
    public:
        JsonSchemaSerializerForBlock(JsonSchemaSerializer& parent, PrinterInterface& printer, T const& object)
            : parent(parent)
            , printer(printer)
            , object(object)
        {}
        ~JsonSchemaSerializerForBlock(){}
        void printMembers()
        {
            printer.openMap();
            printer.addKey("type");
            printer.addValue(std::string("object"));
            printer.addKey("properties");
            printer.openMap();
            parent.printObjectMembers(object);
            printer.closeMap();
            printer.closeMap();
        }
};

inline std::string getJsonTypeName(short int) { return "integer"; }
inline std::string getJsonTypeName(int) { return "integer"; }
inline std::string getJsonTypeName(long int) { return "integer"; }
inline std::string getJsonTypeName(long long int) { return "integer"; }
inline std::string getJsonTypeName(unsigned short int) { return "integer"; }
inline std::string getJsonTypeName(unsigned int) { return "integer"; }
inline std::string getJsonTypeName(unsigned long int) { return "integer"; }
inline std::string getJsonTypeName(unsigned long long int) { return "integer"; }
inline std::string getJsonTypeName(float) { return "number"; }
inline std::string getJsonTypeName(double) { return "number"; }
inline std::string getJsonTypeName(long double) { return "number"; }
inline std::string getJsonTypeName(bool) { return "bool"; }
inline std::string getJsonTypeName(std::string const&) { return "string"; }

template<typename T>
class JsonSchemaSerializerForBlock<TraitType::Value, T>
{
    PrinterInterface&   printer;
    T const&            object;

    public:
        JsonSchemaSerializerForBlock(JsonSchemaSerializer&, PrinterInterface& printer,T const& object)
            : printer(printer)
            , object(object)
        {}
        ~JsonSchemaSerializerForBlock() {}
        void printMembers()
        {
            printer.openMap();
            printer.addKey("type");
            printer.addValue(getJsonTypeName(object));
            printer.closeMap();
        }
};

template<typename T>
class JsonSchemaSerializerForBlock<TraitType::Enum, T>
{
    PrinterInterface&   printer;
    T const&            object;
    public:
        JsonSchemaSerializerForBlock(JsonSchemaSerializer&, PrinterInterface& printer,T const& object)
            : printer(printer)
            , object(object)
        {}
        ~JsonSchemaSerializerForBlock() {}
        void printMembers()
        {
            printer.openMap();
            printer.addKey("type");
            printer.addValue(std::string("string"));
            printer.addKey("enum");
            printer.openArray(Traits<T>::getSize());
            for (std::size_t i=0; i<Traits<T>::getSize(); ++i)
                printer.addValue(std::string(Traits<T>::getValues()[i]));
            printer.closeArray();
            printer.closeMap();
        }
};

template<typename T>
class JsonSchemaSerializerForBlock<TraitType::Array, T>
{
    JsonSchemaSerializer&         parent;
    PrinterInterface& printer;
    T const&            object;
    public:
        JsonSchemaSerializerForBlock(JsonSchemaSerializer& parent, PrinterInterface& printer, T const& object)
            : parent(parent)
            , printer(printer)
            , object(object)
        {}
        ~JsonSchemaSerializerForBlock() {}
        void printMembers()
        {
            printer.openMap();
            printer.addKey("type");
            printer.addValue(std::string("array"));
            printer.addKey("items");
            typename T::value_type item;
            JsonSchemaSerializerForBlock<Traits<typename T::value_type>::type, typename T::value_type> serialzier(parent, printer, item);
            serialzier.printMembers();
            printer.closeMap();
        }
};

/* ------------ JsonSchemaSerializeMember ------------------------- */

template<typename T, typename M, TraitType type>
JsonSchemaSerializeMember<T, M, type>::JsonSchemaSerializeMember(PrinterInterface& printer, T const& object, std::pair<char const*, M T::*> const& memberInfo)
{
    static_assert(type != TraitType::Invalid, "Trying to serialize an object that does not have a ThorsAnvil::Serialize::Trait<> defined."
                                              "Look at macro ThorsAnvil_MakeTrait() for more information.");

    printer.addKey(memberInfo.first);

    JsonSchemaSerializer      serialzier(printer, false);
    serialzier.print(object.*(memberInfo.second));
}
template<typename T, typename M>
class JsonSchemaSerializeMember<T, M, TraitType::Value>
{
    public:
        JsonSchemaSerializeMember(PrinterInterface& printer, T const& object, std::pair<char const*, M T::*> const& memberInfo)
        {
            printer.addKey(memberInfo.first);
            JsonSchemaSerializer      serialzier(printer, false);
            serialzier.print(object.*(memberInfo.second));
        }
};

template<typename T, typename M>
JsonSchemaSerializeMember<T, M> make_JsonSchemaSerializeMember(PrinterInterface& printer, T const& object, std::pair<char const*, M T::*> const& memberInfo)
{
    return JsonSchemaSerializeMember<T,M>(printer, object, memberInfo);
}

/* ------------ JsonSchemaSerializer ------------------------- */

template<typename T, typename Members, std::size_t... Seq>
inline void JsonSchemaSerializer::printEachMember(T const& object, Members const& member, std::index_sequence<Seq...> const&)
{
    auto discard = {(make_JsonSchemaSerializeMember(printer, object, std::get<Seq>(member)),1)...};
    (void)discard;
}

template<typename T, typename... Members>
inline void JsonSchemaSerializer::printMembers(T const& object, std::tuple<Members...> const& members)
{
    printEachMember(object, members, std::make_index_sequence<sizeof...(Members)>());
}

template<typename T, typename Action>
inline void JsonSchemaSerializer::printMembers(T const& object, Action action)
{
    action(printer, object);
}

template<typename T>
inline void JsonSchemaSerializer::print(T const& object)
{
    JsonSchemaSerializerForBlock<Traits<T>::type, T>     block(*this, printer, object);
    block.printMembers();
}

template<typename T>
inline void JsonSchemaSerializer::printObjectMembers(T const& object)
{
    typedef typename IndexType<Traits<T>::type>::IndexInfoType IndexInfoType;

    JsonSchemaApplyActionToParent<Traits<T>::type, T, IndexInfoType>     parentPrinter;

    parentPrinter.printParentMembers(*this, object);
    printMembers(object, Traits<T>::getMembers());
}

    }
}

#endif

