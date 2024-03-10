#include <iostream>

#include "../models/parserModel.h"
#include "../view/mainwindow.h"
#include "contoller.h"

void Controller::parseFile(QString fileName) {
  if (parserModel && mainWindow && glWidget) {
    parserModel->clear();
    parserModel->loadModel(fileName.toStdString());
    parserModel->normalizeModel();
    mainWindow->setEdgesLabel(parserModel->getEdgeCount());
    mainWindow->setVertexLabel(parserModel->getVertexCount());
    glWidget->setProperties(parserModel);
  }
  if (!parserModel) {
    qDebug() << "Parser model is NULL";
  }
  if (!mainWindow) {
    qDebug() << "Main window is NULL";
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
}
void Controller::setScrollSpeed(int speed) {
  if (glWidget) {
    glWidget->setScroolSpeed(speed);
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
}
void Controller::setEdgeSize(int size) {
  if (glWidget) {
    glWidget->setEdgeSize(size);
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
}
void Controller::setVertexSize(int size) {
  if (glWidget) {
    glWidget->setVertexSize(size);
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
}
void Controller::setEdgeColor(QColor color) {
  if (glWidget) {
    glWidget->setEdgeColor(color);
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
}
void Controller::setVertexColor(QColor color) {
  if (glWidget) {
    glWidget->setVertexColor(color);
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
}
void Controller::setBackgroundColor(QColor color) {
  if (glWidget) {
    glWidget->setBackgroundColor(color);
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
}
void Controller::screenshotRedirection(QString fileName) {
  if (glWidget) {
    glWidget->screenshot(fileName);
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
}
int Controller::getRotateValue() {
  if (glWidget) {
    return glWidget->getRotateValue();
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
  return 0;
}
int Controller::getEdgeValue() {
  if (glWidget) {
    return glWidget->getEdgeValue();
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
  return 0;
}
int Controller::getVertexValue() {
  if (glWidget) {
    return glWidget->getVertexValue();
  }
  if (!glWidget) {
    qDebug() << "GL widget is NULL";
  }
  return 0;
}