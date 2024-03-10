#include "glwidget.h"

#include <iostream>

void GLProperties::saveToJson() {
  QJsonObject rootObj;

  QJsonObject cameraObj;
  cameraObj["xRot"] = camera.xRot;
  cameraObj["yRot"] = camera.yRot;
  cameraObj["zoomFactor"] = camera.zoomFactor;
  cameraObj["scroll_speed"] = camera.scroll_speed;
  cameraObj["movePos"] = QJsonObject{{"x", camera.movePos.x},
                                     {"y", camera.movePos.y},
                                     {"z", camera.movePos.z}};
  rootObj["camera"] = cameraObj;

  QJsonObject drawModeObj;
  drawModeObj["vertexMode"] = drawMode.vertexMode;
  drawModeObj["edgeMode"] = drawMode.edgeMode;
  drawModeObj["menuMode"] = drawMode.menuMode;
  drawModeObj["vertexSize"] = drawMode.vertexSize;
  drawModeObj["edgeSize"] = drawMode.edgeSize;
  drawModeObj["roundVertex"] = drawMode.roundVertex;
  drawModeObj["stippleLine"] = drawMode.stippleLine;
  drawModeObj["parallelProjection"] = drawMode.parallelProjection;
  rootObj["drawMode"] = drawModeObj;

  QJsonObject edgeColorObj{
      {"r", edgeColor.r}, {"g", edgeColor.g}, {"b", edgeColor.b}};
  rootObj["edgeColor"] = edgeColorObj;
  QJsonObject vertexColorObj{
      {"r", vertexColor.r}, {"g", vertexColor.g}, {"b", vertexColor.b}};
  rootObj["vertexColor"] = vertexColorObj;
  QJsonObject fillColorObj{
      {"r", fillColor.r}, {"g", fillColor.g}, {"b", fillColor.b}};
  rootObj["fillColor"] = fillColorObj;
  QJsonObject backgroundColorObj{{"r", backgroundColor.r},
                                 {"g", backgroundColor.g},
                                 {"b", backgroundColor.b}};
  rootObj["backgroundColor"] = backgroundColorObj;

  QJsonDocument doc(rootObj);
  QFile file(filename);
  if (file.open(QIODevice::WriteOnly)) {
    file.write(doc.toJson());
    file.close();
  }
}

