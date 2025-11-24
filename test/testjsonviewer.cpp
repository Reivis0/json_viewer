#include <gtest/gtest.h>
#include <QByteArray>
#include <QString>
#include "jsonmodel.h"

// ИСПРАВЛЕННЫЙ МАКРОС
// Мы явно создаем QString из textStr перед передачей в функцию и перед выводом
#define EXPECT_HAS_ELEMENT(model, parent, textStr) \
    EXPECT_TRUE(model.hasElement(parent, QString(textStr))) << "Expected element not found: " << QString(textStr).toStdString()

TEST(JsonModelTest, LoadJsonWithSimpleObject)
{
  JsonModel model;
  QByteArray json = R"({
    "name": "John Doe",
    "age": 30
  })";

  ASSERT_TRUE(model.loadJson(json));

  EXPECT_EQ(model.rowCount(), 1);
  EXPECT_HAS_ELEMENT(model, QModelIndex(), "object {2}");

  QModelIndex root = model.index(0, 0);
  EXPECT_EQ(model.rowCount(root), 2);
  
  EXPECT_HAS_ELEMENT(model, root, "name : \"John Doe\"");
  EXPECT_HAS_ELEMENT(model, root, "age : 30");

  model.clear();
  EXPECT_EQ(model.rowCount(), 0);
}

TEST(JsonModelTest, LoadJsonWithEmptyObject)
{
  JsonModel model;
  QByteArray json = "{}";

  ASSERT_TRUE(model.loadJson(json));

  EXPECT_EQ(model.rowCount(), 1);
  EXPECT_HAS_ELEMENT(model, QModelIndex(), "object {0}");

  model.clear();
  EXPECT_EQ(model.rowCount(), 0);
}

TEST(JsonModelTest, LoadJsonWithEmptyArray)
{
  JsonModel model;
  QByteArray json = "[]";

  ASSERT_TRUE(model.loadJson(json));

  EXPECT_EQ(model.rowCount(), 1);
  EXPECT_HAS_ELEMENT(model, QModelIndex(), "array [0]");

  model.clear();
  EXPECT_EQ(model.rowCount(), 0);
}

TEST(JsonModelTest, LoadJsonWithSimpleArray)
{
  JsonModel model;
  QByteArray json = R"([
    "apple",
    "banana"
  ])";

  ASSERT_TRUE(model.loadJson(json));

  EXPECT_EQ(model.rowCount(), 1);
  EXPECT_HAS_ELEMENT(model, QModelIndex(), "array [2]");
  
  QModelIndex root = model.index(0, 0);
  EXPECT_EQ(model.rowCount(root), 2);
  
  EXPECT_HAS_ELEMENT(model, root, "0 : \"apple\"");
  EXPECT_HAS_ELEMENT(model, root, "1 : \"banana\"");

  model.clear();
  EXPECT_EQ(model.rowCount(), 0);
}

TEST(JsonModelTest, LoadJsonWithArrayWithNestedObject)
{
  JsonModel model;
  QByteArray json = R"([
      "apple",
      {
        "name": "Orange",
        "color": "Orange"
      },
      "cherry"
  ])";

  ASSERT_TRUE(model.loadJson(json));

  EXPECT_EQ(model.rowCount(), 1);
  EXPECT_HAS_ELEMENT(model, QModelIndex(), "array [3]");

  QModelIndex root = model.index(0, 0);
  EXPECT_EQ(model.rowCount(root), 3);
  
  EXPECT_HAS_ELEMENT(model, root, "0 : \"apple\"");
  EXPECT_HAS_ELEMENT(model, root, "1 {2}");
  EXPECT_HAS_ELEMENT(model, root, "2 : \"cherry\"");
  
  QModelIndex objectIndex = model.index(1, 0, root);
  EXPECT_EQ(model.rowCount(objectIndex), 2);
  EXPECT_HAS_ELEMENT(model, objectIndex, "name : \"Orange\"");
  EXPECT_HAS_ELEMENT(model, objectIndex, "color : \"Orange\"");

  model.clear();
  EXPECT_EQ(model.rowCount(), 0);
}

TEST(JsonModelTest, LoadJsonWithArrayWithNestedArray)
{
  JsonModel model;
  QByteArray json = R"([
    "apple",
    [
      "grape",
      "kiwi"
    ],
    "cherry"
  ])";

  ASSERT_TRUE(model.loadJson(json));

  EXPECT_EQ(model.rowCount(), 1);
  EXPECT_HAS_ELEMENT(model, QModelIndex(), "array [3]");

  QModelIndex root = model.index(0, 0);
  EXPECT_HAS_ELEMENT(model, root, "0 : \"apple\"");
  EXPECT_HAS_ELEMENT(model, root, "1 [2]");
  EXPECT_HAS_ELEMENT(model, root, "2 : \"cherry\"");

  QModelIndex nestedArrayIndex = model.index(1, 0, root);
  EXPECT_EQ(model.rowCount(nestedArrayIndex), 2);
  EXPECT_HAS_ELEMENT(model, nestedArrayIndex, "0 : \"grape\"");
  EXPECT_HAS_ELEMENT(model, nestedArrayIndex, "1 : \"kiwi\"");

  model.clear();
  EXPECT_EQ(model.rowCount(), 0);
}

