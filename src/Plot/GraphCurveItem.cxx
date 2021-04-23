/*!
 * \file  GraphCurveItem.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 29 mai 2012
 */

#include <QtCore/QtDebug>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QGraphicsSceneContextMenuEvent>

#include "TFEL/Plot/Graph.hxx"
#include "TFEL/Plot/Curve.hxx"
#include "TFEL/Plot/CurveConfigurationDialog.hxx"
#include "TFEL/Plot/GraphCurveItem.hxx"

namespace tfel {

  namespace plot {

    GraphCurveItem::GraphCurveItem(Graph *const g,
                                   QGraphicsItem *const d,
                                   Curve *const c)
        : QGraphicsPathItem(d), graph(g), curve(c) {}

    GraphCurveItem::GraphCurveItem(const QPainterPath &qpath,
                                   Graph *const g,
                                   QGraphicsItem *const d,
                                   Curve *const c)
        : QGraphicsPathItem(qpath, d), graph(g), curve(c) {}

    void GraphCurveItem::configure() {
      CurveConfigurationDialog *d;
      d = new CurveConfigurationDialog(this->curve, this->graph);
      d->setModal(false);
      d->show();
    }

    void GraphCurveItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e) {
      if (e->button() == Qt::LeftButton) {
        QPointF pev = e->pos();
        const qreal w = this->curve->getWidth();
        pev.rx() -= 0.5 * w + 1;
        pev.ry() -= 0.5 * w + 1;
        QRectF r(pev, QSizeF(w + 2, w + 2));
        if (this->shape().intersects(r)) {
          this->configure();
        } else {
          e->ignore();
        }
      } else {
        e->ignore();
      }
    }  // end of GraphCurveItem::mouseDoubleClickEvent

    void GraphCurveItem::remove() { this->curve->sendRemoveSignal(); }

    QPainterPath GraphCurveItem::shape() const {
      QPainterPathStroker ps;
      return ps.createStroke(this->path());
    }

    void GraphCurveItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *ev) {
      QPointF pev = ev->pos();
      const qreal w = this->curve->getWidth();
      pev.rx() -= w;
      pev.ry() -= w;
      QRectF r(pev, QSizeF(2 * w, 2 * w));
      if (this->shape().intersects(r)) {
        QMenu menu;
        auto *ca = menu.addAction(tr("Configure"));
        QObject::connect(ca, &QAction::triggered, this,
                         &GraphCurveItem::configure);
        auto *ra = menu.addAction(tr("Remove"));
        ra->setStatusTip(tr("Remove the selected curve"));
        QObject::connect(ra, &QAction::triggered, this,
                         &GraphCurveItem::remove);
        menu.exec(ev->screenPos());
      } else {
        ev->ignore();
      }
    }

  }  // end of namespace plot

}  // end of namespace tfel
