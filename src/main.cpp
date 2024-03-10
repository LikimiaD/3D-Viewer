#include <QApplication>

#include "../view/mainwindow.h"
#include "./controller/contoller.h"
#include "./models/parserModel.h"

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);

  MainWindow w;
  Controller controller;
  ObjectModel parserModel;

  controller.setMainWindow(&w);
  controller.setGLWidget(w.getGLWidget());
  controller.setParserModel(&parserModel);
  w.setController(&controller);

  w.show();
  return a.exec();
}