#include "fish_annotator/video_annotator/scene.h"

namespace fish_annotator {
	namespace video_annotator {

		Scene::Scene(QObject* parent) : QGraphicsScene(parent)
		{
			sceneMode = NoMode;
			itemToDraw = 0;
		}

		void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
			if (sceneMode == DrawLine) {
				itemToDraw = 0;
				itemToDraw = new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));
				addItem(itemToDraw);
				itemToDraw->setPen(QPen(Qt::black, 3, Qt::SolidLine));
			}
			QGraphicsScene::mousePressEvent(event);
		}

		void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
			if (sceneMode == DrawLine && itemToDraw != 0) {
				QLineF newLine(itemToDraw->line().p1(), event->scenePos());
				itemToDraw->setLine(newLine);
			}
			else
				QGraphicsScene::mouseMoveEvent(event);
		}

		void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
			//Here is where you'll add the itemToDraw to the annotation list before resetting the poionter
			if (sceneMode == DrawLine && itemToDraw != 0) {
				removeItem(itemToDraw);
				emit line_finished(itemToDraw->line());
				sceneMode = SelectObject;
			}
			QGraphicsScene::mouseReleaseEvent(event);
		}

		void Scene::makeItemsControllable(bool areControllable) {
			foreach(QGraphicsItem* item, items()) {
				item->setFlag(QGraphicsItem::ItemIsSelectable,
					areControllable);
				item->setFlag(QGraphicsItem::ItemIsMovable,
					areControllable);
			}
		}

		void Scene::setMode(Mode mode) {
			sceneMode = mode;
			QGraphicsView::DragMode vMode =
				QGraphicsView::NoDrag;
			if (mode == DrawLine) {
				makeItemsControllable(false);
				vMode = QGraphicsView::NoDrag;
			}
			else if (mode == SelectObject) {
				makeItemsControllable(true);
				vMode = QGraphicsView::RubberBandDrag;
			}
			QGraphicsView* mView = views().at(0);
			if (mView)
				mView->setDragMode(vMode);
		}

		void Scene::keyPressEvent(QKeyEvent *event) {
			if (event->key() == Qt::Key_Delete)
				foreach(QGraphicsItem* item, selectedItems()) {
				removeItem(item);
				delete item;
			}
			else
				QGraphicsScene::keyPressEvent(event);
		}

        #include "../../include/fish_annotator/video_annotator/moc_scene.cpp"
	}
} // namespace fish_annotator

