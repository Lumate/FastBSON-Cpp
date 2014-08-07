#include "../element.h"
#include "gtest/gtest.h"
#include "../document.h"
#include <sstream>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <set>
#include "../json/jsonscanner.h"

TEST(JSONConvert, FromString)
{
  std::string key="asdf", value="asdf2";
  bson::Document d("{\"" + key + "\":\"" + value + "\"}");
  ASSERT_EQ(1, d.field_names().size());
  ASSERT_EQ(value, d[key].data<std::string>());
}

TEST(JSONConvert, FromInt)
{
  std::string key="asdf", value="42";
  int ival = 42;
  bson::Document d("{\"" + key + "\":" + value + "}");
  ASSERT_EQ(1, d.field_names().size());
  ASSERT_EQ(ival, d[key].data<long>());
}

TEST(JSONConvert, FromDouble)
{
  std::string key="asdf", value="3.14000";
  double dval = 3.14000;
  bson::Document d("{\"" + key + "\":" + value + "}");
  ASSERT_EQ(1, d.field_names().size());
  ASSERT_EQ(dval, d[key].data<double>());
}

TEST(JSONConvert, FromNull)
{
  std::string key = "asdf";
  bson::Document d("{\"" + key + "\": null }");
  ASSERT_EQ(1, d.field_names().size());
  ASSERT_EQ(bson::NIL, d[key].get_type());
}

TEST(JSONConvert, FromArr)
{
  std::string key = "asdf";
  bson::Document d("{\"" + key + "\": [0, 1, 2, 3, 4, 5, 6.2] }");
  ASSERT_EQ(1, d.field_names().size());
  for (int i=0; i<6; i++)
    ASSERT_EQ(i, d[key].data<bson::array>()[i].data<long>());
  ASSERT_EQ(6.2, d[key].data<bson::array>()[6].data<double>());
}

TEST(JSONConvert, FromBool)
{
  std::string key = "asdf";
  bson::Document d("{\"" + key + "\": true }");
  ASSERT_EQ(1, d.field_names().size());
  ASSERT_TRUE(d[key].data<bool>());
  d.from_json("{\"" + key + "\": false }");
  ASSERT_EQ(1, d.field_names().size());
  ASSERT_FALSE(d[key].data<bool>()); 
}

TEST(JSONConvert, InvalidJSON)
{
  bson::Document d;
  ASSERT_THROW(d.from_json("asdf"), bson::invalid_token);
}

TEST(JSONConvert, EmptyObject)
{
  bson::Document d("{}");
  ASSERT_EQ(0, d.field_names().size());
}

TEST(JSONConvert, EmptyArray)
{
  bson::Document d("{\"asdf\": []}");
  ASSERT_EQ(1, d.field_names().size());
  ASSERT_EQ(0, d["asdf"].data<bson::array>().size());
}

TEST(JSONConvert, NestedObject)
{
  bson::Document d("{\"asdf\": {}}");
  ASSERT_EQ(1, d.field_names().size());
  ASSERT_EQ(0, d["asdf"].data<bson::Document>().field_names().size());
}

TEST(JSONConvert, MultiField)
{
  bson::Document d("{\"a\": 1, \"b\": 3.14000}");
  ASSERT_EQ(2, d.field_names().size());
  ASSERT_EQ(1, d["a"].data<long>());
  ASSERT_EQ(3.14000, d["b"].data<double>());
}