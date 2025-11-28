#include "controller.h"

#include <memory>

using namespace s21;
using namespace std;

Controller::Controller(Model* model, QObject* parent)
    : QObject(parent), model_(model) {}

void Controller::setGLWidget(GLWidget* glWidget) { glWidget_ = glWidget; }

void Controller::parseFile(string filepath) {
  model_->parseFile(filepath);

  emit updateVerticiesCountLabel(static_cast<int>(model_->vertices().size()));
  emit updateFacesCountLabel(static_cast<int>(model_->faces().size()));

  if (!glWidget_) {
    return;
  }

  glWidget_->setModelData(model_->vertexBuffer(), model_->facesBuffer());
}

void Controller::clearData() {
  model_->clearData();
  model_->rebuildBuffers();

  emit updateVerticiesCountLabel(static_cast<int>(model_->vertices().size()));
  emit updateFacesCountLabel(static_cast<int>(model_->faces().size()));

  if (!glWidget_) {
    return;
  }

  glWidget_->setModelData(model_->vertexBuffer(), model_->facesBuffer());
}

void Controller::setCameraAngles(float xAngle, float yAngle) {
  glWidget_->setCameraAngles(xAngle, yAngle);
}

void Controller::setCameraDistance(float distance) {
  glWidget_->setCameraDistance(distance);
}

void Controller::onRotateModel(Point<int> shift) {
  applyTransform(std::make_unique<RotateStrategy>(shift));
}

void Controller::onTranslateModel(Point<float> shift) {
  applyTransform(std::make_unique<TranslateStrategy>(shift));
}

void Controller::onScaleModel(int percent) {
  applyTransform(std::make_unique<ScaleStrategy>(percent));
}

void Controller::applyTransform(std::unique_ptr<TransformStrategy> transform) {
  if (!model_ || !transform) {
    return;
  }

  transform->apply(*model_);

  if (!glWidget_) {
    return;
  }

  glWidget_->setModelData(model_->vertexBuffer(), model_->facesBuffer());
}