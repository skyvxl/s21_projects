#include <gtest/gtest.h>

#include <QApplication>
#include <cmath>
#include <fstream>
#include <memory>

#include "../controllers/controller.h"
#include "../models/strategy.h"
#include "../utils/logger.h"

using namespace s21;

// ============================================================================
// Model Parsing Tests
// ============================================================================

class ModelParseTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
  }

  void TearDown() override {
    model_.reset();
    controller_.reset();
  }

  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
};

TEST_F(ModelParseTest, ParseCubeFile) {
  controller_->parseFile("../obj/cube.obj");

  EXPECT_EQ(model_->vertices().size(), 8);
  EXPECT_EQ(model_->faces().size(), 12);
  EXPECT_FALSE(model_->vertices().empty());
  EXPECT_FALSE(model_->faces().empty());
}

TEST_F(ModelParseTest, ParseCubeNormalizedValue) {
  controller_->parseFile("../obj/cube.obj");
  // base scale для куба 0.9999995231628418
  EXPECT_DOUBLE_EQ(model_->vertices()[0].x_, 0.9999995231628418);
}

TEST_F(ModelParseTest, ParseMonkey2File) {
  controller_->parseFile("../obj/monkey2.obj");

  EXPECT_EQ(model_->vertices().size(), 7958);
  EXPECT_EQ(model_->faces().size(), 15744);
}

TEST_F(ModelParseTest, ParseTeapotFile) {
  controller_->parseFile("../obj/teapot.obj");

  EXPECT_GT(model_->vertices().size(), 0);
  EXPECT_GT(model_->faces().size(), 0);
}

TEST_F(ModelParseTest, ParseNonExistentFile) {
  controller_->parseFile("../obj/nonexistent.obj");

  EXPECT_EQ(model_->vertices().size(), 0);
  EXPECT_EQ(model_->faces().size(), 0);
}

TEST_F(ModelParseTest, BuffersMatchVerticesAndFaces) {
  controller_->parseFile("../obj/cube.obj");

  EXPECT_EQ(model_->vertexBuffer().size(), model_->vertices().size() * 3);
  EXPECT_EQ(model_->facesBuffer().size(), model_->faces().size() * 3);
}

TEST_F(ModelParseTest, VertexBufferContainsCorrectData) {
  controller_->parseFile("../obj/cube.obj");

  const auto& vertices = model_->vertices();
  const auto& buffer = model_->vertexBuffer();

  for (size_t i = 0; i < vertices.size(); ++i) {
    EXPECT_FLOAT_EQ(buffer[i * 3 + 0], vertices[i].x_);
    EXPECT_FLOAT_EQ(buffer[i * 3 + 1], vertices[i].y_);
    EXPECT_FLOAT_EQ(buffer[i * 3 + 2], vertices[i].z_);
  }
}

TEST_F(ModelParseTest, FacesBufferContainsCorrectData) {
  controller_->parseFile("../obj/cube.obj");

  const auto& faces = model_->faces();
  const auto& buffer = model_->facesBuffer();

  for (size_t i = 0; i < faces.size(); ++i) {
    EXPECT_EQ(buffer[i * 3 + 0], faces[i].x_);
    EXPECT_EQ(buffer[i * 3 + 1], faces[i].y_);
    EXPECT_EQ(buffer[i * 3 + 2], faces[i].z_);
  }
}

TEST_F(ModelParseTest, ParseMultipleFilesSequentially) {
  controller_->parseFile("../obj/cube.obj");
  size_t cubeVertices = model_->vertices().size();
  size_t cubeFaces = model_->faces().size();

  EXPECT_EQ(cubeVertices, 8);
  EXPECT_EQ(cubeFaces, 12);

  controller_->parseFile("../obj/monkey2.obj");
  size_t monkeyVertices = model_->vertices().size();
  size_t monkeyFaces = model_->faces().size();

  EXPECT_EQ(monkeyVertices, 7958);
  EXPECT_EQ(monkeyFaces, 15744);
  EXPECT_NE(cubeVertices, monkeyVertices);
}

// ============================================================================
// Model Transform Tests - Rotation
// ============================================================================

class ModelRotateTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
    controller_->parseFile("../obj/cube.obj");
  }

  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
};

TEST_F(ModelRotateTest, RotateAroundXAxis) {
  const auto originalVertices = model_->vertices();

  controller_->onRotateModel({90, 0, 0});

  const auto& rotatedVertices = model_->vertices();
  EXPECT_EQ(rotatedVertices.size(), originalVertices.size());

  // После поворота на 90° вокруг X: Y -> Z, Z -> -Y
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(rotatedVertices[i].x_, originalVertices[i].x_, 1e-5);
  }
}

TEST_F(ModelRotateTest, RotateAroundYAxis) {
  const auto originalVertices = model_->vertices();

  controller_->onRotateModel({0, 90, 0});

  const auto& rotatedVertices = model_->vertices();
  EXPECT_EQ(rotatedVertices.size(), originalVertices.size());

  // После поворота на 90° вокруг Y: X -> -Z, Z -> X
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(rotatedVertices[i].y_, originalVertices[i].y_, 1e-5);
  }
}

TEST_F(ModelRotateTest, RotateAroundZAxis) {
  const auto originalVertices = model_->vertices();

  controller_->onRotateModel({0, 0, 90});

  const auto& rotatedVertices = model_->vertices();
  EXPECT_EQ(rotatedVertices.size(), originalVertices.size());

  // После поворота на 90° вокруг Z: X -> -Y, Y -> X
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(rotatedVertices[i].z_, originalVertices[i].z_, 1e-5);
  }
}

