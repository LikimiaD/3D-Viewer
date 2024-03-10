#ifndef CONTROLLER_CONTOLLER_H_
#define CONTROLLER_CONTOLLER_H_

#include <QDebug>

#include "../models/glwidget.h"
#include "../models/parserModel.h"

class MainWindow;

class Controller {
 public:
  Controller() = default;
  ~Controller() = default;

  void parseFile(QString fileName);

  void setParserModel(ObjectModel *parserModel) {
    this->parserModel = parserModel;
  }
  void setGLWidget(GLWidget *glWidget) { this->glWidget = glWidget; }
  void setMainWindow(MainWindow *mainWindow) { this->mainWindow = mainWindow; }
  void setScrollSpeed(int speed);
  void setEdgeSize(int size);
  void setVertexSize(int size);
  void setEdgeColor(QColor color);
  void setVertexColor(QColor color);
  void setBackgroundColor(QColor color);
  void screenshotRedirection(QString fileName);
  int getRotateValue();
  int getEdgeValue();
  int getVertexValue();

 private:
  ObjectModel *parserModel = NULL;
  GLWidget *glWidget = NULL;
  MainWindow *mainWindow = NULL;
};

#endif /* CONTROLLER_CONTOLLER_H_ */