TEST(JsonModelTest, LoadJsonWithPrimitiveValues)
{
  JsonModel model;
  QByteArray json = R"({
    "name": "John Doe",
    "age": 30,
    "isMarried": true,
    "height": 1.85
  })";

  ASSERT_TRUE(model.loadJson(json));

  EXPECT_EQ(model.rowCount(), 1);
  EXPECT_HAS_ELEMENT(model, QModelIndex(), "object {4}");

  QModelIndex root = model.index(0, 0);
  EXPECT_EQ(model.rowCount(root), 4);
  EXPECT_HAS_ELEMENT(model, root, "name : \"John Doe\"");
  EXPECT_HAS_ELEMENT(model, root, "age : 30");
  EXPECT_HAS_ELEMENT(model, root, "isMarried : true");
  EXPECT_HAS_ELEMENT(model, root, "height : 1.85");

  model.clear();
  EXPECT_EQ(model.rowCount(), 0);
}

TEST(JsonModelTest, LoadJsonWithComplexNestedStructure)
{
  JsonModel model;
  QByteArray json = R"({
    "name": "John Doe",
    "age": 30,
    "address": {
        "street": "123 Main St",
        "city": "Anytown",
        "zip": "12345"
    },
    "hobbies": [
        "reading",
        "hiking",
        "coding"
    ],
    "favoriteColors": [
        "red",
        "green",
        "blue"
    ],
    "isMarried": true,
    "height": 1.85,
    "family": [
      {
        "name": "Jane Doe",
        "age": 28
      },
      {
        "name": "Little John",
        "age": 5
      }
    ]
  })";

  ASSERT_TRUE(model.loadJson(json));

  EXPECT_EQ(model.rowCount(), 1);
  EXPECT_HAS_ELEMENT(model, QModelIndex(), "object {8}");
  
  QModelIndex root = model.index(0, 0);
  EXPECT_EQ(model.rowCount(root), 8);

  EXPECT_HAS_ELEMENT(model, root, "name : \"John Doe\"");
  EXPECT_HAS_ELEMENT(model, root, "age : 30");
  EXPECT_HAS_ELEMENT(model, root, "address {3}");
  EXPECT_HAS_ELEMENT(model, root, "hobbies [3]");
  EXPECT_HAS_ELEMENT(model, root, "favoriteColors [3]");
  EXPECT_HAS_ELEMENT(model, root, "isMarried : true");
  EXPECT_HAS_ELEMENT(model, root, "height : 1.85");
  EXPECT_HAS_ELEMENT(model, root, "family [2]");

  QModelIndex addressIndex = model.index(2, 0, root); 
  EXPECT_EQ(model.rowCount(addressIndex), 3);
  EXPECT_HAS_ELEMENT(model, addressIndex, "street : \"123 Main St\"");
  EXPECT_HAS_ELEMENT(model, addressIndex, "city : \"Anytown\"");
  EXPECT_HAS_ELEMENT(model, addressIndex, "zip : \"12345\"");

  QModelIndex hobbiesIndex = model.index(3, 0, root);
  EXPECT_EQ(model.rowCount(hobbiesIndex), 3);
  EXPECT_HAS_ELEMENT(model, hobbiesIndex, "0 : \"reading\"");
  EXPECT_HAS_ELEMENT(model, hobbiesIndex, "1 : \"hiking\"");
  EXPECT_HAS_ELEMENT(model, hobbiesIndex, "2 : \"coding\"");

  QModelIndex colorsIndex = model.index(4, 0, root); 
  EXPECT_EQ(model.rowCount(colorsIndex), 3);
  EXPECT_HAS_ELEMENT(model, colorsIndex, "0 : \"red\"");
  EXPECT_HAS_ELEMENT(model, colorsIndex, "1 : \"green\"");
  EXPECT_HAS_ELEMENT(model, colorsIndex, "2 : \"blue\"");

  QModelIndex familyIndex = model.index(7, 0, root);
  EXPECT_EQ(model.rowCount(familyIndex), 2);
  EXPECT_HAS_ELEMENT(model, familyIndex, "0 {2}");
  EXPECT_HAS_ELEMENT(model, familyIndex, "1 {2}");

  QModelIndex familyMember1Index = model.index(0, 0, familyIndex);
  EXPECT_EQ(model.rowCount(familyMember1Index), 2);
  EXPECT_HAS_ELEMENT(model, familyMember1Index, "name : \"Jane Doe\"");
  EXPECT_HAS_ELEMENT(model, familyMember1Index, "age : 28");

  QModelIndex familyMember2Index = model.index(1, 0, familyIndex);
  EXPECT_EQ(model.rowCount(familyMember2Index), 2);
  EXPECT_HAS_ELEMENT(model, familyMember2Index, "name : \"Little John\"");
  EXPECT_HAS_ELEMENT(model, familyMember2Index, "age : 5");

  model.clear();
  EXPECT_EQ(model.rowCount(), 0);
}
