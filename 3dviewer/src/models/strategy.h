#pragma once

#include "model.h"

namespace s21 {
class TransformStrategy {
 public:
  virtual ~TransformStrategy() = default;
  virtual void apply(Model& model) = 0;
};

class RotateStrategy : public TransformStrategy {
 public:
  explicit RotateStrategy(Point<int> shift) : shift_(shift) {}
  void apply(Model& model) override { model.rotateModel(shift_); }

 private:
  Point<int> shift_;
};

class TranslateStrategy : public TransformStrategy {
 public:
  explicit TranslateStrategy(Point<float> shift) : shift_(shift) {}
  void apply(Model& model) override { model.translateModel(shift_); }

 private:
  Point<float> shift_;
};

class ScaleStrategy : public TransformStrategy {
 public:
  explicit ScaleStrategy(int percent) : percent_(percent) {}
  void apply(Model& model) override { model.setScalePercent(percent_); }

 private:
  int percent_;
};
}  // namespace s21