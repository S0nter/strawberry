/* This file is part of Clementine.
   Copyright 2010, David Sansome <me@davidsansome.com>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROJECTMVISUALISATION_H
#define PROJECTMVISUALISATION_H

#include <QBasicTimer>
#include <QGraphicsScene>
#include <QSet>
#include <memory>

#include "engine/gstbufferconsumer.h"

class projectM;

class ProjectMPresetModel;

class VisualisationContainer;

class QTemporaryFile;

class ProjectMVisualisation : public QGraphicsScene, public GstBufferConsumer {
  Q_OBJECT
 public:
  ProjectMVisualisation(VisualisationContainer* container);
  ~ProjectMVisualisation();

  enum Mode {
    Random = 0,
    FromList = 1,
  };

  QString preset_url() const;
  ProjectMPresetModel* preset_model() const { return preset_model_; }

  Mode mode() const { return mode_; }
  int duration() const { return duration_; }

  // BufferConsumer
  void ConsumeBuffer(GstBuffer* buffer, const int pipeline_id, const QString &format);

 public slots:
  void SetTextureSize(int size);
  void SetDuration(int seconds);

  void SetSelected(const QStringList& paths, bool selected);
  void ClearSelected();
  void SetImmediatePreset(const QString& path);
  void SetMode(Mode mode);

  void Lock(bool lock);

 protected:
  // QGraphicsScene
  void drawBackground(QPainter* painter, const QRectF& rect);

 private slots:
  void SceneRectChanged(const QRectF& rect);

 private:
  void InitProjectM();
  void Load();
  void Save();

  int IndexOfPreset(const QString& path) const;

 private:
  std::unique_ptr<projectM> projectm_;
  ProjectMPresetModel* preset_model_;
  Mode mode_;
  int duration_;

  std::unique_ptr<QTemporaryFile> temporary_font_;

  std::vector<int> default_rating_list_;

  int texture_size_;
  // As of version 5.6, Qt supports automatic scaling for high-DPI displays. We
  // need to know the pixel ratio so that we can convert coordinates for
  // projectM.
  // https://doc.qt.io/qt-5/highdpi.html
  qreal pixel_ratio_;
  VisualisationContainer* container_;
};

#endif  // PROJECTMVISUALISATION_H