TEST_F(ModelRotateTest, Rotate360DegreesReturnsToOriginal) {
  const auto originalVertices = model_->vertices();

  controller_->onRotateModel({360, 0, 0});

  const auto& rotatedVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(rotatedVertices[i].x_, originalVertices[i].x_, 1e-4);
    EXPECT_NEAR(rotatedVertices[i].y_, originalVertices[i].y_, 1e-4);
    EXPECT_NEAR(rotatedVertices[i].z_, originalVertices[i].z_, 1e-4);
  }
}

TEST_F(ModelRotateTest, RotateMultipleAxes) {
  const auto originalVertices = model_->vertices();

  controller_->onRotateModel({45, 45, 45});

  const auto& rotatedVertices = model_->vertices();
  EXPECT_EQ(rotatedVertices.size(), originalVertices.size());

  bool hasChanges = false;
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    if (std::abs(rotatedVertices[i].x_ - originalVertices[i].x_) > 1e-5 ||
        std::abs(rotatedVertices[i].y_ - originalVertices[i].y_) > 1e-5 ||
        std::abs(rotatedVertices[i].z_ - originalVertices[i].z_) > 1e-5) {
      hasChanges = true;
      break;
    }
  }
  EXPECT_TRUE(hasChanges);
}

TEST_F(ModelRotateTest, RotateZeroDegrees) {
  const auto originalVertices = model_->vertices();

  controller_->onRotateModel({0, 0, 0});

  const auto& rotatedVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_FLOAT_EQ(rotatedVertices[i].x_, originalVertices[i].x_);
    EXPECT_FLOAT_EQ(rotatedVertices[i].y_, originalVertices[i].y_);
    EXPECT_FLOAT_EQ(rotatedVertices[i].z_, originalVertices[i].z_);
  }
}

TEST_F(ModelRotateTest, RotateNegativeAngles) {
  const auto originalVertices = model_->vertices();

  controller_->onRotateModel({-90, 0, 0});

  const auto& rotatedVertices = model_->vertices();
  EXPECT_EQ(rotatedVertices.size(), originalVertices.size());

  bool hasChanges = false;
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    if (std::abs(rotatedVertices[i].y_ - originalVertices[i].y_) > 1e-5 ||
        std::abs(rotatedVertices[i].z_ - originalVertices[i].z_) > 1e-5) {
      hasChanges = true;
      break;
    }
  }
  EXPECT_TRUE(hasChanges);
}

// ============================================================================
// Model Transform Tests - Translation
// ============================================================================

class ModelTranslateTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
    controller_->parseFile("../obj/cube.obj");
  }

  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
};

TEST_F(ModelTranslateTest, TranslateAlongXAxis) {
  const auto originalVertices = model_->vertices();

  controller_->onTranslateModel({1.5f, 0.0f, 0.0f});

  const auto& translatedVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_FLOAT_EQ(translatedVertices[i].x_, originalVertices[i].x_ + 1.5f);
    EXPECT_FLOAT_EQ(translatedVertices[i].y_, originalVertices[i].y_);
    EXPECT_FLOAT_EQ(translatedVertices[i].z_, originalVertices[i].z_);
  }
}

TEST_F(ModelTranslateTest, TranslateAlongYAxis) {
  const auto originalVertices = model_->vertices();

  controller_->onTranslateModel({0.0f, 2.0f, 0.0f});

  const auto& translatedVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_FLOAT_EQ(translatedVertices[i].x_, originalVertices[i].x_);
    EXPECT_FLOAT_EQ(translatedVertices[i].y_, originalVertices[i].y_ + 2.0f);
    EXPECT_FLOAT_EQ(translatedVertices[i].z_, originalVertices[i].z_);
  }
}

TEST_F(ModelTranslateTest, TranslateAlongZAxis) {
  const auto originalVertices = model_->vertices();

  controller_->onTranslateModel({0.0f, 0.0f, -1.0f});

  const auto& translatedVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_FLOAT_EQ(translatedVertices[i].x_, originalVertices[i].x_);
    EXPECT_FLOAT_EQ(translatedVertices[i].y_, originalVertices[i].y_);
    EXPECT_FLOAT_EQ(translatedVertices[i].z_, originalVertices[i].z_ - 1.0f);
  }
}

TEST_F(ModelTranslateTest, TranslateAllAxes) {
  const auto originalVertices = model_->vertices();

  controller_->onTranslateModel({1.0f, 2.0f, 3.0f});

  const auto& translatedVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_FLOAT_EQ(translatedVertices[i].x_, originalVertices[i].x_ + 1.0f);
    EXPECT_FLOAT_EQ(translatedVertices[i].y_, originalVertices[i].y_ + 2.0f);
    EXPECT_FLOAT_EQ(translatedVertices[i].z_, originalVertices[i].z_ + 3.0f);
  }
}

TEST_F(ModelTranslateTest, TranslateZero) {
  const auto originalVertices = model_->vertices();

  controller_->onTranslateModel({0.0f, 0.0f, 0.0f});

  const auto& translatedVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_FLOAT_EQ(translatedVertices[i].x_, originalVertices[i].x_);
    EXPECT_FLOAT_EQ(translatedVertices[i].y_, originalVertices[i].y_);
    EXPECT_FLOAT_EQ(translatedVertices[i].z_, originalVertices[i].z_);
  }
}

TEST_F(ModelTranslateTest, TranslateNegativeValues) {
  const auto originalVertices = model_->vertices();

  controller_->onTranslateModel({-1.0f, -2.0f, -3.0f});

  const auto& translatedVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_FLOAT_EQ(translatedVertices[i].x_, originalVertices[i].x_ - 1.0f);
    EXPECT_FLOAT_EQ(translatedVertices[i].y_, originalVertices[i].y_ - 2.0f);
    EXPECT_FLOAT_EQ(translatedVertices[i].z_, originalVertices[i].z_ - 3.0f);
  }
}

