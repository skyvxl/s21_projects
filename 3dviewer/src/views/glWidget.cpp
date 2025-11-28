#include "glWidget.h"

#include <QDebug>
#include <QFile>
#include <QMatrix4x4>
#include <QVector3D>
#include <algorithm>
#include <cmath>
#include <numbers>

using namespace s21;

//// PUBLIC

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent), vertexBuffer_(QOpenGLBuffer::VertexBuffer) {}

GLWidget::~GLWidget() {
  makeCurrent();
  if (vertexArrayObject_.isCreated()) {
    vertexArrayObject_.destroy();
  }
  if (vertexBuffer_.isCreated()) {
    vertexBuffer_.destroy();
  }
  shaderProgram_.removeAllShaders();
  doneCurrent();
}

void GLWidget::setCameraAngles(float angleX, float angleY) {
  cameraAngleX_ = angleX;
  cameraAngleY_ = angleY;
  update();
}

void GLWidget::setCameraDistance(float distance) {
  constexpr float kMinDistance = 0.1f;
  cameraDistance_ = std::max(distance, kMinDistance);
  update();
}

void GLWidget::setModelData(const std::vector<float>& vertices,
                            const std::vector<int>& faces) {
  modelVertices_ = vertices;
  modelFaces_ = faces;

  if (!isValid()) {
    update();
    return;
  }

  makeCurrent();
  updateGeometryBuffers();
  doneCurrent();
  update();
}

//// PROTECTED

void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  const QString vertexShaderSource =
      readResourceFile(QStringLiteral(":/shaders/triangle.vert"));
  const QString fragmentShaderSource =
      readResourceFile(QStringLiteral(":/shaders/triangle.frag"));

  if (vertexShaderSource.isEmpty() || fragmentShaderSource.isEmpty()) {
    qWarning() << "Shader source is empty; initialization aborted";
    return;
  }

  if (!shaderProgram_.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                              vertexShaderSource)) {
    qWarning() << "Vertex shader compilation failed" << shaderProgram_.log();
  }

  if (!shaderProgram_.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                              fragmentShaderSource)) {
    qWarning() << "Fragment shader compilation failed" << shaderProgram_.log();
  }

  shaderProgram_.bindAttributeLocation("position", 0);
  shaderProgram_.bindAttributeLocation("color", 1);
  shaderProgram_.bindAttributeLocation("normal", 2);

  if (!shaderProgram_.link()) {
    qWarning() << "Shader program link failed" << shaderProgram_.log();
  }

  setupGeometry();
}

void GLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void GLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!shaderProgram_.isLinked() || !vertexArrayObject_.isCreated() ||
      vertexCount_ == 0) {
    return;
  }

  shaderProgram_.bind();
  vertexArrayObject_.bind();

  const float aspect =
      height() > 0 ? static_cast<float>(width()) / height() : 1.0f;

  QMatrix4x4 projection;
  projection.perspective(45.0f, aspect, 0.1f, 100.0f);

  QMatrix4x4 view;
  QVector3D cameraPos(
      cameraDistance_ * std::cos(cameraAngleY_) * std::cos(cameraAngleX_),
      cameraDistance_ * std::sin(cameraAngleY_),
      cameraDistance_ * std::cos(cameraAngleY_) * std::sin(cameraAngleX_));
  view.lookAt(cameraPos, QVector3D(0.0f, 0.0f, 0.0f),
              QVector3D(0.0f, 1.0f, 0.0f));

  QMatrix4x4 model;
  model.setToIdentity();

  const QMatrix4x4 mvp = projection * view * model;
  shaderProgram_.setUniformValue("u_mvp", mvp);
  shaderProgram_.setUniformValue("u_model", model);

  QMatrix3x3 normalMatrix = model.normalMatrix();
  shaderProgram_.setUniformValue("u_normalMatrix", normalMatrix);

  shaderProgram_.setUniformValue("u_lightPos", QVector3D(5.0f, 5.0f, 5.0f));
  shaderProgram_.setUniformValue("u_lightPosSecondary",
                                 QVector3D(-4.0f, -3.0f, -4.0f));
  shaderProgram_.setUniformValue("u_viewPos", cameraPos);

  glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
  vertexArrayObject_.release();
  shaderProgram_.release();
}

//// PRIVATE

void GLWidget::setupGeometry() {
  if (!shaderProgram_.isLinked()) {
    return;
  }

  updateGeometryBuffers();
}

