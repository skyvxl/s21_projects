#pragma once

#include <qtmetamacros.h>

#include <QObject>
#include <memory>

#include "../models/model.h"
#include "../models/strategy.h"
#include "../views/glWidget.h"

namespace s21 {
class Controller : public QObject {
  Q_OBJECT
 public:
  explicit Controller(Model* model, QObject* parent = nullptr);

  void setGLWidget(GLWidget* glWidget);

 signals:
  void updateVerticiesCountLabel(int vetricies);
  void updateFacesCountLabel(int faces);

 public slots:
  void parseFile(std::string filepath);
  void clearData();
  void onRotateModel(Point<int> shift);
  void onTranslateModel(Point<float> shift);
  void onScaleModel(int percent);

  // GlWidget slots
  void setCameraAngles(float angleX, float angleY);
  void setCameraDistance(float distance);

 private:
  // паттерн стратегия
  void applyTransform(std::unique_ptr<TransformStrategy> transform);

  Model* model_;
  GLWidget* glWidget_ = nullptr;
};

}  // namespace s21