TEST_F(ModelTranslateTest, TranslateMultipleTimes) {
  const auto originalVertices = model_->vertices();

  controller_->onTranslateModel({1.0f, 0.0f, 0.0f});
  controller_->onTranslateModel({0.0f, 1.0f, 0.0f});
  controller_->onTranslateModel({0.0f, 0.0f, 1.0f});

  const auto& translatedVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_FLOAT_EQ(translatedVertices[i].x_, originalVertices[i].x_ + 1.0f);
    EXPECT_FLOAT_EQ(translatedVertices[i].y_, originalVertices[i].y_ + 1.0f);
    EXPECT_FLOAT_EQ(translatedVertices[i].z_, originalVertices[i].z_ + 1.0f);
  }
}

// ============================================================================
// Model Transform Tests - Scale
// ============================================================================

class ModelScaleTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
    controller_->parseFile("../obj/cube.obj");
  }

  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
};

TEST_F(ModelScaleTest, ScaleUp200Percent) {
  const auto originalVertices = model_->vertices();

  controller_->onScaleModel(200);

  const auto& scaledVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(scaledVertices[i].x_, originalVertices[i].x_ * 2.0, 1e-5);
    EXPECT_NEAR(scaledVertices[i].y_, originalVertices[i].y_ * 2.0, 1e-5);
    EXPECT_NEAR(scaledVertices[i].z_, originalVertices[i].z_ * 2.0, 1e-5);
  }
}

TEST_F(ModelScaleTest, ScaleDown50Percent) {
  const auto originalVertices = model_->vertices();

  controller_->onScaleModel(50);

  const auto& scaledVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(scaledVertices[i].x_, originalVertices[i].x_ * 0.5, 1e-5);
    EXPECT_NEAR(scaledVertices[i].y_, originalVertices[i].y_ * 0.5, 1e-5);
    EXPECT_NEAR(scaledVertices[i].z_, originalVertices[i].z_ * 0.5, 1e-5);
  }
}

TEST_F(ModelScaleTest, Scale100PercentNoChange) {
  const auto originalVertices = model_->vertices();

  controller_->onScaleModel(100);

  const auto& scaledVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_FLOAT_EQ(scaledVertices[i].x_, originalVertices[i].x_);
    EXPECT_FLOAT_EQ(scaledVertices[i].y_, originalVertices[i].y_);
    EXPECT_FLOAT_EQ(scaledVertices[i].z_, originalVertices[i].z_);
  }
}

TEST_F(ModelScaleTest, ScaleMultipleTimes) {
  const auto originalVertices = model_->vertices();

  controller_->onScaleModel(200);  // 2x
  controller_->onScaleModel(100);  // back to 1x

  const auto& scaledVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(scaledVertices[i].x_, originalVertices[i].x_, 1e-5);
    EXPECT_NEAR(scaledVertices[i].y_, originalVertices[i].y_, 1e-5);
    EXPECT_NEAR(scaledVertices[i].z_, originalVertices[i].z_, 1e-5);
  }
}

TEST_F(ModelScaleTest, ScaleUp500Percent) {
  const auto originalVertices = model_->vertices();

  controller_->onScaleModel(500);

  const auto& scaledVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(scaledVertices[i].x_, originalVertices[i].x_ * 5.0, 1e-4);
    EXPECT_NEAR(scaledVertices[i].y_, originalVertices[i].y_ * 5.0, 1e-4);
    EXPECT_NEAR(scaledVertices[i].z_, originalVertices[i].z_ * 5.0, 1e-4);
  }
}

TEST_F(ModelScaleTest, ScaleDown10Percent) {
  const auto originalVertices = model_->vertices();

  controller_->onScaleModel(10);

  const auto& scaledVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(scaledVertices[i].x_, originalVertices[i].x_ * 0.1, 1e-5);
    EXPECT_NEAR(scaledVertices[i].y_, originalVertices[i].y_ * 0.1, 1e-5);
    EXPECT_NEAR(scaledVertices[i].z_, originalVertices[i].z_ * 0.1, 1e-5);
  }
}

// ============================================================================
// Model Data Management Tests
// ============================================================================

class ModelDataTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
  }

  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
};

TEST_F(ModelDataTest, ClearDataEmptiesModel) {
  controller_->parseFile("../obj/cube.obj");
  EXPECT_GT(model_->vertices().size(), 0);
  EXPECT_GT(model_->faces().size(), 0);

  controller_->clearData();

  EXPECT_EQ(model_->vertices().size(), 0);
  EXPECT_EQ(model_->faces().size(), 0);
  EXPECT_EQ(model_->vertexBuffer().size(), 0);
  EXPECT_EQ(model_->facesBuffer().size(), 0);
}

TEST_F(ModelDataTest, ClearDataThenParseNewFile) {
  controller_->parseFile("../obj/cube.obj");
  controller_->clearData();
  controller_->parseFile("../obj/monkey2.obj");

  EXPECT_EQ(model_->vertices().size(), 7958);
  EXPECT_EQ(model_->faces().size(), 15744);
}

TEST_F(ModelDataTest, RebuildBuffersAfterDirectVertexModification) {
  controller_->parseFile("../obj/cube.obj");

  const size_t expectedVertexBufferSize = model_->vertices().size() * 3;
  const size_t expectedFacesBufferSize = model_->faces().size() * 3;

  EXPECT_EQ(model_->vertexBuffer().size(), expectedVertexBufferSize);
  EXPECT_EQ(model_->facesBuffer().size(), expectedFacesBufferSize);
}

// ============================================================================
// Strategy Pattern Tests
// ============================================================================

class StrategyPatternTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
    controller_->parseFile("../obj/cube.obj");
  }

  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
};

