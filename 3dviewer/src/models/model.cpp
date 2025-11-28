#include "model.h"

#include <fstream>
#include <iostream>
#include <numbers>
#include <regex>
#include <sstream>
#include <string>

#include "../utils/logger.h"

using namespace s21;
using namespace std;

// пока что через regex, если будет медленно работать нужно будет что нибудь
// придумать
void Model::parseFile(string filepath) {
  Logger& logger = Logger::getInstance();
  logger.info("═══════════════════════════════════════════════════════");
  logger.info("Начало парсинга файла: " + filepath);

  ifstream f(filepath);

  if (!f.is_open()) {
    logger.error("КРИТИЧЕСКАЯ ОШИБКА: Не удалось открыть файл: " + filepath);
    cerr << "Не удалось открыть файл: " << filepath << endl;
    return;
  }

  logger.debug("Файл успешно открыт, начинаем чтение данных");
  clearData();
  logger.debug("Предыдущие данные очищены");

  string s;
  regex vertex(
      "^v\\s+([-+]?\\d*\\.?\\d+)\\s+([-+]?\\d*\\.?\\d+)\\s+([-+]?\\d*\\.?\\d+"
      ")");

  float xf = 0, yf = 0, zf = 0;
  size_t lineCount = 0;
  size_t vertexCount = 0;
  size_t faceCount = 0;
  size_t triangulatedCount = 0;

  while (getline(f, s)) {
    lineCount++;

    if (s.empty() || s[0] == '#') {
      continue;
    }

    if (s[0] == 'v' && s[1] == ' ') {
      stringstream ss(s);
      ss >> s;  // skip 'v'
      ss >> xf >> yf >> zf;
      vertices_.push_back({xf, yf, zf});
      vertexCount++;

      if (vertexCount % 10000 == 0) {
        logger.debug("Прочитано вершин: " + to_string(vertexCount));
      }
    } else if (s[0] == 'f' && s[1] == ' ') {
      stringstream ss(s);
      ss >> s;  // skip 'f'

      vector<int> faceIndices;
      string token;
      while (ss >> token) {
        size_t slashPos = token.find('/');
        string vertexIndexStr =
            (slashPos != string::npos) ? token.substr(0, slashPos) : token;

        int vertexIndex = stoi(vertexIndexStr);
        faceIndices.push_back(vertexIndex - 1);
      }

      if (faceIndices.size() == 3) {
        faces_.push_back({faceIndices[0], faceIndices[1], faceIndices[2]});
      } else if (faceIndices.size() == 4) {
        faces_.push_back({faceIndices[0], faceIndices[1], faceIndices[2]});
        faces_.push_back({faceIndices[0], faceIndices[2], faceIndices[3]});
        triangulatedCount++;
      } else if (faceIndices.size() > 4) {
        for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
          faces_.push_back(
              {faceIndices[0], faceIndices[i], faceIndices[i + 1]});
        }
        triangulatedCount++;
        logger.debug("Полигон с " + to_string(faceIndices.size()) +
                     " вершинами триангулирован");
      }

      faceCount++;
      if (faceCount % 20000 == 0) {
        logger.debug("Обработано граней: " + to_string(faceCount) +
                     ", треугольников: " + to_string(faces_.size()));
      }
    }
  }

  f.close();
  logger.debug("Файл закрыт, строк обработано: " + to_string(lineCount));

  logger.info("─── Статистика парсинга ───");
  logger.info("Вершин загружено: " + to_string(vertices_.size()));
  logger.info("Граней обработано: " + to_string(faceCount));
  logger.info("Треугольников создано: " + to_string(faces_.size()));
  if (triangulatedCount > 0) {
    logger.info("Полигонов триангулировано: " + to_string(triangulatedCount));
  }

  logger.info("Начало нормализации модели...");
  normalizeModel();
  logger.info("Парсинг и нормализация завершены успешно");
  logger.info("═══════════════════════════════════════════════════════");
}

