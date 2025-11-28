#pragma once

#include <QObject>
#include <memory>
#include <string>

namespace s21 {
template <typename T>
struct Point;

class Model;
class Controller;
class GLWidget;

class ViewerFacade : public QObject {
  Q_OBJECT

 public:
  explicit ViewerFacade(GLWidget* glWidget, QObject* parent = nullptr);
  ~ViewerFacade();

 public slots:
  void loadModel(std::string filepath);
  void resetModel();
  void rotate(Point<int> shift);
  void translate(Point<float> shift);
  void scale(int percent);
  void setCameraAngles(float angleX, float angleY);
  void setCameraDistance(float distance);

 signals:
  void vertexCountChanged(int vertecies);
  void facesCountChanged(int faces);

 private:
  void connectControllerSignals();

  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
  GLWidget* glWidget_ = nullptr;
};

}  // namespace s21