TEST_F(StrategyPatternTest, RotateStrategyApplies) {
  const auto originalVertices = model_->vertices();

  RotateStrategy strategy({45, 0, 0});
  strategy.apply(*model_);

  const auto& rotatedVertices = model_->vertices();
  bool hasChanges = false;
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    if (std::abs(rotatedVertices[i].y_ - originalVertices[i].y_) > 1e-5 ||
        std::abs(rotatedVertices[i].z_ - originalVertices[i].z_) > 1e-5) {
      hasChanges = true;
      break;
    }
  }
  EXPECT_TRUE(hasChanges);
}

TEST_F(StrategyPatternTest, TranslateStrategyApplies) {
  const auto originalVertices = model_->vertices();

  TranslateStrategy strategy({1.0f, 2.0f, 3.0f});
  strategy.apply(*model_);

  const auto& translatedVertices = model_->vertices();
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_FLOAT_EQ(translatedVertices[i].x_, originalVertices[i].x_ + 1.0f);
    EXPECT_FLOAT_EQ(translatedVertices[i].y_, originalVertices[i].y_ + 2.0f);
    EXPECT_FLOAT_EQ(translatedVertices[i].z_, originalVertices[i].z_ + 3.0f);
  }
}

TEST_F(StrategyPatternTest, ScaleStrategyApplies) {
  const auto originalVertices = model_->vertices();

  ScaleStrategy strategy(200);
  strategy.apply(*model_);

  const auto& scaledVertices = model_->vertices();
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(scaledVertices[i].x_, originalVertices[i].x_ * 2.0, 1e-5);
    EXPECT_NEAR(scaledVertices[i].y_, originalVertices[i].y_ * 2.0, 1e-5);
    EXPECT_NEAR(scaledVertices[i].z_, originalVertices[i].z_ * 2.0, 1e-5);
  }
}

TEST_F(StrategyPatternTest, CombineMultipleStrategies) {
  const auto originalVertices = model_->vertices();

  RotateStrategy rotate({90, 0, 0});
  TranslateStrategy translate({1.0f, 0.0f, 0.0f});
  ScaleStrategy scale(150);

  rotate.apply(*model_);
  translate.apply(*model_);
  scale.apply(*model_);

  const auto& finalVertices = model_->vertices();
  EXPECT_EQ(finalVertices.size(), originalVertices.size());

  bool hasChanges = false;
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    if (std::abs(finalVertices[i].x_ - originalVertices[i].x_) > 1e-5 ||
        std::abs(finalVertices[i].y_ - originalVertices[i].y_) > 1e-5 ||
        std::abs(finalVertices[i].z_ - originalVertices[i].z_) > 1e-5) {
      hasChanges = true;
      break;
    }
  }
  EXPECT_TRUE(hasChanges);
}

// ============================================================================
// Edge Cases and Error Handling Tests
// ============================================================================

class ModelEdgeCasesTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
  }

  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
};

TEST_F(ModelEdgeCasesTest, RotateEmptyModel) {
  controller_->onRotateModel({90, 90, 90});

  EXPECT_EQ(model_->vertices().size(), 0);
  EXPECT_EQ(model_->faces().size(), 0);
}

TEST_F(ModelEdgeCasesTest, TranslateEmptyModel) {
  controller_->onTranslateModel({1.0f, 2.0f, 3.0f});

  EXPECT_EQ(model_->vertices().size(), 0);
  EXPECT_EQ(model_->faces().size(), 0);
}

TEST_F(ModelEdgeCasesTest, ScaleEmptyModel) {
  controller_->onScaleModel(200);

  EXPECT_EQ(model_->vertices().size(), 0);
  EXPECT_EQ(model_->faces().size(), 0);
}

TEST_F(ModelEdgeCasesTest, ParseEmptyFilePath) {
  controller_->parseFile("");

  EXPECT_EQ(model_->vertices().size(), 0);
  EXPECT_EQ(model_->faces().size(), 0);
}

TEST_F(ModelEdgeCasesTest, MultipleTransformationsConsistency) {
  controller_->parseFile("../obj/cube.obj");
  const auto originalVertices = model_->vertices();

  controller_->onRotateModel({90, 0, 0});
  controller_->onRotateModel({-90, 0, 0});

  const auto& finalVertices = model_->vertices();

  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(finalVertices[i].x_, originalVertices[i].x_, 1e-4);
    EXPECT_NEAR(finalVertices[i].y_, originalVertices[i].y_, 1e-4);
    EXPECT_NEAR(finalVertices[i].z_, originalVertices[i].z_, 1e-4);
  }
}

TEST_F(ModelEdgeCasesTest, LargeRotationAngles) {
  controller_->parseFile("../obj/cube.obj");

  controller_->onRotateModel({720, 1080, 1440});

  EXPECT_EQ(model_->vertices().size(), 8);
  EXPECT_EQ(model_->faces().size(), 12);
}

TEST_F(ModelEdgeCasesTest, VeryLargeTranslation) {
  controller_->parseFile("../obj/cube.obj");

  controller_->onTranslateModel({1000.0f, -1000.0f, 500.0f});

  EXPECT_EQ(model_->vertices().size(), 8);
  EXPECT_GT(model_->vertices()[0].x_, 900.0f);
}

TEST_F(ModelEdgeCasesTest, VerySmallScale) {
  controller_->parseFile("../obj/cube.obj");
  const auto originalVertices = model_->vertices();

  controller_->onScaleModel(1);

  const auto& scaledVertices = model_->vertices();
  for (size_t i = 0; i < originalVertices.size(); ++i) {
    EXPECT_NEAR(scaledVertices[i].x_, originalVertices[i].x_ * 0.01, 1e-5);
    EXPECT_NEAR(scaledVertices[i].y_, originalVertices[i].y_ * 0.01, 1e-5);
    EXPECT_NEAR(scaledVertices[i].z_, originalVertices[i].z_ * 0.01, 1e-5);
  }
}

