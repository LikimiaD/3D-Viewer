#include "parserModel.h"

#include <iostream>
void ObjectModel::loadModel(const std::string& path_) {
  path = path_;
  std::ifstream myfile(path);
  if (myfile.is_open()) {
    std::string mystring;
    while (std::getline(myfile, mystring)) {
      checkLine(mystring);
    }
  } else {
    throw std::invalid_argument("Unable to open file");
  }
}
void ObjectModel::checkLine(const std::string& line) {
  if (line.find("v ") != std::string::npos) {  // * Vertex
    parseVertex(line);
  } else if (line.find("f ") != std::string::npos) {  // * Face
    parseFace(line);
  }
}
void ObjectModel::parseVertex(const std::string& line) {
  Vertex vertex;
  sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
  vertices.push_back(vertex);
  vertexCount++;
}
void ObjectModel::parseFace(const std::string& line) {
  std::istringstream iss(line.substr(2));
  std::vector<int> edge;
  int index;
  while (iss >> index) {
    edge.push_back(index - 1);
  }
  edges.push_back(edge);
}

void ObjectModel::normalizeModel() {
  if (vertices.empty()) return;

  Vertex center{0, 0, 0};
  for (const auto& v : vertices) {
    center.x += v.x;
    center.y += v.y;
    center.z += v.z;
  }

  center.x /= vertices.size();
  center.y /= vertices.size();
  center.z /= vertices.size();

  for (auto& v : vertices) {
    v.x -= center.x;
    v.y -= center.y;
    v.z -= center.z;
  }
}
void ObjectModel::clear() {
  vertexCount = 0;
  edgeCount = 0;
  vertices.clear();
  edges.clear();
  path.clear();
}
int ObjectModel::getVertexCount() { return vertexCount; }
int ObjectModel::getEdgeCount() { return edges.size(); }
std::string ObjectModel::getPath() { return path; }
std::vector<Vertex> ObjectModel::getVertices() { return vertices; }
std::vector<std::vector<int>> ObjectModel::getEdges() { return edges; }