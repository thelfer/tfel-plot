/*!
 * \file  GraphCurveItem.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 mai 2012
 */

#ifndef LIB_TFEL_PLOT_GRAPHCURVEITEM_H_
#define LIB_TFEL_PLOT_GRAPHCURVEITEM_H_

#include <QtWidgets/QGraphicsPathItem>

namespace tfel {

  namespace plot {

    struct Curve;

    struct Graph;

    struct GraphCurveItem : QObject, QGraphicsPathItem {
      GraphCurveItem(Graph *const, QGraphicsItem *const, Curve *const);

      GraphCurveItem(const QPainterPath &,
                     Graph *const,
                     QGraphicsItem *const,
                     Curve *const);

      virtual QPainterPath shape() const override;

     protected slots:

      void configure();

      void remove();

     protected:
      virtual void mouseDoubleClickEvent(
          QGraphicsSceneMouseEvent *) override;

      virtual void contextMenuEvent(
          QGraphicsSceneContextMenuEvent *) override;

      Graph *const graph;

      Curve *const curve;

     private:
      Q_OBJECT

    };  // end of struct GraphCurveItem

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPHCURVEITEM_H */