// ============================================================================
// Integration Tests
// ============================================================================

class IntegrationTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
  }

  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
};

TEST_F(IntegrationTest, CompleteWorkflow) {
  controller_->parseFile("../obj/cube.obj");
  EXPECT_EQ(model_->vertices().size(), 8);

  controller_->onScaleModel(150);
  controller_->onRotateModel({45, 45, 0});
  controller_->onTranslateModel({1.0f, 0.5f, -0.5f});

  EXPECT_EQ(model_->vertices().size(), 8);
  EXPECT_EQ(model_->faces().size(), 12);
  EXPECT_EQ(model_->vertexBuffer().size(), 24);
  EXPECT_EQ(model_->facesBuffer().size(), 36);
}

TEST_F(IntegrationTest, LoadDifferentModels) {
  std::vector<std::string> models = {"../obj/cube.obj", "../obj/monkey2.obj",
                                     "../obj/teapot.obj"};

  for (const auto& modelPath : models) {
    controller_->parseFile(modelPath);
    EXPECT_GT(model_->vertices().size(), 0);
    EXPECT_GT(model_->faces().size(), 0);
    EXPECT_EQ(model_->vertexBuffer().size(), model_->vertices().size() * 3);
    EXPECT_EQ(model_->facesBuffer().size(), model_->faces().size() * 3);
  }
}

TEST_F(IntegrationTest, StressTestMultipleOperations) {
  controller_->parseFile("../obj/cube.obj");

  for (int i = 0; i < 10; ++i) {
    controller_->onRotateModel({10, 10, 10});
    controller_->onTranslateModel({0.1f, 0.1f, 0.1f});
  }

  EXPECT_EQ(model_->vertices().size(), 8);
  EXPECT_EQ(model_->faces().size(), 12);
}

TEST_F(IntegrationTest, ResetAndReload) {
  controller_->parseFile("../obj/cube.obj");
  controller_->onScaleModel(200);
  controller_->onRotateModel({90, 90, 90});

  controller_->clearData();
  EXPECT_EQ(model_->vertices().size(), 0);

  controller_->parseFile("../obj/cube.obj");
  EXPECT_EQ(model_->vertices().size(), 8);
  EXPECT_DOUBLE_EQ(model_->vertices()[0].x_, 0.9999995231628418);
}

// ============================================================================
// Logger Singleton Pattern Tests
// ============================================================================

class LoggerSingletonTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Создаём уникальный файл для каждого теста
    testLogFile_ =
        "test_logger_" +
        std::to_string(
            std::chrono::system_clock::now().time_since_epoch().count()) +
        ".log";
  }

  void TearDown() override {
    // Удаляем тестовый лог-файл
    std::remove(testLogFile_.c_str());
  }

  std::string testLogFile_;
};

TEST_F(LoggerSingletonTest, GetInstanceReturnsSameObject) {
  Logger& logger1 = Logger::getInstance();
  Logger& logger2 = Logger::getInstance();

  EXPECT_EQ(&logger1, &logger2);
}

TEST_F(LoggerSingletonTest, SingletonPersistsAcrossMultipleCalls) {
  Logger& logger1 = Logger::getInstance();
  logger1.setLogFile(testLogFile_);

  Logger& logger2 = Logger::getInstance();
  logger2.info("Test message");

  // Проверяем, что настройки сохранились
  std::ifstream logFile(testLogFile_);
  EXPECT_TRUE(logFile.is_open());

  std::string line;
  bool foundMessage = false;
  while (std::getline(logFile, line)) {
    if (line.find("Test message") != std::string::npos) {
      foundMessage = true;
      break;
    }
  }
  logFile.close();

  EXPECT_TRUE(foundMessage);
}

// ============================================================================
// Logger File Operations Tests
// ============================================================================

class LoggerFileTest : public ::testing::Test {
 protected:
  void SetUp() override {
    logger_ = &Logger::getInstance();
    testLogFile_ =
        "test_logger_file_" +
        std::to_string(
            std::chrono::system_clock::now().time_since_epoch().count()) +
        ".log";
    logger_->setLogFile(testLogFile_);
    logger_->setConsoleOutput(false);  // Отключаем вывод в консоль для тестов
    logger_->setLogLevel(LogLevel::DEBUG);
  }

  void TearDown() override { std::remove(testLogFile_.c_str()); }

  std::string readLogFile() {
    std::ifstream file(testLogFile_);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

  Logger* logger_;
  std::string testLogFile_;
};

TEST_F(LoggerFileTest, SetLogFileCreatesFile) {
  logger_->info("Test");

  std::ifstream logFile(testLogFile_);
  EXPECT_TRUE(logFile.is_open());
  logFile.close();
}

TEST_F(LoggerFileTest, LogMessageWrittenToFile) {
  const std::string testMessage = "Test log message";
  logger_->info(testMessage);

  std::string content = readLogFile();
  EXPECT_NE(content.find(testMessage), std::string::npos);
}

TEST_F(LoggerFileTest, MultipleMessagesWrittenInOrder) {
  logger_->info("First message");
  logger_->info("Second message");
  logger_->info("Third message");

  std::string content = readLogFile();

  size_t pos1 = content.find("First message");
  size_t pos2 = content.find("Second message");
  size_t pos3 = content.find("Third message");

  EXPECT_NE(pos1, std::string::npos);
  EXPECT_NE(pos2, std::string::npos);
  EXPECT_NE(pos3, std::string::npos);
  EXPECT_LT(pos1, pos2);
  EXPECT_LT(pos2, pos3);
}

TEST_F(LoggerFileTest, ChangeLogFileClosesOldAndOpensNew) {
  const std::string firstFile = testLogFile_;
  logger_->info("Message in first file");

  const std::string secondFile =
      "test_logger_second_" +
      std::to_string(
          std::chrono::system_clock::now().time_since_epoch().count()) +
      ".log";
  logger_->setLogFile(secondFile);
  logger_->info("Message in second file");

  std::string content1 = readLogFile();
  EXPECT_NE(content1.find("Message in first file"), std::string::npos);
  EXPECT_EQ(content1.find("Message in second file"), std::string::npos);

  std::ifstream file2(secondFile);
  std::stringstream buffer2;
  buffer2 << file2.rdbuf();
  std::string content2 = buffer2.str();
  file2.close();

  EXPECT_NE(content2.find("Message in second file"), std::string::npos);

  std::remove(secondFile.c_str());
}

// ============================================================================
// Logger Level Tests
// ============================================================================

class LoggerLevelTest : public ::testing::Test {
 protected:
  void SetUp() override {
    logger_ = &Logger::getInstance();
    testLogFile_ =
        "test_logger_level_" +
        std::to_string(
            std::chrono::system_clock::now().time_since_epoch().count()) +
        ".log";
    logger_->setLogFile(testLogFile_);
    logger_->setConsoleOutput(false);
  }

