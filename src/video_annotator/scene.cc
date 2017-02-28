#include "fish_annotator/video_annotator/scene.h"

namespace fish_annotator {
  namespace video_annotator {

    Scene::Scene(QObject* parent) : QGraphicsScene(parent)
    {
      scene_mode_ = NoMode;
      item_to_draw_ = 0;
    }

    void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
      if (scene_mode_ == DrawLine) {
        item_to_draw_ = new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));
        addItem(item_to_draw_);
        item_to_draw_->setPen(QPen(Qt::black, 7, Qt::SolidLine));
      }
      QGraphicsScene::mousePressEvent(event);
    }

    void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
      if (scene_mode_ == DrawLine && item_to_draw_ != 0) {
        QLineF newLine(item_to_draw_->line().p1(), event->scenePos());
        item_to_draw_->setLine(newLine);
      }
      else
        QGraphicsScene::mouseMoveEvent(event);
    }
	  void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
		  //Here is where you'll add the item_to_draw_ to the annotation list before resetting the poionter
		  if (scene_mode_ == DrawLine && item_to_draw_ != 0) {
        line_to_draw_ = item_to_draw_->line();
			  emit lineFinished(line_to_draw_);
			  removeItem(item_to_draw_);
        item_to_draw_ = 0;
			  scene_mode_ = SelectObject;
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
      scene_mode_ = mode;
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