void Model::clearData() {
  Logger& logger = Logger::getInstance();
  logger.debug("Очистка данных модели...");

  size_t oldVertices = vertices_.size();
  size_t oldFaces = faces_.size();

  vertices_.clear();
  faces_.clear();
  vertices_.shrink_to_fit();
  faces_.shrink_to_fit();
  vertexBuffer_.clear();
  facesBuffer_.clear();
  vertexBuffer_.shrink_to_fit();
  facesBuffer_.shrink_to_fit();

  if (oldVertices > 0 || oldFaces > 0) {
    logger.debug("Очищено: вершин " + to_string(oldVertices) + ", граней " +
                 to_string(oldFaces));
  }
}

void Model::normalizeModel() {
  Logger& logger = Logger::getInstance();
  logger.debug("Нормализация: этап 1 - центрирование");

  centerModel();
  logger.debug("Нормализация: этап 2 - масштабирование");

  float maxDiff = max(
      max(extremums_.maxX - extremums_.minX, extremums_.maxY - extremums_.minY),
      extremums_.maxZ - extremums_.minZ);

  double baseScale = BASE_SCALE / maxDiff;

  logger.debug("Границы модели: X[" + to_string(extremums_.minX) + ", " +
               to_string(extremums_.maxX) + "], Y[" +
               to_string(extremums_.minY) + ", " + to_string(extremums_.maxY) +
               "], Z[" + to_string(extremums_.minZ) + ", " +
               to_string(extremums_.maxZ) + "]");
  logger.debug("Максимальный размер: " + to_string(maxDiff) +
               ", коэффициент масштаба: " + to_string(baseScale));

  scaleModel(baseScale);
  logger.info("Модель нормализована и отцентрирована");
}

void Model::centerModel() {
  Logger& logger = Logger::getInstance();
  logger.debug("Поиск экстремумов модели...");

  findExtremums();

  float centerX = extremums_.minX + (extremums_.maxX - extremums_.minX) / 2;
  float centerY = extremums_.minY + (extremums_.maxY - extremums_.minY) / 2;
  float centerZ = extremums_.minZ + (extremums_.maxZ - extremums_.minZ) / 2;

  logger.debug("Центр модели: (" + to_string(centerX) + ", " +
               to_string(centerY) + ", " + to_string(centerZ) + ")");

  for (auto& vertex : vertices_) {
    vertex.x_ -= centerX;
    vertex.y_ -= centerY;
    vertex.z_ -= centerZ;
  }

  logger.debug("Модель отцентрирована в (0, 0, 0)");
}

void Model::findExtremums() {
  extremums_.maxX = extremums_.minX = vertices_[0].x_;
  extremums_.maxY = extremums_.minY = vertices_[0].y_;
  extremums_.maxZ = extremums_.minZ = vertices_[0].z_;

  for (const auto& vertex : vertices_) {
    if (vertex.x_ > extremums_.maxX) extremums_.maxX = vertex.x_;
    if (vertex.y_ > extremums_.maxY) extremums_.maxY = vertex.y_;
    if (vertex.z_ > extremums_.maxZ) extremums_.maxZ = vertex.z_;
    if (vertex.x_ < extremums_.minX) extremums_.minX = vertex.x_;
    if (vertex.y_ < extremums_.minY) extremums_.minY = vertex.y_;
    if (vertex.z_ < extremums_.minZ) extremums_.minZ = vertex.z_;
  }
}

void Model::rebuildBuffers() {
  Logger& logger = Logger::getInstance();
  logger.debug("Пересборка буферов OpenGL...");

  vertexBuffer_.clear();
  facesBuffer_.clear();

  vertexBuffer_.reserve(vertices_.size() * 3);
  for (const auto& v : vertices_) {
    vertexBuffer_.push_back(v.x_);
    vertexBuffer_.push_back(v.y_);
    vertexBuffer_.push_back(v.z_);
  }

  facesBuffer_.reserve(faces_.size() * 3);
  for (const auto& f : faces_) {
    facesBuffer_.push_back(f.x_);
    facesBuffer_.push_back(f.y_);
    facesBuffer_.push_back(f.z_);
  }

  logger.debug(
      "Буферы пересобраны: vertexBuffer=" + to_string(vertexBuffer_.size()) +
      " floats, facesBuffer=" + to_string(facesBuffer_.size()) + " ints");
}

