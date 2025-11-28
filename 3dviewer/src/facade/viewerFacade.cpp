#include "viewerFacade.h"

#include <utility>

#include "../controllers/controller.h"
#include "../models/model.h"
#include "../utils/logger.h"
#include "../views/glWidget.h"

namespace s21 {

ViewerFacade::ViewerFacade(GLWidget* glWidget, QObject* parent)
    : QObject(parent),
      model_(std::make_unique<Model>()),
      controller_(std::make_unique<Controller>(model_.get(), this)),
      glWidget_(glWidget) {
  Logger& logger = Logger::getInstance();
  logger.debug("ViewerFacade: initialization");

  if (!glWidget_) {
    logger.warning("ViewerFacade: GLWidget is null, data will not be rendered");
  } else if (controller_) {
    controller_->setGLWidget(glWidget_);
  }

  connectControllerSignals();
}

ViewerFacade::~ViewerFacade() = default;

void ViewerFacade::connectControllerSignals() {
  if (!controller_) {
    return;
  }

  connect(controller_.get(), &Controller::updateVerticiesCountLabel, this,
          &ViewerFacade::vertexCountChanged);
  connect(controller_.get(), &Controller::updateFacesCountLabel, this,
          &ViewerFacade::facesCountChanged);
}

void ViewerFacade::loadModel(std::string filepath) {
  Logger& logger = Logger::getInstance();
  logger.info("ViewerFacade: loading model from " + filepath);

  if (controller_) {
    controller_->parseFile(std::move(filepath));
  }
}

void ViewerFacade::resetModel() {
  Logger& logger = Logger::getInstance();
  logger.info("ViewerFacade: resetting model");

  if (controller_) {
    controller_->clearData();
  }
}

void ViewerFacade::rotate(Point<int> shift) {
  Logger& logger = Logger::getInstance();
  logger.debug("ViewerFacade: applying rotation");

  if (controller_) {
    controller_->onRotateModel(shift);
  }
}

void ViewerFacade::translate(Point<float> shift) {
  Logger& logger = Logger::getInstance();
  logger.debug("ViewerFacade: applying translation");

  if (controller_) {
    controller_->onTranslateModel(shift);
  }
}

void ViewerFacade::scale(int percent) {
  Logger& logger = Logger::getInstance();
  logger.debug("ViewerFacade: scaling model to " + std::to_string(percent) +
               "%");

  if (controller_) {
    controller_->onScaleModel(percent);
  }
}

void ViewerFacade::setCameraAngles(float angleX, float angleY) {
  if (controller_) {
    controller_->setCameraAngles(angleX, angleY);
  }
}

void ViewerFacade::setCameraDistance(float distance) {
  if (controller_) {
    controller_->setCameraDistance(distance);
  }
}

}  // namespace s21
