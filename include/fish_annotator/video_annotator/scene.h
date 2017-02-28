#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QMessageBox>
#include <QGraphicsView>
#include <QKeyEvent>

namespace fish_annotator {
  namespace video_annotator {
		class Scene : public QGraphicsScene
		{
			Q_OBJECT
		public:
			enum Mode { NoMode, SelectObject, DrawLine };
			Scene(QObject* parent = 0);
			void setMode(Mode mode);
		protected:
			void mousePressEvent(QGraphicsSceneMouseEvent *event);
			void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
			void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
			void keyPressEvent(QKeyEvent *event);
		private:
			Mode scene_mode_;
			QGraphicsLineItem* item_to_draw_;
      QLineF line_to_draw_;
			void makeItemsControllable(bool areControllable);
		signals:
			void lineFinished(const QLineF &line_to_add);
		};
	}
} // namespace fish_annotator

#endif // ANNOTATEDREGION_H