void GLWidget::updateGeometryBuffers() {
  vertexData_.clear();
  appendAxesGeometry();
  appendModelGeometry();

  if (vertexData_.empty()) {
    vertexCount_ = 0;
    return;
  }

  if (!vertexArrayObject_.isCreated() && !vertexArrayObject_.create()) {
    qWarning() << "Failed to create VAO";
    return;
  }
  QOpenGLVertexArrayObject::Binder vaoBinder(&vertexArrayObject_);

  if (!vertexBuffer_.isCreated() && !vertexBuffer_.create()) {
    qWarning() << "Failed to create VBO";
    return;
  }

  if (!vertexBuffer_.bind()) {
    qWarning() << "Failed to bind VBO";
    return;
  }

  vertexBuffer_.setUsagePattern(QOpenGLBuffer::DynamicDraw);
  vertexBuffer_.allocate(vertexData_.data(),
                         static_cast<int>(vertexData_.size() * sizeof(float)));

  vertexCount_ = static_cast<int>(vertexData_.size() / 9);

  if (!shaderProgram_.bind()) {
    qWarning() << "Failed to bind shader program";
    vertexBuffer_.release();
    return;
  }

  const int stride = 9 * static_cast<int>(sizeof(float));
  shaderProgram_.enableAttributeArray(0);
  shaderProgram_.setAttributeBuffer(0, GL_FLOAT, 0, 3, stride);

  shaderProgram_.enableAttributeArray(1);
  shaderProgram_.setAttributeBuffer(
      1, GL_FLOAT, 3 * static_cast<int>(sizeof(float)), 3, stride);

  shaderProgram_.enableAttributeArray(2);
  shaderProgram_.setAttributeBuffer(
      2, GL_FLOAT, 6 * static_cast<int>(sizeof(float)), 3, stride);

  shaderProgram_.release();

  vertexBuffer_.release();
}

void GLWidget::appendAxesGeometry() {
  constexpr int kSegments = 32;
  constexpr float kRadius = 0.03f;
  constexpr float kLength = 1.2f;
  const float twoPi = 2.0f * std::numbers::pi_v<float>;

  constexpr int kAxes = 3;
  constexpr int kTrianglesPerSegment = 2;
  constexpr int kVerticesPerTriangle = 3;
  constexpr int kFloatsPerVertex = 9;
  vertexData_.reserve(vertexData_.size() +
                      kAxes * kSegments * kTrianglesPerSegment *
                          kVerticesPerTriangle * kFloatsPerVertex);

  auto pushVertex = [this](const QVector3D& position, const QVector3D& color,
                           const QVector3D& normal) {
    QVector3D safeNormal = normal;
    if (!safeNormal.isNull()) {
      safeNormal.normalize();
    }
    vertexData_.push_back(position.x());
    vertexData_.push_back(position.y());
    vertexData_.push_back(position.z());
    vertexData_.push_back(color.x());
    vertexData_.push_back(color.y());
    vertexData_.push_back(color.z());
    vertexData_.push_back(safeNormal.x());
    vertexData_.push_back(safeNormal.y());
    vertexData_.push_back(safeNormal.z());
  };

  auto appendTriangle = [&](const QVector3D& a, const QVector3D& b,
                            const QVector3D& c, const QVector3D& na,
                            const QVector3D& nb, const QVector3D& nc,
                            const QVector3D& color) {
    pushVertex(a, color, na);
    pushVertex(b, color, nb);
    pushVertex(c, color, nc);
  };

  auto buildXAxis = [&](const QVector3D& color) {
    for (int i = 0; i < kSegments; ++i) {
      const float angle = (twoPi * i) / kSegments;
      const float nextAngle = (twoPi * (i + 1)) / kSegments;

      const QVector3D base1(0.0f, kRadius * std::cos(angle),
                            kRadius * std::sin(angle));
      const QVector3D base2(0.0f, kRadius * std::cos(nextAngle),
                            kRadius * std::sin(nextAngle));

      const QVector3D tip1(kLength, base1.y(), base1.z());
      const QVector3D tip2(kLength, base2.y(), base2.z());

      const QVector3D normalBase1(0.0f, base1.y(), base1.z());
      const QVector3D normalBase2(0.0f, base2.y(), base2.z());
      const QVector3D normalTip1(0.0f, tip1.y(), tip1.z());
      const QVector3D normalTip2(0.0f, tip2.y(), tip2.z());

      appendTriangle(base1, base2, tip2, normalBase1, normalBase2, normalTip2,
                     color);
      appendTriangle(base1, tip2, tip1, normalBase1, normalTip2, normalTip1,
                     color);
    }
  };

  auto buildYAxis = [&](const QVector3D& color) {
    for (int i = 0; i < kSegments; ++i) {
      const float angle = (twoPi * i) / kSegments;
      const float nextAngle = (twoPi * (i + 1)) / kSegments;

      const QVector3D base1(kRadius * std::cos(angle), 0.0f,
                            kRadius * std::sin(angle));
      const QVector3D base2(kRadius * std::cos(nextAngle), 0.0f,
                            kRadius * std::sin(nextAngle));

      const QVector3D tip1(base1.x(), kLength, base1.z());
      const QVector3D tip2(base2.x(), kLength, base2.z());

      const QVector3D normalBase1(base1.x(), 0.0f, base1.z());
      const QVector3D normalBase2(base2.x(), 0.0f, base2.z());
      const QVector3D normalTip1(tip1.x(), 0.0f, tip1.z());
      const QVector3D normalTip2(tip2.x(), 0.0f, tip2.z());

      appendTriangle(base1, base2, tip2, normalBase1, normalBase2, normalTip2,
                     color);
      appendTriangle(base1, tip2, tip1, normalBase1, normalTip2, normalTip1,
                     color);
    }
  };

  auto buildZAxis = [&](const QVector3D& color) {
    for (int i = 0; i < kSegments; ++i) {
      const float angle = (twoPi * i) / kSegments;
      const float nextAngle = (twoPi * (i + 1)) / kSegments;

      const QVector3D base1(kRadius * std::cos(angle),
                            kRadius * std::sin(angle), 0.0f);
      const QVector3D base2(kRadius * std::cos(nextAngle),
                            kRadius * std::sin(nextAngle), 0.0f);

      const QVector3D tip1(base1.x(), base1.y(), kLength);
      const QVector3D tip2(base2.x(), base2.y(), kLength);

      const QVector3D normalBase1(base1.x(), base1.y(), 0.0f);
      const QVector3D normalBase2(base2.x(), base2.y(), 0.0f);
      const QVector3D normalTip1(tip1.x(), tip1.y(), 0.0f);
      const QVector3D normalTip2(tip2.x(), tip2.y(), 0.0f);

      appendTriangle(base1, base2, tip2, normalBase1, normalBase2, normalTip2,
                     color);
      appendTriangle(base1, tip2, tip1, normalBase1, normalTip2, normalTip1,
                     color);
    }
  };

  buildXAxis(QVector3D(1.0f, 0.0f, 0.0f));
  buildYAxis(QVector3D(0.0f, 1.0f, 0.0f));
  buildZAxis(QVector3D(0.0f, 0.0f, 1.0f));
}

