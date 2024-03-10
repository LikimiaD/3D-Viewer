#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QPalette>
#include <QString>

#include "../controller/contoller.h"
#include "../models/glwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void loadStyle();

  GLWidget *getGLWidget();

  void setController(Controller *controller) { this->controller = controller; }
  void setEdgesLabel(int count);
  void setVertexLabel(int count);

 private:
  Ui::MainWindow *ui;
  GLWidget *glWidget;
  Controller *controller;

  QString getStyleSheet();
  void openFile();
  void edgeColorPicker();
  void vertexColorPicker();
  void backgroundColorPicker();
  void screenshot();
  void setSliderValue();
  void enableButtons();
};
#endif  // MAINWINDOW_H