  void TearDown() override { std::remove(testLogFile_.c_str()); }

  std::string readLogFile() {
    std::ifstream file(testLogFile_);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

  Logger* logger_;
  std::string testLogFile_;
};

TEST_F(LoggerLevelTest, DebugLevelLogsAllMessages) {
  logger_->setLogLevel(LogLevel::DEBUG);

  logger_->debug("Debug message");
  logger_->info("Info message");
  logger_->warning("Warning message");
  logger_->error("Error message");

  std::string content = readLogFile();
  EXPECT_NE(content.find("Debug message"), std::string::npos);
  EXPECT_NE(content.find("Info message"), std::string::npos);
  EXPECT_NE(content.find("Warning message"), std::string::npos);
  EXPECT_NE(content.find("Error message"), std::string::npos);
}

TEST_F(LoggerLevelTest, InfoLevelFiltersDebug) {
  logger_->setLogLevel(LogLevel::INFO);

  logger_->debug("Debug message");
  logger_->info("Info message");
  logger_->warning("Warning message");
  logger_->error("Error message");

  std::string content = readLogFile();
  EXPECT_EQ(content.find("Debug message"), std::string::npos);
  EXPECT_NE(content.find("Info message"), std::string::npos);
  EXPECT_NE(content.find("Warning message"), std::string::npos);
  EXPECT_NE(content.find("Error message"), std::string::npos);
}

TEST_F(LoggerLevelTest, WarningLevelFiltersDebugAndInfo) {
  logger_->setLogLevel(LogLevel::WARNING);

  logger_->debug("Debug message");
  logger_->info("Info message");
  logger_->warning("Warning message");
  logger_->error("Error message");

  std::string content = readLogFile();
  EXPECT_EQ(content.find("Debug message"), std::string::npos);
  EXPECT_EQ(content.find("Info message"), std::string::npos);
  EXPECT_NE(content.find("Warning message"), std::string::npos);
  EXPECT_NE(content.find("Error message"), std::string::npos);
}

TEST_F(LoggerLevelTest, ErrorLevelOnlyLogsErrors) {
  logger_->setLogLevel(LogLevel::ERROR);

  logger_->debug("Debug message");
  logger_->info("Info message");
  logger_->warning("Warning message");
  logger_->error("Error message");

  std::string content = readLogFile();
  EXPECT_EQ(content.find("Debug message"), std::string::npos);
  EXPECT_EQ(content.find("Info message"), std::string::npos);
  EXPECT_EQ(content.find("Warning message"), std::string::npos);
  EXPECT_NE(content.find("Error message"), std::string::npos);
}

// ============================================================================
// Logger Message Format Tests
// ============================================================================

class LoggerFormatTest : public ::testing::Test {
 protected:
  void SetUp() override {
    logger_ = &Logger::getInstance();
    testLogFile_ =
        "test_logger_format_" +
        std::to_string(
            std::chrono::system_clock::now().time_since_epoch().count()) +
        ".log";
    logger_->setLogFile(testLogFile_);
    logger_->setConsoleOutput(false);
    logger_->setLogLevel(LogLevel::DEBUG);
  }

  void TearDown() override { std::remove(testLogFile_.c_str()); }

  std::string readLogFile() {
    std::ifstream file(testLogFile_);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

  Logger* logger_;
  std::string testLogFile_;
};

TEST_F(LoggerFormatTest, LogContainsTimestamp) {
  logger_->info("Test message");

  std::string content = readLogFile();
  // Формат: [YYYY-MM-DD HH:MM:SS.mmm]
  EXPECT_NE(content.find("[202"), std::string::npos);  // Год начинается с 202X
  EXPECT_NE(content.find(":"), std::string::npos);  // Разделители времени
}

TEST_F(LoggerFormatTest, LogContainsLevelTag) {
  logger_->debug("Test");
  logger_->info("Test");
  logger_->warning("Test");
  logger_->error("Test");

  std::string content = readLogFile();
  EXPECT_NE(content.find("[DEBUG]"), std::string::npos);
  EXPECT_NE(content.find("[INFO]"), std::string::npos);
  EXPECT_NE(content.find("[WARNING]"), std::string::npos);
  EXPECT_NE(content.find("[ERROR]"), std::string::npos);
}

TEST_F(LoggerFormatTest, LogContainsMessage) {
  const std::string message =
      "This is a test message with special chars: !@#$%";
  logger_->info(message);

  std::string content = readLogFile();
  EXPECT_NE(content.find(message), std::string::npos);
}

TEST_F(LoggerFormatTest, EmptyMessageLogged) {
  logger_->info("");

  std::string content = readLogFile();
  EXPECT_NE(content.find("[INFO]"), std::string::npos);
}

TEST_F(LoggerFormatTest, LongMessageLogged) {
  std::string longMessage(1000, 'A');
  logger_->info(longMessage);

  std::string content = readLogFile();
  EXPECT_NE(content.find(longMessage), std::string::npos);
}

TEST_F(LoggerFormatTest, MultilineMessageLogged) {
  const std::string multilineMessage = "Line 1\nLine 2\nLine 3";
  logger_->info(multilineMessage);

  std::string content = readLogFile();
  EXPECT_NE(content.find(multilineMessage), std::string::npos);
}

// ============================================================================
// Logger Integration with Model Tests
// ============================================================================

class LoggerIntegrationTest : public ::testing::Test {
 protected:
  void SetUp() override {
    logger_ = &Logger::getInstance();
    testLogFile_ =
        "test_logger_integration_" +
        std::to_string(
            std::chrono::system_clock::now().time_since_epoch().count()) +
        ".log";
    logger_->setLogFile(testLogFile_);
    logger_->setConsoleOutput(false);
    logger_->setLogLevel(LogLevel::DEBUG);

    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
  }

