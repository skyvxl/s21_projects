#include <qlabel.h>

#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>
#include <QResizeEvent>
#include <QSlider>
#include <QWidget>

#include "../models/model.h"

namespace s21 {

class GLWidget;
class ViewerFacade;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

 protected:
  void resizeEvent(QResizeEvent* event) override;
  bool eventFilter(QObject* watched, QEvent* event) override;

 signals:
  void parseFile(std::string filepath);
  void clearData();
  void rotateModel(Point<int> shift);
  void translateModel(Point<float> shift);
  void scaleModel(int percent);
  void resetTransformUI();

 private slots:
  void loadFile();
  void resetModel();
  void setCameraAngles();
  void updateCameraDistance();
  void onScaleSliderChanged(int value);
  void updateVerteciesLabel(int verticies);
  void updateFacesLabel(int faces);
  void resetUIControls();

 private:
  void setupUI();
  void setupSlots();
  void setupMainWindow();
  void setupCentralLayout();
  void setupGLWidget();
  void setupControlPanel();
  void setupAxisSliders(QWidget* parent, int min, int max,
                        QLineEdit* inputs[3]);
  void setupTransformControls();
  void applyControlPanelStyles();
  void assembleLayout();

  QLineEdit* rotateInputs_[3] = {nullptr, nullptr, nullptr};
  QLineEdit* moveInputs_[3] = {nullptr, nullptr, nullptr};

  // Для хранения предыдущих значений слайдеров
  int prevRotate_[3] = {0, 0, 0};
  float prevMove_[3] = {0.0f, 0.0f, 0.0f};

  QSlider* scaleSlider_ = nullptr;
  QLabel* scaleValueLabel_ = nullptr;
  QWidget* centralContainer_ = nullptr;
  QHBoxLayout* mainLayout_ = nullptr;
  GLWidget* glWidget_ = nullptr;
  QWidget* controlPanel_ = nullptr;
  QGroupBox* transformGroup_ = nullptr;
  QVBoxLayout* transformLayout_ = nullptr;

  QPushButton* loadFileButton_ = nullptr;
  QLabel* fileNameLabel_ = nullptr;
  QLabel* vertexCountLabel_ = nullptr;
  QLabel* facesCountLabel_ = nullptr;
  QPushButton* resetModelButton_ = nullptr;

  ViewerFacade* viewerFacade_ = nullptr;

  bool isRotatingCamera_ = false;
  QPoint lastMousePosition_;
  float cameraAngleX_ = 0.785398163f;  // ~45°
  float cameraAngleY_ = 0.615479708f;  // ~35.264°
  const float cameraSensitivity_ = 0.005f;
  float cameraDistance_ = 4.33012724f;  // 2.5 * sqrt(3)
  const float cameraDistanceMin_ = 0.5f;
  const float zoomSensitivity_ = 0.1f;
};

}  // namespace s21