
#include "gtest/gtest.h"
#include "json.h"

#include <map>

using ThorsAnvil::Serialize::jsonImport;
using ThorsAnvil::Serialize::jsonExport;


template<typename T>
std::string testAction(std::string const& expected)
{
    T                   object;

    std::stringstream   input(expected);
    input  >> jsonImport(object);

    std::stringstream   output;
    output << jsonExport(object);

    return output.str();
}

void ValidateSerializedStrings(std::string lhs, std::string rhs)
{
    lhs.erase(std::remove(lhs.begin(), lhs.end(), ' '), lhs.end());
    rhs.erase(std::remove(rhs.begin(), rhs.end(), ' '), rhs.end());

    ASSERT_EQ(lhs, rhs);
}


TEST(JsonSerialize, JsonMap)
{
    std::string input   = "{\"action\": 15}";
    std::string result  = testAction<std::map<std::string, int> >(input);
    ValidateSerializedStrings(input, result);
}

TEST(JsonSerialize, JsonMapWithMap)
{
    std::string input   = "{\"action\": {\"data\": 15, \"location\": 12}}";
    std::string result  = testAction<std::map<std::string, std::map<std::string, int> > >(input);
    ValidateSerializedStrings(input, result);
}

TEST(JsonSerialize, JsonMapWithMapNotStringKey)
{
    std::string input   = "[ {\"first\": 1, \"second\": {\"data\": 23, \"location\": 345}}, {\"first\": 2, \"second\": { \"data\": 18, \"location\": 567}}]";
    std::string result  = testAction<std::map<int, std::map<std::string, int> > >(input);
    ValidateSerializedStrings(input, result);
}

TEST(JsonSerialize, JsonArrayOfPOD)
{
    std::string input   = "[ 1, 2, 3, 4, 5, 6, 7]";
    std::string result  = testAction<std::vector<int> >(input);
    ValidateSerializedStrings(input, result);
}
TEST(JsonSerialize, JsonArrayOfArray)
{
    std::string input   = "[ [1, 2, 3, 4, 5, 6, 7], [1,2,3,4], [90,100,200,300]]";
    std::string result  = testAction<std::vector<std::vector<int> > >(input);
    ValidateSerializedStrings(input, result);
}
TEST(JsonSerialize, JsonArrayOfMap)
{
    std::string input   = "[ {\"alpha\": 123, \"beta\": 987, \"gama\": 200}, {\"alpha\": 45, \"beta\": 567, \"plop\": 56}]";
    std::string result  = testAction<std::vector<std::map<std::string, int> > >(input);
    ValidateSerializedStrings(input, result);
}


class EmptyJsonClass
{};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<EmptyJsonClass>
{
    static JsonSerializeType const  type    = Map;
    typedef void                  SerializeInfo;
};
}}}

TEST(JsonSerialize, EmptyJsonClass)
{
    std::string input   = "{}";
    std::string result  = testAction<EmptyJsonClass>(input);
    ValidateSerializedStrings(input, result);
}

class OneMemberJsonClass
{
public:
    int value;
    friend class ThorsAnvil::Serialize::Json::JsonSerializeTraits<OneMemberJsonClass>;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<OneMemberJsonClass>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(OneMemberJsonClass, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, OneMemberJsonClass)
{
    std::string input   = "{\"value\": 15}";
    std::string result  = testAction<OneMemberJsonClass>(input);
    ValidateSerializedStrings(input, result);
}

struct NestedJsonTest
{
    OneMemberJsonClass  nested;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<NestedJsonTest>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(NestedJsonTest, nested);
    typedef boost::mpl::vector<nested>   SerializeInfo;
};
}}}

TEST(JsonSerialize, NestedJsonTest)
{
    std::string input   = "{\"nested\": {\"value\": 105}}";
    std::string result  = testAction<NestedJsonTest>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestFloat
{
    float value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestFloat>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestFloat, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestFloat)
{
    std::string input   = "{\"value\": 1234.45}";
    std::string result  = testAction<TestFloat>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestDouble
{
    double value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestDouble>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestDouble, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestDouble)
{
    std::string input   = "{\"value\": 1234.45}";
    std::string result  = testAction<TestDouble>(input);
    ValidateSerializedStrings(input, result);
}


// #####

struct TestShort
{
    short value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestShort>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestShort, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestShort)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestShort>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestInt
{
    int value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestInt>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestInt, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestInt)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestInt>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestLong
{
    long value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestLong>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestLong, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestLong)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestLong>(input);
    ValidateSerializedStrings(input, result);
}


// #####
#if 0
struct TestLongLong
{
    long long value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestLongLong>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestLongLong, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestLongLong)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestLongLong>(input);
    ValidateSerializedStrings(input, result);
}
#endif

// #####

struct TestUnsignedShort
{
    unsigned short value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestUnsignedShort>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestUnsignedShort, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestUnsignedShort)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestUnsignedShort>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestUnsignedInt
{
    unsigned int value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestUnsignedInt>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestUnsignedInt, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestUnsignedInt)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestUnsignedInt>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestUnsignedLong
{
    unsigned long value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestUnsignedLong>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestUnsignedLong, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestUnsignedLong)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestUnsignedLong>(input);
    ValidateSerializedStrings(input, result);
}


// #####
#if 0
struct TestUnsignedLongLong
{
    unsigned long long value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestUnsignedLongLong>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestUnsignedLongLong, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestUnsignedLongLong)
{
    std::string input   = "{\"value\": 23}";
    std::string result  = testAction<TestUnsignedLongLong>(input);
    ValidateSerializedStrings(input, result);
}
#endif


// #####

struct TestBool
{
    bool value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestBool>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestBool, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestBoolTrue)
{
    std::string input   = "{\"value\": true}";
    std::string result  = testAction<TestBool>(input);
    ValidateSerializedStrings(input, result);
}

TEST(JsonSerialize, TestBoolFalse)
{
    std::string input   = "{\"value\": false}";
    std::string result  = testAction<TestBool>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestString
{
    std::string value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestString>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestString, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestString)
{
    std::string input   = "{\"value\": \"Hi There this is a string\"}";
    std::string result  = testAction<TestString>(input);
    ValidateSerializedStrings(input, result);
}

// #####

struct TestPointer
{
    TestString*     value;
};

namespace ThorsAnvil { namespace Serialize { namespace Json {
template<>
struct JsonSerializeTraits<TestPointer>
{
    static  JsonSerializeType const       type    = Map;
    THORSANVIL_SERIALIZE_JsonAttribute(TestPointer, value);
    typedef boost::mpl::vector<value>   SerializeInfo;
};
}}}

TEST(JsonSerialize, TestPointerNULL)
{
    std::string input   = "{\"value\": null }";
    std::string result  = testAction<TestPointer>(input);
    ValidateSerializedStrings(input, result);
}

TEST(JsonSerialize, TestPointerNotNull)
{
    std::string input   = "{\"value\": {\"value\":\"plop\" }}";
    std::string result  = testAction<TestPointer>(input);
    ValidateSerializedStrings(input, result);
}


