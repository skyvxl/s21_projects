#pragma once

#include <QPoint>
#include <QString>
#include <vector>

namespace s21 {
constexpr int BASE_SCALE = 2;

struct Extremums {
  float maxX, minX, maxY, minY, maxZ, minZ;
};

template <typename T>
struct Point {
  Point() = delete;

  Point(T x, T y, T z) : x_(x), y_(y), z_(z){};

  T x_, y_, z_;
};

class Model {
 public:
  Model() = default;

  void parseFile(std::string filepath);

  const std::vector<Point<float>>& vertices() const { return vertices_; }
  const std::vector<Point<int>>& faces() const { return faces_; }
  const std::vector<float>& vertexBuffer() const { return vertexBuffer_; }
  const std::vector<int>& facesBuffer() const { return facesBuffer_; }

  void clearData();
  void rebuildBuffers();
  void rotateModel(Point<int> shift);
  void translateModel(Point<float> shift);
  void setScalePercent(int percent);

 private:
  void normalizeModel();
  void centerModel();
  void scaleModel(double scale);
  void findExtremums();

  std::vector<Point<float>> vertices_;
  std::vector<Point<int>> faces_;
  std::vector<float> vertexBuffer_;
  std::vector<int> facesBuffer_;

  Extremums extremums_;
  double currentScale_ = 1.0;
};
}  // namespace s21