#include "mainWindow.h"

#include <QIntValidator>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QSlider>
#include <QWheelEvent>
#include <numbers>

#include "../facade/viewerFacade.h"
#include "glWidget.h"

using namespace s21;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setupUI();
  setupSlots();

  show();
}

MainWindow::~MainWindow() {
  if (viewerFacade_) {
    delete viewerFacade_;
    viewerFacade_ = nullptr;
  }
}

void MainWindow::setupSlots() {
  connect(loadFileButton_, &QPushButton::clicked, this, &MainWindow::loadFile);
  connect(resetModelButton_, &QPushButton::clicked, this,
          &MainWindow::resetModel);

  connect(this, &MainWindow::parseFile, viewerFacade_,
          &ViewerFacade::loadModel);
  connect(this, &MainWindow::clearData, viewerFacade_,
          &ViewerFacade::resetModel);

  connect(this, &MainWindow::rotateModel, viewerFacade_, &ViewerFacade::rotate);
  connect(this, &MainWindow::translateModel, viewerFacade_,
          &ViewerFacade::translate);
  connect(this, &MainWindow::scaleModel, viewerFacade_, &ViewerFacade::scale);
  connect(viewerFacade_, &ViewerFacade::vertexCountChanged, this,
          &MainWindow::updateVerteciesLabel);
  connect(viewerFacade_, &ViewerFacade::facesCountChanged, this,
          &MainWindow::updateFacesLabel);

  connect(this, &MainWindow::resetTransformUI, this,
          &MainWindow::resetUIControls);
}

void MainWindow::setupUI() {
  setupMainWindow();  // 1. Базовые параметры окна
  setupCentralLayout();  // 2. Центральный контейнер + общий layout
  setupGLWidget();            // 3. OpenGL виджет
  setupControlPanel();        // 4. Панель управления
  setupTransformControls();   // 5. Блок Move / Rotate
  applyControlPanelStyles();  // 6. Стили
  assembleLayout();           // 7. Сборка интерфейса
}

void MainWindow::setupMainWindow() {
  setWindowTitle("CPP4_3D_VIEWER");
  resize(1080, 720);
  setMinimumSize(1080, 720);
  setMaximumSize(1080, 720);
}

void MainWindow::setupCentralLayout() {
  centralContainer_ = new QWidget(this);
  setCentralWidget(centralContainer_);

  mainLayout_ = new QHBoxLayout();
  mainLayout_->setContentsMargins(0, 0, 0, 0);
  mainLayout_->setSpacing(0);
  centralContainer_->setLayout(mainLayout_);
}

void MainWindow::setupGLWidget() {
  glWidget_ = new GLWidget(centralContainer_);
  glWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  glWidget_->installEventFilter(this);
  glWidget_->setCursor(Qt::OpenHandCursor);

  viewerFacade_ = new ViewerFacade(glWidget_, this);
  viewerFacade_->setCameraAngles(cameraAngleX_, cameraAngleY_);
  viewerFacade_->setCameraDistance(cameraDistance_);
}

void MainWindow::setupControlPanel() {
  controlPanel_ = new QWidget(centralContainer_);
  controlPanel_->setObjectName("controlPanel");
  controlPanel_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

  loadFileButton_ = new QPushButton("Load file");
  fileNameLabel_ = new QLabel("No file loaded");
  vertexCountLabel_ = new QLabel("Vertecies: 0");
  facesCountLabel_ = new QLabel("Faces: 0");
  resetModelButton_ = new QPushButton("Reset");

  auto* controlLayout = new QVBoxLayout(controlPanel_);
  controlLayout->setContentsMargins(24, 24, 24, 24);
  controlLayout->addStretch(1);

  controlLayout->addWidget(loadFileButton_);
  controlLayout->addWidget(fileNameLabel_);

  auto* countsContainer = new QWidget(controlPanel_);
  auto* countsLayout = new QHBoxLayout(countsContainer);
  countsLayout->setContentsMargins(0, 0, 0, 0);
  countsLayout->setSpacing(8);
  countsLayout->addWidget(vertexCountLabel_, 1);
  countsLayout->addWidget(facesCountLabel_, 1);
  controlLayout->addWidget(countsContainer);

  controlLayout->addWidget(resetModelButton_);

  transformGroup_ = new QGroupBox("Transform");
  transformLayout_ = new QVBoxLayout(transformGroup_);
  controlLayout->addWidget(transformGroup_);
}

