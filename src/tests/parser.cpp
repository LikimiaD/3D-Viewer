#include <gtest/gtest.h>

#include <filesystem>

#include "../models/parserModel.h"

TEST(ParserModel, LoadModel) {
  std::string currentPath = std::filesystem::current_path();
  std::string fullPath = currentPath + "/tests" + "/cube.obj";

  std::cout << fullPath << std::endl;

  ObjectModel model;
  model.loadModel(fullPath);
  ASSERT_EQ(model.getPath(), fullPath);
  ASSERT_NO_THROW(model.getVertices());
  ASSERT_NO_THROW(model.getEdges());
}
TEST(ParserModel, LoadModelException) {
  std::string currentPath = std::filesystem::current_path();
  std::string fullPath = currentPath + "/tests" + "/cube1.obj";

  ObjectModel model;
  ASSERT_THROW(model.loadModel(fullPath), std::invalid_argument);
}
TEST(ParserModel, NormalizeModel) {
  std::string currentPath = std::filesystem::current_path();
  std::string fullPath = currentPath + "/tests" + "/cube.obj";

  ObjectModel model;
  model.loadModel(fullPath);
  ASSERT_NO_THROW(model.normalizeModel());
  auto vertices = model.getVertices();
  for (const auto& vertex : vertices) {
    ASSERT_GE(vertex.x, -0.5);
    ASSERT_LE(vertex.x, 0.5);
    ASSERT_GE(vertex.y, -0.5);
    ASSERT_LE(vertex.y, 0.5);
    ASSERT_GE(vertex.z, -0.5);
    ASSERT_LE(vertex.z, 0.5);
  }
}
TEST(ParserModel, Clear) {
  std::string currentPath = std::filesystem::current_path();
  std::string fullPath = currentPath + "/tests" + "/cube.obj";

  ObjectModel model;
  model.loadModel(fullPath);
  model.clear();
  ASSERT_EQ(model.getVertexCount(), 0);
  ASSERT_EQ(model.getEdgeCount(), 0);
  ASSERT_EQ(model.getPath(), "");
  ASSERT_TRUE(model.getVertices().empty());
  ASSERT_TRUE(model.getEdges().empty());
}