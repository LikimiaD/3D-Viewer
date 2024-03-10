#include "mainwindow.h"

#include <iostream>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  loadStyle();
  connect(ui->load_button, &QPushButton::clicked, this,
          [this]() { openFile(); });
  connect(ui->rotate_slider, &QSlider::valueChanged, this,
          [this](int value) { controller->setScrollSpeed(value); });
  connect(ui->edge_slider, &QSlider::valueChanged, this,
          [this](int value) { controller->setEdgeSize(value); });
  connect(ui->vertex_slider, &QSlider::valueChanged, this,
          [this](int value) { controller->setVertexSize(value); });
  connect(ui->edge_color, &QPushButton::clicked, this,
          [this]() { edgeColorPicker(); });
  connect(ui->vertex_color, &QPushButton::clicked, this,
          [this]() { vertexColorPicker(); });
  connect(ui->background_color, &QPushButton::clicked, this,
          [this]() { backgroundColorPicker(); });
  connect(ui->save_button, &QPushButton::clicked, this,
          [this]() { screenshot(); });
}

MainWindow::~MainWindow() { delete ui; }

QString MainWindow::getStyleSheet() {
  QFile file(":/css/style.qss");
  QString styleSheet;
  if (file.open(QFile::ReadOnly)) {
    styleSheet = QString::fromUtf8(file.readAll());
  } else {
    qDebug() << "Failed to open .qss file";
  }
  return styleSheet;
}

void MainWindow::loadStyle() {
  QString styleSheet = getStyleSheet();
  this->setStyleSheet(getStyleSheet());
  ui->app_qwidget->setObjectName("app_background");
  ui->edge_counter->setObjectName("app_label");
  ui->vertex_counter->setObjectName("app_label");

  ui->rotate_label->setObjectName("type_label");
  ui->rotate_slider->setObjectName("horizontal");

  ui->vertex_label->setObjectName("type_label");
  ui->vertex_slider->setObjectName("horizontal");

  ui->edge_label->setObjectName("type_label");
  ui->edge_slider->setObjectName("horizontal");

  ui->save_button->setObjectName("button");
  ui->load_button->setObjectName("button");
  ui->edge_color->setObjectName("button");
  ui->vertex_color->setObjectName("button");
  ui->background_color->setObjectName("button");
}

void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open File"), QDir::currentPath(), tr("Text Files (*.obj)"));
  if (!fileName.isEmpty()) {
    controller->parseFile(fileName);
  }
  setSliderValue();
  enableButtons();
}

void MainWindow::setEdgesLabel(int count) {
  ui->edge_counter->setText("Edges: " + QString::number(count));
}
void MainWindow::setVertexLabel(int count) {
  ui->vertex_counter->setText("Vertex: " + QString::number(count));
}
GLWidget* MainWindow::getGLWidget() { return ui->openGLWidget; }
void MainWindow::edgeColorPicker() {
  QColor color =
      QColorDialog::getColor(Qt::white, this, "Choose new Edge color");
  if (color.isValid()) {
    ui->openGLWidget->setEdgeColor(color);
  }
}
void MainWindow::vertexColorPicker() {
  QColor color =
      QColorDialog::getColor(Qt::white, this, "Choose new Vertex color");
  if (color.isValid()) {
    ui->openGLWidget->setVertexColor(color);
  }
}
void MainWindow::backgroundColorPicker() {
  QColor color =
      QColorDialog::getColor(Qt::white, this, "Choose new Background color");
  if (color.isValid()) {
    ui->openGLWidget->setBackgroundColor(color);
  }
}
void MainWindow::screenshot() {
  QString filter = "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp);;GIF (*.gif)";
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Screenshot"),
                                                  QDir::homePath(), filter);
  controller->screenshotRedirection(fileName);
}
void MainWindow::setSliderValue() {
  ui->rotate_slider->setValue(controller->getRotateValue());
  ui->edge_slider->setValue(controller->getEdgeValue());
  ui->vertex_slider->setValue(controller->getVertexValue());
}
void MainWindow::enableButtons() {
  ui->edge_color->setEnabled(true);
  ui->vertex_color->setEnabled(true);
  ui->background_color->setEnabled(true);
  ui->save_button->setEnabled(true);
  ui->rotate_slider->setEnabled(true);
  ui->edge_slider->setEnabled(true);
  ui->vertex_slider->setEnabled(true);
}