void MainWindow::setupTransformControls() {
  QTabWidget* transformTabs = new QTabWidget(this);
  QWidget* moveTab = new QWidget(transformTabs);
  QWidget* rotateTab = new QWidget(transformTabs);
  transformTabs->addTab(moveTab, "Move");
  transformTabs->addTab(rotateTab, "Rotate");

  setupAxisSliders(moveTab, -10, 10, moveInputs_);
  setupAxisSliders(rotateTab, -180, 180, rotateInputs_);

  transformLayout_->addWidget(transformTabs);

  QWidget* scaleContainer = new QWidget(transformGroup_);
  auto* scaleLayout = new QHBoxLayout(scaleContainer);
  scaleLayout->setContentsMargins(0, 12, 0, 0);

  QLabel* scaleLabel = new QLabel("Scale");
  scaleLabel->setStyleSheet(
      "QLabel { color: #333; font-size: 16px; font-weight: 600; }");
  scaleSlider_ = new QSlider(Qt::Horizontal);
  scaleSlider_->setRange(1, 200);
  scaleSlider_->setValue(100);
  scaleSlider_->setCursor(Qt::PointingHandCursor);

  scaleValueLabel_ = new QLabel("100%");
  scaleValueLabel_->setFixedWidth(45);
  scaleValueLabel_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

  scaleLayout->addWidget(scaleLabel);
  scaleLayout->addWidget(scaleSlider_);
  scaleLayout->addWidget(scaleValueLabel_);

  transformLayout_->addWidget(scaleContainer);

  connect(scaleSlider_, &QSlider::valueChanged, this,
          &MainWindow::onScaleSliderChanged);
}

void MainWindow::setupAxisSliders(QWidget* parent, int min, int max,
                                  QLineEdit* inputs[3]) {
  auto* layout = new QGridLayout(parent);
  QStringList axisLabels = {"X", "Y", "Z"};
  bool isMoveTab = (inputs == moveInputs_);

  for (int i = 0; i < 3; ++i) {
    QLabel* axisLabel = new QLabel(axisLabels[i]);
    QLineEdit* axisInput = new QLineEdit();
    axisInput->setFixedWidth(70);

    QSlider* axisSlider = new QSlider(Qt::Horizontal);
    if (isMoveTab) {
      axisSlider->setRange(min * 10, max * 10);
    } else {
      axisSlider->setRange(min, max);
    }
    axisSlider->setValue(0);

    if (isMoveTab) {
      axisInput->setValidator(new QDoubleValidator(min, max, 2, axisInput));
      axisInput->setText(QString::number(axisSlider->value() / 10.0));
    } else {
      axisInput->setValidator(new QIntValidator(min, max, axisInput));
      axisInput->setText(QString::number(axisSlider->value()));
    }

    inputs[i] = axisInput;

    layout->addWidget(axisLabel, i, 0);
    layout->addWidget(axisInput, i, 1);
    layout->addWidget(axisSlider, i, 2);

    connect(axisSlider, &QSlider::valueChanged,
            [axisInput, isMoveTab](int value) {
              if (isMoveTab) {
                axisInput->setText(QString::number(value / 10.0, 'f', 2));
              } else {
                axisInput->setText(QString::number(value));
              }
            });

    connect(axisInput, &QLineEdit::textChanged,
            [axisSlider, isMoveTab](const QString& text) {
              bool ok;
              if (isMoveTab) {
                double val = text.toDouble(&ok);
                if (ok) axisSlider->setValue(static_cast<int>(val * 10.0));
              } else {
                int val = text.toInt(&ok);
                if (ok) axisSlider->setValue(val);
              }
            });

    if (isMoveTab) {
      connect(axisSlider, &QSlider::valueChanged, this, [this, i]() {
        bool ok;
        float current = moveInputs_[i]->text().toFloat(&ok);
        float delta = 0.0f;
        if (ok) {
          delta = current - prevMove_[i];
          prevMove_[i] = current;
        }
        // Только если есть дельта
        if (ok && std::abs(delta) > 1e-6) {
          float x = (i == 0) ? delta : 0.0f;
          float y = (i == 1) ? delta : 0.0f;
          float z = (i == 2) ? delta : 0.0f;
          emit translateModel({x, y, z});
        }
      });
    } else {
      connect(axisSlider, &QSlider::valueChanged, this, [this, i]() {
        bool ok;
        int current = rotateInputs_[i]->text().toInt(&ok);
        int delta = 0;
        if (ok) {
          delta = current - prevRotate_[i];
          prevRotate_[i] = current;
        }
        // Только если есть дельта
        if (ok && std::abs(delta) > 0) {
          int x = (i == 0) ? delta : 0;
          int y = (i == 1) ? delta : 0;
          int z = (i == 2) ? delta : 0;
          emit rotateModel({x, y, z});
        }
      });
    }
  }
}