void Model::scaleModel(double scale) {
  Logger& logger = Logger::getInstance();
  logger.debug("Применение масштаба: " + to_string(scale));

  for (auto& vertex : vertices_) {
    vertex.x_ *= scale;
    vertex.y_ *= scale;
    vertex.z_ *= scale;
  }
  rebuildBuffers();

  logger.debug("Масштабирование завершено");
}

void Model::rotateModel(Point<int> shift) {
  if (vertices_.empty()) {
    return;
  }

  Logger& logger = Logger::getInstance();
  logger.debug("Поворот модели: X=" + to_string(shift.x_) + "°, Y=" +
               to_string(shift.y_) + "°, Z=" + to_string(shift.z_) + "°");

  const double radX =
      static_cast<double>(shift.x_) * std::numbers::pi_v<double> / 180.0;
  const double radY =
      static_cast<double>(shift.y_) * std::numbers::pi_v<double> / 180.0;
  const double radZ =
      static_cast<double>(shift.z_) * std::numbers::pi_v<double> / 180.0;

  const double cosX = std::cos(radX);
  const double sinX = std::sin(radX);
  const double cosY = std::cos(radY);
  const double sinY = std::sin(radY);
  const double cosZ = std::cos(radZ);
  const double sinZ = std::sin(radZ);

  for (auto& vertex : vertices_) {
    const double y0 = vertex.y_;
    const double z0 = vertex.z_;

    const double y1 = y0 * cosX - z0 * sinX;  // rotate around X
    const double z1 = y0 * sinX + z0 * cosX;

    const double x0 = vertex.x_;
    const double x1 = x0 * cosY + z1 * sinY;  // rotate around Y
    const double z2 = -x0 * sinY + z1 * cosY;

    const double x2 = x1 * cosZ - y1 * sinZ;  // rotate around Z
    const double y2 = x1 * sinZ + y1 * cosZ;

    vertex.x_ = static_cast<float>(x2);
    vertex.y_ = static_cast<float>(y2);
    vertex.z_ = static_cast<float>(z2);
  }

  rebuildBuffers();
  logger.debug("Поворот применён");
}

void Model::translateModel(Point<float> shift) {
  if (vertices_.empty()) {
    return;
  }

  Logger& logger = Logger::getInstance();
  logger.debug("Перемещение модели: ΔX=" + to_string(shift.x_) +
               ", ΔY=" + to_string(shift.y_) + ", ΔZ=" + to_string(shift.z_));

  for (auto& vertex : vertices_) {
    vertex.x_ += static_cast<float>(shift.x_);
    vertex.y_ += static_cast<float>(shift.y_);
    vertex.z_ += static_cast<float>(shift.z_);
  }

  rebuildBuffers();
  logger.debug("Перемещение применено");
}

void Model::setScalePercent(int percent) {
  if (vertices_.empty()) {
    return;
  }

  Logger& logger = Logger::getInstance();

  const double newScale = percent / 100.0;

  if (std::abs(newScale - currentScale_) <
      std::numeric_limits<double>::epsilon()) {
    logger.debug("Масштаб не изменился: " + to_string(percent) + "%");
    return;
  }

  const double scaleRatio = newScale / currentScale_;
  logger.info("Изменение масштаба: " + to_string(currentScale_ * 100) + "% → " +
              to_string(percent) + "% (коэф. " + to_string(scaleRatio) + ")");

  scaleModel(scaleRatio);
  currentScale_ = newScale;
}