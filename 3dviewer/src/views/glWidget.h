#ifndef __GLWIDGET_H__
#define __GLWIDGET_H__

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QString>
#include <vector>

namespace s21 {

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit GLWidget(QWidget* parent = nullptr);
  ~GLWidget() override;

  void setCameraAngles(float angleX, float angleY);
  void setCameraDistance(float distance);
  void setModelData(const std::vector<float>& vertices,
                    const std::vector<int>& faces);

 protected:
  void initializeGL() override;  // Вызывается один раз при создании виджета

  void resizeGL(int w, int h) override;  // Вызывается при изменении размера

  void paintGL() override;  // Вызывается каждый раз,
                            // когда нужно перерисовать виджет

 private:
  void setupGeometry();
  void updateGeometryBuffers();
  void appendAxesGeometry();
  void appendModelGeometry();
  QString readResourceFile(const QString& resourcePath) const;

  QOpenGLShaderProgram shaderProgram_;
  QOpenGLBuffer vertexBuffer_;
  QOpenGLVertexArrayObject vertexArrayObject_;
  std::vector<float> vertexData_;
  int vertexCount_ = 0;

  std::vector<float> modelVertices_;
  std::vector<int> modelFaces_;

  // Параметры камеры
  float cameraAngleX_ = 0.785398163f;   // ~45°
  float cameraAngleY_ = 0.615479708f;   // ~35.264°
  float cameraDistance_ = 4.33012724f;  // 2.5 * sqrt(3)
};

}  // namespace s21

#endif  // __GLWIDGET_H__