void MainWindow::applyControlPanelStyles() {
  loadFileButton_->setStyleSheet(
      "QPushButton { background-color: #4CAF50; color: white; border: none; "
      "padding: 10px 20px; border-radius: 8px; font-weight: 600; font-size: "
      "14px; }"
      "QPushButton:hover { background-color: #45a049; }");

  const char* infoLabelStyle =
      "QLabel { background-color: #ffffff; color: #333333; border: 1px solid "
      "#d4d8de; padding: 8px 12px; border-radius: 6px; font-size: 14px; }";

  vertexCountLabel_->setStyleSheet(infoLabelStyle);
  facesCountLabel_->setStyleSheet(infoLabelStyle);

  fileNameLabel_->setStyleSheet(
      "QLabel { background-color: #ffffff; color: #333333; border: 1px solid "
      "#d4d8de; padding: 8px 12px; border-radius: 6px; font-size: 14px; }");

  fileNameLabel_->setAlignment(Qt::AlignCenter);
  vertexCountLabel_->setAlignment(Qt::AlignCenter);
  facesCountLabel_->setAlignment(Qt::AlignCenter);

  resetModelButton_->setStyleSheet(
      "QPushButton { background-color: #0B63FF; color: white; border: none; "
      "padding: 10px 20px; border-radius: 8px; font-weight: 600; font-size: "
      "14px; }"
      "QPushButton:hover { background-color: #0B63F0; }");

  controlPanel_->setStyleSheet(
      "#controlPanel { background-color: #f4f5f7; border-left: 1px solid "
      "#d4d8de; }");

  transformGroup_->setStyleSheet(
      "QGroupBox { font-weight: 600; font-size: 14px; border: 1px solid "
      "#d4d8de; "
      "border-radius: 8px; margin-top: 10px; padding: 12px; background-color: "
      "#ffffff; }"
      "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top "
      "center; padding: 0 8px; color: #222; font-size: 20px; font-weight: "
      "bold; }");

  QString tabWidgetStyle =
      "QTabWidget::pane { border: 1px solid #d4d8de; border-radius: 6px; "
      "background-color: #ffffff; padding: 8px; }"
      "QTabBar::tab { background-color: #e8eaed; color: #333333; padding: 8px "
      "16px; "
      "border: 1px solid #d4d8de; border-bottom: none; border-top-left-radius: "
      "6px; "
      "border-top-right-radius: 6px; font-size: 13px; }"
      "QTabBar::tab:selected { background-color: #ffffff; color: #0B63FF; "
      "font-weight: 600; }"
      "QTabBar::tab:hover { background-color: #f0f2f5; }";

  QString sliderStyle =
      "QSlider::groove:horizontal { border: 1px solid #d4d8de; height: 6px; "
      "background-color: #e8eaed; border-radius: 3px; }"
      "QSlider::handle:horizontal { background-color: #0B63FF; border: 2px "
      "solid #0B63FF; "
      "width: 16px; height: 16px; margin: -6px 0; border-radius: 8px; }"
      "QSlider::handle:horizontal:hover { background-color: #0851d9; "
      "border-color: #0851d9; }";

  QString inputStyle =
      "QLineEdit { background-color: #ffffff; color: #333333; border: 1px "
      "solid #d4d8de; "
      "padding: 6px 10px; border-radius: 6px; font-size: 13px; }"
      "QLineEdit:focus { border-color: #0B63FF; }";

  QString labelStyle =
      "QLabel { color: #333333; font-size: 13px; font-weight: 500; }";

  if (transformGroup_) {
    QTabWidget* tabWidget = transformGroup_->findChild<QTabWidget*>();
    if (tabWidget) {
      tabWidget->setStyleSheet(tabWidgetStyle);

      for (int i = 0; i < tabWidget->count(); ++i) {
        QWidget* tab = tabWidget->widget(i);
        QList<QSlider*> sliders = tab->findChildren<QSlider*>();
        for (QSlider* slider : sliders) {
          slider->setStyleSheet(sliderStyle);
        }
        QList<QLineEdit*> inputs = tab->findChildren<QLineEdit*>();
        for (QLineEdit* input : inputs) {
          input->setStyleSheet(inputStyle);
        }
        QList<QLabel*> labels = tab->findChildren<QLabel*>();
        for (QLabel* label : labels) {
          label->setStyleSheet(labelStyle);
        }
      }
    }

    if (scaleSlider_) {
      scaleSlider_->setStyleSheet(sliderStyle);
    }
    if (scaleValueLabel_) {
      scaleValueLabel_->setStyleSheet(labelStyle);
    }
  }

  loadFileButton_->setCursor(Qt::PointingHandCursor);
  resetModelButton_->setCursor(Qt::PointingHandCursor);
}

