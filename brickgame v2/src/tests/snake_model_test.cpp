#include <gtest/gtest.h>

#include "../brick_game/snake/snake.h"

namespace s21 {

TEST(SnakeModelTest, ResetInitialState) {
  SnakeModel model;

  const Snake& snake = model.getSnake();
  EXPECT_EQ(snake.body.size(), 4U);
  EXPECT_EQ(model.getScore(), 0);
  EXPECT_EQ(model.getLevel(), 1);
  EXPECT_GE(model.getHighScore(), 0);
  EXPECT_EQ(model.getCurrentState(), STATE_START);
}

TEST(SnakeModelTest, StartSetsRunningAndMoving) {
  SnakeModel model;
  model.start();

  EXPECT_TRUE(model.isRunning());
  EXPECT_FALSE(model.isPaused());
  EXPECT_EQ(model.getCurrentState(), STATE_MOVING);
}

TEST(SnakeModelTest, TogglePauseTransitionsState) {
  SnakeModel model;
  model.start();
  EXPECT_EQ(model.getCurrentState(), STATE_MOVING);

  model.togglePause();
  EXPECT_TRUE(model.isPaused());
  EXPECT_EQ(model.getCurrentState(), STATE_PAUSE);

  model.togglePause();
  EXPECT_FALSE(model.isPaused());
  EXPECT_EQ(model.getCurrentState(), STATE_MOVING);
}

TEST(SnakeModelTest, AppleConsumptionIncreasesScore) {
  SnakeModel model;
  model.start();

  const size_t initial_length = model.getSnake().body.size();
  Point head = model.getSnake().body.front();
  Point apple_target{head.x + 1, head.y};

  SnakeModelTestAccessor::SetApplePosition(model, apple_target);
  SnakeModelTestAccessor::SetState(model, STATE_MOVING);
  SnakeModelTestAccessor::SetRunning(model, true);
  SnakeModelTestAccessor::SetPaused(model, false);
  SnakeModelTestAccessor::ForceReadyForMove(model);

  model.move();

  EXPECT_EQ(model.getScore(), 10);
  EXPECT_EQ(model.getSnake().body.size(), initial_length + 1);
  EXPECT_EQ(model.getCurrentState(), STATE_MOVING);
}

TEST(SnakeModelTest, WallCollisionTriggersGameOver) {
  SnakeModel model;
  model.start();

  std::deque<Point> body = {{0, 5}, {1, 5}, {2, 5}, {3, 5}};
  SnakeModelTestAccessor::SetSnakeBody(model, body);
  SnakeModelTestAccessor::SetDirection(model, 2);  // left
  SnakeModelTestAccessor::SetState(model, STATE_MOVING);
  SnakeModelTestAccessor::SetRunning(model, true);
  SnakeModelTestAccessor::SetPaused(model, false);
  SnakeModelTestAccessor::ForceReadyForMove(model);

  model.move();

  EXPECT_TRUE(model.isGameOver());
  EXPECT_EQ(model.getCurrentState(), STATE_GAME_OVER);
}

TEST(SnakeModelTest, SelfCollisionTriggersGameOver) {
  SnakeModel model;
  model.start();

  std::deque<Point> body = {{5, 5}, {5, 6}, {4, 6}, {4, 5}};
  SnakeModelTestAccessor::SetSnakeBody(model, body);
  SnakeModelTestAccessor::SetDirection(model, 1);  // down
  SnakeModelTestAccessor::SetState(model, STATE_MOVING);
  SnakeModelTestAccessor::SetRunning(model, true);
  SnakeModelTestAccessor::SetPaused(model, false);
  SnakeModelTestAccessor::ForceReadyForMove(model);

  model.move();

  EXPECT_TRUE(model.isGameOver());
  EXPECT_EQ(model.getCurrentState(), STATE_GAME_OVER);
}

TEST(SnakeFSMTest, ActionStartsGameFromStartState) {
  snake_init_game();
  ASSERT_NE(s21::snake_model, nullptr);
  EXPECT_EQ(snake_get_current_state(), STATE_START);

  snake_handle_input(Action, false);

  EXPECT_EQ(snake_get_current_state(), STATE_MOVING);
  EXPECT_TRUE(s21::snake_model->isRunning());

  snake_cleanup_game();
}

TEST(SnakeFSMTest, PauseInputTogglesState) {
  snake_init_game();
  auto* model = s21::snake_model;
  ASSERT_NE(model, nullptr);

  snake_handle_input(Action, false);
  EXPECT_EQ(model->getCurrentState(), STATE_MOVING);

  snake_handle_input(Pause, false);
  EXPECT_EQ(model->getCurrentState(), STATE_PAUSE);

  snake_handle_input(Pause, false);
  EXPECT_EQ(model->getCurrentState(), STATE_MOVING);

  snake_cleanup_game();
}

TEST(SnakeFSMTest, TerminateTransitionsToGameOver) {
  snake_init_game();
  auto* model = s21::snake_model;
  ASSERT_NE(model, nullptr);

  snake_handle_input(Action, false);
  EXPECT_EQ(model->getCurrentState(), STATE_MOVING);

  snake_handle_input(Terminate, false);
  EXPECT_EQ(model->getCurrentState(), STATE_GAME_OVER);

  snake_cleanup_game();
}

TEST(SnakeFSMTest, ActionFromGameOverResetsAndRestarts) {
  snake_init_game();
  auto* model = s21::snake_model;
  ASSERT_NE(model, nullptr);

  snake_handle_input(Action, false);
  EXPECT_EQ(model->getCurrentState(), STATE_MOVING);

  Point head = model->getSnake().body.front();
  Point target{head.x + 1, head.y};
  SnakeModelTestAccessor::SetApplePosition(*model, target);
  SnakeModelTestAccessor::SetRunning(*model, true);
  SnakeModelTestAccessor::SetPaused(*model, false);
  SnakeModelTestAccessor::ForceReadyForMove(*model);
  SnakeModelTestAccessor::SetState(*model, STATE_MOVING);
  snake_handle_moving_state();
  EXPECT_GT(model->getScore(), 0);

  snake_handle_input(Terminate, false);
  EXPECT_EQ(model->getCurrentState(), STATE_GAME_OVER);

  snake_handle_input(Action, false);
  EXPECT_EQ(model->getCurrentState(), STATE_MOVING);
  EXPECT_TRUE(model->isRunning());
  EXPECT_EQ(model->getScore(), 0);

  snake_cleanup_game();
}

TEST(SnakeFSMTest, MovingStateWrapperTriggersCollisions) {
  snake_init_game();
  auto* model = s21::snake_model;
  ASSERT_NE(model, nullptr);

  snake_handle_input(Action, false);
  EXPECT_EQ(model->getCurrentState(), STATE_MOVING);

  std::deque<Point> body = {{0, 5}, {1, 5}, {2, 5}, {3, 5}};
  SnakeModelTestAccessor::SetSnakeBody(*model, body);
  SnakeModelTestAccessor::SetDirection(*model, 2);
  SnakeModelTestAccessor::SetRunning(*model, true);
  SnakeModelTestAccessor::SetPaused(*model, false);
  SnakeModelTestAccessor::SetState(*model, STATE_MOVING);
  SnakeModelTestAccessor::ForceReadyForMove(*model);

  snake_handle_moving_state();

  EXPECT_TRUE(model->isGameOver());
  EXPECT_EQ(model->getCurrentState(), STATE_GAME_OVER);

  snake_cleanup_game();
}

TEST(SnakeFSMTest, ActionHoldAdjustsAccelerationFlag) {
  snake_init_game();
  auto* model = s21::snake_model;
  ASSERT_NE(model, nullptr);

  snake_handle_input(Action, false);
  EXPECT_EQ(model->getCurrentState(), STATE_MOVING);
  EXPECT_FALSE(SnakeModelTestAccessor::IsAccelerated(*model));

  snake_handle_input(Action, true);
  EXPECT_TRUE(SnakeModelTestAccessor::IsAccelerated(*model));

  snake_handle_input(Action, false);
  EXPECT_FALSE(SnakeModelTestAccessor::IsAccelerated(*model));

  snake_cleanup_game();
}

}  // namespace s21