  void TearDown() override {
    model_.reset();
    controller_.reset();
    std::remove(testLogFile_.c_str());
  }

  std::string readLogFile() {
    std::ifstream file(testLogFile_);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
  }

  Logger* logger_;
  std::string testLogFile_;
  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
};

TEST_F(LoggerIntegrationTest, ParseFileLogsToFile) {
  controller_->parseFile("../obj/cube.obj");

  std::string content = readLogFile();
  EXPECT_NE(content.find("Начало парсинга файла"), std::string::npos);
  EXPECT_NE(content.find("cube.obj"), std::string::npos);
  EXPECT_NE(content.find("Парсинг и нормализация завершены"),
            std::string::npos);
}

TEST_F(LoggerIntegrationTest, ParseFileLogsStatistics) {
  controller_->parseFile("../obj/cube.obj");

  std::string content = readLogFile();
  EXPECT_NE(content.find("Вершин загружено"), std::string::npos);
  EXPECT_NE(content.find("Треугольников создано"), std::string::npos);
}

TEST_F(LoggerIntegrationTest, TransformationsLoggedCorrectly) {
  controller_->parseFile("../obj/cube.obj");

  controller_->onRotateModel({45, 0, 0});
  controller_->onTranslateModel({1.0f, 2.0f, 3.0f});
  controller_->onScaleModel(150);

  std::string content = readLogFile();
  EXPECT_NE(content.find("Поворот модели"), std::string::npos);
  EXPECT_NE(content.find("Перемещение модели"), std::string::npos);
  EXPECT_NE(content.find("Изменение масштаба"), std::string::npos);
}

TEST_F(LoggerIntegrationTest, ErrorOnNonExistentFileLogged) {
  controller_->parseFile("../obj/nonexistent.obj");

  std::string content = readLogFile();
  EXPECT_NE(content.find("КРИТИЧЕСКАЯ ОШИБКА"), std::string::npos);
  EXPECT_NE(content.find("Не удалось открыть файл"), std::string::npos);
}

TEST_F(LoggerIntegrationTest, ClearDataLogged) {
  controller_->parseFile("../obj/cube.obj");
  controller_->clearData();

  std::string content = readLogFile();
  EXPECT_NE(content.find("Очистка данных модели"), std::string::npos);
  EXPECT_NE(content.find("Очищено"), std::string::npos);
}

// ============================================================================
// Logger Thread Safety Tests (Basic)
// ============================================================================

class LoggerThreadSafetyTest : public ::testing::Test {
 protected:
  void SetUp() override {
    logger_ = &Logger::getInstance();
    testLogFile_ =
        "test_logger_thread_" +
        std::to_string(
            std::chrono::system_clock::now().time_since_epoch().count()) +
        ".log";
    logger_->setLogFile(testLogFile_);
    logger_->setConsoleOutput(false);
    logger_->setLogLevel(LogLevel::DEBUG);
  }

  void TearDown() override { std::remove(testLogFile_.c_str()); }

  Logger* logger_;
  std::string testLogFile_;
};

TEST_F(LoggerThreadSafetyTest, MultipleSequentialLogsSucceed) {
  for (int i = 0; i < 100; ++i) {
    logger_->info("Message " + std::to_string(i));
  }

  std::ifstream file(testLogFile_);
  int lineCount = 0;
  std::string line;
  while (std::getline(file, line)) {
    ++lineCount;
  }
  file.close();

  EXPECT_EQ(lineCount, 100);
}

// ============================================================================
// Heavy Model Tests
// ============================================================================

class HeavyModelTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
    logger_ = &Logger::getInstance();
    testLogFile_ =
        "test_heavy_model_" +
        std::to_string(
            std::chrono::system_clock::now().time_since_epoch().count()) +
        ".log";
    logger_->setLogFile(testLogFile_);
    logger_->setConsoleOutput(false);
    logger_->setLogLevel(LogLevel::DEBUG);
  }

  void TearDown() override { std::remove(testLogFile_.c_str()); }