void MainWindow::assembleLayout() {
  mainLayout_->addWidget(glWidget_, 7);
  mainLayout_->addWidget(controlPanel_, 3);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  QMainWindow::resizeEvent(event);
  // Вывод размера окна при изменении (для отладки)
  // QSize newSize = event->size();
  // qDebug() << "Window resized to:" << newSize;
}

void MainWindow::loadFile() {
  QString filepath;
  filepath = QFileDialog::getOpenFileName(this, "Выбрать файл", "..",
                                          "All Objects Files (*.obj)");

  QFileInfo fileInfo(filepath);
  QString fileName = fileInfo.fileName();
  if (!fileName.isEmpty()) fileNameLabel_->setText(fileName);

  emit resetTransformUI();
  emit parseFile(filepath.toStdString());
}

void MainWindow::resetModel() {
  fileNameLabel_->setText("No file loaded");
  emit resetTransformUI();
  emit clearData();
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
  if (watched == glWidget_) {
    switch (event->type()) {
      case QEvent::MouseButtonPress: {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
          isRotatingCamera_ = true;
          lastMousePosition_ = mouseEvent->pos();
          glWidget_->setCursor(Qt::ClosedHandCursor);
          glWidget_->setFocus();
          return true;
        }
        break;
      }
      case QEvent::MouseMove: {
        if (isRotatingCamera_) {
          auto* mouseEvent = static_cast<QMouseEvent*>(event);
          const QPoint delta = mouseEvent->pos() - lastMousePosition_;
          lastMousePosition_ = mouseEvent->pos();

          cameraAngleX_ += static_cast<float>(delta.x()) * cameraSensitivity_;
          cameraAngleY_ += static_cast<float>(delta.y()) * cameraSensitivity_;

          const float pitchLimit =
              std::numbers::pi_v<float> * 0.5f - 0.01f;  // избегаем переворота
          cameraAngleY_ = std::clamp(cameraAngleY_, -pitchLimit, pitchLimit);

          setCameraAngles();
          return true;
        }
        break;
      }
      case QEvent::MouseButtonRelease: {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton && isRotatingCamera_) {
          isRotatingCamera_ = false;
          glWidget_->setCursor(Qt::OpenHandCursor);
          return true;
        }
        break;
      }
      case QEvent::Leave: {
        if (isRotatingCamera_) {
          isRotatingCamera_ = false;
          glWidget_->setCursor(Qt::OpenHandCursor);
        }
        break;
      }
      case QEvent::Wheel: {
        auto* wheelEvent = static_cast<QWheelEvent*>(event);
        const int delta = wheelEvent->angleDelta().y();
        if (delta != 0) {
          const float steps = static_cast<float>(delta) / 120.0f;
          const float factor = 1.0f - steps * zoomSensitivity_;
          cameraDistance_ *= factor;
          cameraDistance_ = std::max(cameraDistance_, cameraDistanceMin_);
          updateCameraDistance();
        }
        wheelEvent->accept();
        return true;
      }
      default:
        break;
    }
  }

  return QMainWindow::eventFilter(watched, event);
}

void MainWindow::setCameraAngles() {
  if (viewerFacade_) {
    viewerFacade_->setCameraAngles(cameraAngleX_, cameraAngleY_);
  }
}

void MainWindow::updateCameraDistance() {
  if (viewerFacade_) {
    viewerFacade_->setCameraDistance(cameraDistance_);
  }
}

void MainWindow::onScaleSliderChanged(int value) {
  if (!scaleValueLabel_) {
    return;
  }

  scaleValueLabel_->setText(QString::number(value) + "%");
  emit scaleModel(value);
}

void MainWindow::updateVerteciesLabel(int vertecies) {
  if (!vertexCountLabel_) {
    return;
  }

  vertexCountLabel_->setText("Vertecies: " + QString::number(vertecies));
}

void MainWindow::updateFacesLabel(int faces) {
  if (!facesCountLabel_) {
    return;
  }

  facesCountLabel_->setText("Faces: " + QString::number(faces));
}

void MainWindow::resetUIControls() {
  for (int i = 0; i < 3; ++i) {
    if (rotateInputs_[i]) {
      rotateInputs_[i]->setText("0");
    }
    if (moveInputs_[i]) {
      moveInputs_[i]->setText("0");
    }
    prevRotate_[i] = 0;
    prevMove_[i] = 0.0f;
  }

  if (scaleSlider_) {
    scaleSlider_->setValue(100);
  }
  if (scaleValueLabel_) {
    scaleValueLabel_->setText("100%");
  }
}