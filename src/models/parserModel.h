#ifndef MODELS_PARSER_MODEL_H_
#define MODELS_PARSER_MODEL_H_

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

class Vertex {
 public:
  Vertex() = default;
  Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
  ~Vertex() = default;

  float x = 0;
  float y = 0;
  float z = 0;
};

class PropertiesObject {
 public:
  int vertexCount = 0;
  int edgeCount = 0;
  std::vector<Vertex> vertices;
  std::vector<std::vector<int>> edges;
  std::string path;
};

class ObjectModel : protected PropertiesObject {
 public:
  void loadModel(const std::string& path);
  void normalizeModel();
  int getVertexCount();
  int getEdgeCount();
  std::string getPath();
  std::vector<Vertex> getVertices();
  std::vector<std::vector<int>> getEdges();

  void clear();

 private:
  void checkLine(const std::string& line);
  void parseVertex(const std::string& line);
  void parseFace(const std::string& line);
};

#endif /* MODELS_PARSER_MODEL_H_ */