void GLWidget::appendModelGeometry() {
  if (modelVertices_.empty() || modelFaces_.empty()) {
    return;
  }

  const QVector3D color(0.8f, 0.8f, 0.8f);
  const std::size_t vertexCount = modelVertices_.size() / 3;

  const std::size_t triangles = modelFaces_.size() / 3;
  vertexData_.reserve(vertexData_.size() + triangles * 3 * 9);

  auto appendTriangle = [this](const QVector3D& a, const QVector3D& b,
                               const QVector3D& c, const QVector3D& color) {
    QVector3D edge1 = b - a;
    QVector3D edge2 = c - a;
    QVector3D normal = QVector3D::crossProduct(edge1, edge2).normalized();

    const QVector3D vertices[]{a, b, c};
    for (const auto& v : vertices) {
      vertexData_.push_back(v.x());
      vertexData_.push_back(v.y());
      vertexData_.push_back(v.z());
      vertexData_.push_back(color.x());
      vertexData_.push_back(color.y());
      vertexData_.push_back(color.z());
      vertexData_.push_back(normal.x());
      vertexData_.push_back(normal.y());
      vertexData_.push_back(normal.z());
    }
  };

  for (std::size_t i = 0; i < triangles; ++i) {
    const int ia = modelFaces_[3 * i];
    const int ib = modelFaces_[3 * i + 1];
    const int ic = modelFaces_[3 * i + 2];

    if (ia < 0 || ib < 0 || ic < 0) {
      continue;
    }

    if (static_cast<std::size_t>(ia) >= vertexCount ||
        static_cast<std::size_t>(ib) >= vertexCount ||
        static_cast<std::size_t>(ic) >= vertexCount) {
      continue;
    }

    const QVector3D a(modelVertices_[3 * ia], modelVertices_[3 * ia + 1],
                      modelVertices_[3 * ia + 2]);
    const QVector3D b(modelVertices_[3 * ib], modelVertices_[3 * ib + 1],
                      modelVertices_[3 * ib + 2]);
    const QVector3D c(modelVertices_[3 * ic], modelVertices_[3 * ic + 1],
                      modelVertices_[3 * ic + 2]);

    appendTriangle(a, b, c, color);
  }
}

QString GLWidget::readResourceFile(const QString& resourcePath) const {
  QFile file(resourcePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Failed to open resource" << resourcePath
               << file.errorString();
    return {};
  }

  const QByteArray data = file.readAll();
  return QString::fromUtf8(data);
}