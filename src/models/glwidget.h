#ifndef MODELS_GLWIDGET_H_
#define MODELS_GLWIDGET_H_

#include <QFile>
#include <QFutureWatcher>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QProcess>
#include <QTemporaryDir>
#include <QTimer>
#include <QWheelEvent>
#include <QtConcurrent/QtConcurrent>
#include <cmath>

#define GL_GLEXT_PROTOTYPES
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>

#include "parserModel.h"

class Coordinate {
 public:
  float x = 0;
  float y = 0;
  float z = 0;
};

class DrawMode {
 public:
  bool vertexMode = true;
  bool edgeMode = false;
  bool menuMode = true;
  float vertexSize = 1.0f;
  float edgeSize = 1.0f;
  bool roundVertex = false;
  bool stippleLine = false;
  bool parallelProjection = false;
};

class Camera {
 public:
  float xRot = 0;
  float yRot = 0;
  float zoomFactor = 1;
  int scroll_speed = 1;
  Coordinate movePos;
};

class Color {
 public:
  Color(float r, float g, float b) : r(r), g(g), b(b) {}
  ~Color() = default;

  float r = 0;
  float g = 0;
  float b = 0;
};

class GLProperties {
 private:
  QString filename = "settings.json";

 public:
  Camera camera;
  DrawMode drawMode;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> edges;
  Color edgeColor = Color(1.0f, 1.0f, 1.0f);
  Color vertexColor = Color(1.0f, 0.0f, 0.0f);
  Color fillColor = Color(0.0f, 0.0f, 1.0f);
  Color backgroundColor = Color(0.0f, 0.0f, 0.0f);
  QPoint mousePos;
  QOpenGLShaderProgram m_program;
  std::string path;

  void saveToJson();
  void loadFromJson();
  void clear();
};

class GLWidget : public QOpenGLWidget,
                 protected QOpenGLFunctions,
                 public GLProperties {
  Q_OBJECT
  QTimer screenshotTimer;
  QTemporaryDir tempDir;
  int frameCount = 0;
  QString gifFileName;

 public:
  GLWidget(QWidget *parent = nullptr);
  ~GLWidget();

  void setProperties(ObjectModel *ptr);
  void setScroolSpeed(int speed);
  void setEdgeSize(int size);
  void setVertexSize(int size);
  void setEdgeColor(QColor color);
  void setVertexColor(QColor color);
  void setBackgroundColor(QColor color);
  void screenshot(QString fileName);
  int getRotateValue();
  int getEdgeValue();
  int getVertexValue();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 private:
  void initShaders();
  void updateProjectionMatrix();
  void drawMenu();
  void drawObject();
  void cameraInit();
  void captureFrame();
  QImage fastGrab();
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

  QFutureWatcher<void> futureWatcher;
};

#endif /* MODELS_GLWIDGET_H_ */