  std::string readLogFile() {
    std::ifstream file(testLogFile_);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    return content;
  }

  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
  Logger* logger_;
  std::string testLogFile_;
};

TEST_F(HeavyModelTest, IronManModelWithTriangulation) {
  controller_->parseFile("../obj/IronMan.obj");

  EXPECT_GT(model_->vertices().size(), 100000);
  EXPECT_GT(model_->faces().size(), 100000);

  std::string content = readLogFile();
  EXPECT_NE(content.find("Начало парсинга файла"), std::string::npos);
  EXPECT_NE(content.find("успешно"), std::string::npos);
  EXPECT_NE(content.find("вершин"), std::string::npos);
  EXPECT_NE(content.find("граней"), std::string::npos);
  EXPECT_NE(content.find("Прочитано вершин:"), std::string::npos);
}

TEST_F(HeavyModelTest, IronManTriangulationLogsProgress) {
  controller_->parseFile("../obj/IronMan.obj");

  std::string content = readLogFile();
  int progressMarkers = 0;
  size_t pos = 0;
  while ((pos = content.find("Прочитано", pos)) != std::string::npos) {
    ++progressMarkers;
    ++pos;
  }

  EXPECT_GT(progressMarkers, 5);
}

TEST_F(HeavyModelTest, IronManNormalizationSucceeds) {
  controller_->parseFile("../obj/IronMan.obj");

  std::string content = readLogFile();
  EXPECT_NE(content.find("Начало нормализации модели"), std::string::npos);
  EXPECT_NE(content.find("Границы модели"), std::string::npos);
  EXPECT_NE(content.find("Модель нормализована"), std::string::npos);

  for (const auto& vertex : model_->vertices()) {
    EXPECT_GE(vertex.x_, -1.1f);
    EXPECT_LE(vertex.x_, 1.1f);
    EXPECT_GE(vertex.y_, -1.1f);
    EXPECT_LE(vertex.y_, 1.1f);
    EXPECT_GE(vertex.z_, -1.1f);
    EXPECT_LE(vertex.z_, 1.1f);
  }
}

TEST_F(HeavyModelTest, IronManTransformationsWork) {
  controller_->parseFile("../obj/IronMan.obj");
  const size_t originalVertexCount = model_->vertices().size();
  const size_t originalFaceCount = model_->faces().size();

  controller_->onRotateModel({45, 90, 180});
  EXPECT_EQ(model_->vertices().size(), originalVertexCount);
  EXPECT_EQ(model_->faces().size(), originalFaceCount);

  controller_->onTranslateModel({10.0f, -5.0f, 2.5f});
  EXPECT_EQ(model_->vertices().size(), originalVertexCount);
  EXPECT_EQ(model_->faces().size(), originalFaceCount);

  controller_->onScaleModel(150);
  EXPECT_EQ(model_->vertices().size(), originalVertexCount);
  EXPECT_EQ(model_->faces().size(), originalFaceCount);

  std::string content = readLogFile();
  EXPECT_NE(content.find("Поворот модели"), std::string::npos);
  EXPECT_NE(content.find("Перемещение модели"), std::string::npos);
  EXPECT_NE(content.find("Изменение масштаба"), std::string::npos);
}

// ============================================================================
// Controller Method Tests
// ============================================================================

class ControllerMethodTest : public ::testing::Test {
 protected:
  void SetUp() override {
    int argc = 0;
    char** argv = nullptr;
    if (!QApplication::instance()) {
      app_ = std::make_unique<QApplication>(argc, argv);
    }
    model_ = std::make_unique<Model>();
    controller_ = std::make_unique<Controller>(model_.get());
    glWidget_ = std::make_unique<GLWidget>();
  }

  void TearDown() override {
    glWidget_.reset();
    controller_.reset();
    model_.reset();
  }

  std::unique_ptr<QApplication> app_;
  std::unique_ptr<Model> model_;
  std::unique_ptr<Controller> controller_;
  std::unique_ptr<GLWidget> glWidget_;
};

TEST_F(ControllerMethodTest, SetGLWidgetSucceeds) {
  controller_->setGLWidget(glWidget_.get());
  controller_->parseFile("../obj/cube.obj");

  EXPECT_EQ(model_->vertices().size(), 8);
  EXPECT_EQ(model_->faces().size(), 12);
}

TEST_F(ControllerMethodTest, SetModelDataUpdatesGLWidget) {
  controller_->setGLWidget(glWidget_.get());
  controller_->parseFile("../obj/cube.obj");

  const auto& vertexBuffer = model_->vertexBuffer();
  const auto& facesBuffer = model_->facesBuffer();

  EXPECT_GT(vertexBuffer.size(), 0);
  EXPECT_GT(facesBuffer.size(), 0);

  glWidget_->setModelData(vertexBuffer, facesBuffer);
}

TEST_F(ControllerMethodTest, SetCameraAnglesChangesView) {
  controller_->setGLWidget(glWidget_.get());

  controller_->setCameraAngles(1.57f, 0.78f);
  controller_->setCameraAngles(0.0f, 0.0f);
  controller_->setCameraAngles(-1.57f, -0.78f);
}

TEST_F(ControllerMethodTest, SetCameraDistanceChangesZoom) {
  controller_->setGLWidget(glWidget_.get());

  controller_->setCameraDistance(5.0f);
  controller_->setCameraDistance(2.0f);
  controller_->setCameraDistance(10.0f);
}

TEST_F(ControllerMethodTest, ControllerWithoutGLWidgetDoesNotCrash) {
  controller_->parseFile("../obj/cube.obj");
  controller_->onRotateModel({45, 0, 0});
  controller_->onTranslateModel({1.0f, 0.0f, 0.0f});
  controller_->onScaleModel(200);
  controller_->clearData();

  EXPECT_EQ(model_->vertices().size(), 0);
  EXPECT_EQ(model_->faces().size(), 0);
}

TEST_F(ControllerMethodTest, MultipleTransformationsWithGLWidget) {
  controller_->setGLWidget(glWidget_.get());
  controller_->parseFile("../obj/cube.obj");

  controller_->onRotateModel({30, 45, 60});
  controller_->onTranslateModel({2.0f, -1.0f, 0.5f});
  controller_->onScaleModel(120);

  controller_->setCameraAngles(0.5f, 1.0f);
  controller_->setCameraDistance(3.5f);

  EXPECT_EQ(model_->vertices().size(), 8);
  EXPECT_EQ(model_->faces().size(), 12);
}