void GLProperties::loadFromJson() {
  QFile file(filename);
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject rootObj = doc.object();

    QJsonObject cameraObj = rootObj["camera"].toObject();
    camera.xRot = cameraObj["xRot"].toDouble();
    camera.yRot = cameraObj["yRot"].toDouble();
    camera.zoomFactor = cameraObj["zoomFactor"].toDouble();
    camera.scroll_speed = cameraObj["scroll_speed"].toInt();
    QJsonObject movePosObj = cameraObj["movePos"].toObject();
    camera.movePos.x = movePosObj["x"].toDouble();
    camera.movePos.y = movePosObj["y"].toDouble();
    camera.movePos.z = movePosObj["z"].toDouble();

    QJsonObject drawModeObj = rootObj["drawMode"].toObject();
    drawMode.vertexMode = drawModeObj["vertexMode"].toBool();
    drawMode.edgeMode = drawModeObj["edgeMode"].toBool();
    drawMode.menuMode = drawModeObj["menuMode"].toBool();
    drawMode.vertexSize = drawModeObj["vertexSize"].toDouble();
    drawMode.edgeSize = drawModeObj["edgeSize"].toDouble();
    drawMode.roundVertex = drawModeObj["roundVertex"].toBool();
    drawMode.stippleLine = drawModeObj["stippleLine"].toBool();
    drawMode.parallelProjection = drawModeObj["parallelProjection"].toBool();

    // Цвета
    edgeColor = Color(rootObj["edgeColor"].toObject()["r"].toDouble(),
                      rootObj["edgeColor"].toObject()["g"].toDouble(),
                      rootObj["edgeColor"].toObject()["b"].toDouble());
    vertexColor = Color(rootObj["vertexColor"].toObject()["r"].toDouble(),
                        rootObj["vertexColor"].toObject()["g"].toDouble(),
                        rootObj["vertexColor"].toObject()["b"].toDouble());
    fillColor = Color(rootObj["fillColor"].toObject()["r"].toDouble(),
                      rootObj["fillColor"].toObject()["g"].toDouble(),
                      rootObj["fillColor"].toObject()["b"].toDouble());
    backgroundColor =
        Color(rootObj["backgroundColor"].toObject()["r"].toDouble(),
              rootObj["backgroundColor"].toObject()["g"].toDouble(),
              rootObj["backgroundColor"].toObject()["b"].toDouble());
    file.close();
  }
}
void GLProperties::clear() {
  edges.clear();
  vertices.clear();
}
GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  connect(&screenshotTimer, &QTimer::timeout, this, &GLWidget::captureFrame);
}
GLWidget::~GLWidget() {}
void GLWidget::setProperties(ObjectModel *properties) {
  clear();
  vertices = properties->getVertices();
  std::vector<std::vector<int>> edges2D = properties->getEdges();
  path = properties->getPath();
  for (const auto &edge : edges2D) {
    for (int index : edge) {
      edges.push_back(static_cast<unsigned int>(index));
    }
  }

  updateProjectionMatrix();
  update();
}
void GLWidget::updateProjectionMatrix() {
  if (vertices.empty()) return;

  float minX = vertices[0].x;
  float maxX = minX;
  float minY = vertices[0].y;
  float maxY = minY;
  float minZ = vertices[0].z;
  float maxZ = minZ;

  for (const Vertex &vertex : vertices) {
    if (vertex.x < minX) minX = vertex.x;
    if (vertex.x > maxX) maxX = vertex.x;
    if (vertex.y < minY) minY = vertex.y;
    if (vertex.y > maxY) maxY = vertex.y;
    if (vertex.z < minZ) minZ = vertex.z;
    if (vertex.z > maxZ) maxZ = vertex.z;
  }

  float aspectRatio = float(width()) / float(height());
  float centerX = (minX + maxX) / 2;
  float centerY = (minY + maxY) / 2;
  float centerZ = (minZ + maxZ) / 2;

  float rangeX = maxX - minX;
  float rangeY = maxY - minY;
  float rangeZ = maxZ - minZ;

  float maxRange = std::max({rangeX, rangeY, rangeZ});

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (drawMode.parallelProjection) {
    gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
  } else {
    if (aspectRatio >= 1.0) {
      glOrtho(centerX - maxRange * aspectRatio,
              centerX + maxRange * aspectRatio, centerY - maxRange,
              centerY + maxRange, centerZ - maxRange * 2,
              centerZ + maxRange * 2);
    } else {
      glOrtho(centerX - maxRange, centerX + maxRange,
              centerY - maxRange / aspectRatio,
              centerY + maxRange / aspectRatio, centerZ - maxRange * 2,
              centerZ + maxRange * 2);
    }
  }
  glMatrixMode(GL_MODELVIEW);
}
void GLWidget::initializeGL() {
  loadFromJson();
  initializeOpenGLFunctions();
  glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
  glEnable(GL_DEPTH_TEST);
  initShaders();
}
void GLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  updateProjectionMatrix();
}
void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (drawMode.menuMode) {
    drawMenu();
  }
  cameraInit();
  drawObject();
}
void GLWidget::drawObject() {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices.data());

  if (drawMode.vertexMode) {
    if (drawMode.roundVertex) {
      glEnable(GL_POINT_SMOOTH);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
      glPointSize(drawMode.vertexSize);
      glDrawArrays(GL_POINTS, 0, vertices.size());
      glDisable(GL_POINT_SMOOTH);
    } else {
      glColor3f(vertexColor.r, vertexColor.g, vertexColor.b);
      glPointSize(drawMode.vertexSize);
      glDrawArrays(GL_POINTS, 0, vertices.size());
    }
  }

  if (drawMode.edgeMode) {
    if (drawMode.stippleLine) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x00FF);
      glColor3f(edgeColor.r, edgeColor.g, edgeColor.b);
      glLineWidth((GLfloat)drawMode.edgeSize);
      glDrawElements(GL_LINES, static_cast<GLsizei>(edges.size()),
                     GL_UNSIGNED_INT, edges.data());
      glDisable(GL_LINE_STIPPLE);
    } else {
      glColor3f(edgeColor.r, edgeColor.g, edgeColor.b);
      glLineWidth((GLfloat)drawMode.edgeSize);
      glDrawElements(GL_LINES, static_cast<GLsizei>(edges.size()),
                     GL_UNSIGNED_INT, edges.data());
    }
  }

  glDisableClientState(GL_VERTEX_ARRAY);
}
void GLWidget::mousePressEvent(QMouseEvent *event) {  // Крутить мышку
  mousePos = event->pos();
}
void GLWidget::mouseMoveEvent(QMouseEvent *event) {
  camera.xRot = camera.scroll_speed / M_PI * (mousePos.y() - event->y());
  camera.yRot = camera.scroll_speed / M_PI * (mousePos.x() - event->x());
  update();
  saveToJson();
}
void GLWidget::wheelEvent(QWheelEvent *event) {  // Колесико мышки
  int numDegrees = event->angleDelta().y() / 8;
  int numSteps = numDegrees / 15;

  camera.zoomFactor += numSteps * 0.1;
  if (camera.zoomFactor < 0.1) camera.zoomFactor = 0.1;

  update();
  saveToJson();
}
void GLWidget::keyPressEvent(QKeyEvent *event) {  // Нажать кнопку на клаве
  switch (event->key()) {
    case Qt::Key_1:
      drawMode.vertexMode = !drawMode.vertexMode;
      break;
    case Qt::Key_2:
      drawMode.edgeMode = !drawMode.edgeMode;
      break;
    case Qt::Key_3:
      drawMode.menuMode = !drawMode.menuMode;
      break;
    case Qt::Key_W:
      camera.movePos.z += 0.1;
      break;
    case Qt::Key_S:
      camera.movePos.z -= 0.1;
      break;
    case Qt::Key_A:
      camera.movePos.x += 0.1;
      break;
    case Qt::Key_D:
      camera.movePos.x -= 0.1;
      break;
    case Qt::Key_Q:
      camera.yRot += 5;
      break;
    case Qt::Key_E:
      camera.yRot -= 5;
      break;
    case Qt::Key_Z:
      drawMode.roundVertex = !drawMode.roundVertex;
      break;
    case Qt::Key_X:
      drawMode.stippleLine = !drawMode.stippleLine;
      break;
    case Qt::Key_C:
      drawMode.parallelProjection = !drawMode.parallelProjection;
      updateProjectionMatrix();
      break;
    default:
      break;
  }
  saveToJson();
  update();
}
void GLWidget::initShaders() {
  if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                         ":/shaders/vshader.vsh")) {
    close();
  }
  if (!m_program.link()) {
    close();
  }
}
void GLWidget::setEdgeSize(int size) {
  drawMode.edgeSize = (float)size;
  saveToJson();
  update();
}
void GLWidget::setVertexSize(int size) {
  drawMode.vertexSize = (float)size;
  saveToJson();
  update();
}
void GLWidget::drawMenu() {
  QPainter painter(this);

  painter.setPen(Qt::green);
  painter.setFont(QFont("Arial", 10));
  painter.drawText(5, 50, "W -> Forward");
  painter.drawText(5, 65, "S -> Backward");
  painter.drawText(5, 80, "A -> Left");
  painter.drawText(5, 95, "D -> Right");
  painter.drawText(5, 110, "Q -> Rotate left");
  painter.drawText(5, 125, "E -> Rotate right");
  painter.drawText(5, 140, "1 -> Vertex mode");
  painter.drawText(5, 155, "2 -> Edge mode");
  painter.drawText(5, 170, "3 -> Off text-help");
  painter.drawText(5, 185, "Scroll -> Zoom");
  painter.drawText(5, 200, "Mouse -> Rotate");
  painter.drawText(5, 215, "Z -> Change Vertex type(square/round)");
  painter.drawText(5, 230, "X -> Change Edge type(stipple/normal)");
  painter.drawText(5, 245, "C -> Change projection type");

  painter.drawText(5, height() - 40, "Path: " + QString::fromStdString(path));

  painter.end();
}
void GLWidget::setScroolSpeed(int speed) {
  camera.scroll_speed = speed;
  saveToJson();
}
void GLWidget::cameraInit() {
  glTranslatef(camera.movePos.x, camera.movePos.y, camera.movePos.z);
  glRotatef(camera.xRot, 1.0, 0.0, 0.0);
  glRotatef(camera.yRot, 0.0, 1.0, 0.0);
  glScalef(camera.zoomFactor, camera.zoomFactor, camera.zoomFactor);
}
void GLWidget::setEdgeColor(QColor color) {
  edgeColor.r = color.redF();
  edgeColor.g = color.greenF();
  edgeColor.b = color.blueF();
  saveToJson();
  update();
}
void GLWidget::setVertexColor(QColor color) {
  vertexColor.r = color.redF();
  vertexColor.g = color.greenF();
  vertexColor.b = color.blueF();
  saveToJson();
  update();
}
void GLWidget::setBackgroundColor(QColor color) {
  backgroundColor.r = color.redF();
  backgroundColor.g = color.greenF();
  backgroundColor.b = color.blueF();
  glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
  saveToJson();
  update();
}
void GLWidget::screenshot(QString fileName) {
  if (fileName.isEmpty()) {
    qDebug() << "File name is empty. Screenshot not saved.";
    return;
  }
  std::cout << fileName.toStdString() << std::endl;

  if (fileName.endsWith(".gif", Qt::CaseInsensitive)) {
    gifFileName = fileName;
    frameCount = 0;
    screenshotTimer.start(100);
  } else {
    QImage image = fastGrab();
    bool saved = image.save(fileName);
    if (!saved) {
      qDebug() << "Failed to save screenshot to" << fileName;
    }
  }
}
void GLWidget::captureFrame() {
  if (frameCount >= 50) {
    screenshotTimer.stop();
    qDebug() << "Stoped capturing frames for GIF...";
    auto future = QtConcurrent::run([=]() {
      QStringList arguments;
      arguments << "-delay"
                << "10"
                << "-loop"
                << "0"
                << "frame*.png" << gifFileName;
      QProcess::execute("convert", arguments);
    });
    futureWatcher.setFuture(future);
    return;
  }

  QImage image = fastGrab();
  QString frameFileName =
      tempDir.path() +
      QString("/frame%1.png").arg(frameCount++, 5, 10, QChar('0'));
  image.save(frameFileName);

  if (frameCount == 1) {
    qDebug() << "Start capturing frames for GIF...";
  }
}
QImage GLWidget::fastGrab() {
  drawMode.menuMode = false;
  update();
  QImage image = grabFramebuffer();
  drawMode.menuMode = true;
  update();
  return image;
}
int GLWidget::getEdgeValue() {
  if (drawMode.edgeSize == 0.0f) loadFromJson();
  return (int)drawMode.edgeSize;
}
int GLWidget::getVertexValue() {
  if (drawMode.vertexSize == 0.0f) loadFromJson();
  return (int)drawMode.vertexSize;
}
int GLWidget::getRotateValue() {
  if (camera.scroll_speed == 0) loadFromJson();
  return (int)camera.scroll_speed;
}