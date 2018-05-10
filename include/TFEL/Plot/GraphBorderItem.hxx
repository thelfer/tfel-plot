/*!
 * \file  GraphBorderItem.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 oct. 2012
 */

#ifndef LIB_TFEL_PLOT_GRAPHBORDERITEM_H_
#define LIB_TFEL_PLOT_GRAPHBORDERITEM_H_

#include <QtWidgets/QGraphicsPathItem>
#include "TFEL/Plot/Graph.hxx"

namespace tfel {

  namespace plot {

    struct GraphBorderItem : QObject, QGraphicsPathItem {

      GraphBorderItem(const QPainterPath &,
                      Graph *const,
                      const Graph::GraphBorder,
                      const qreal);

      virtual QPainterPath shape() const override;

     protected slots:

      void configure();

     protected:
      virtual void contextMenuEvent(
          QGraphicsSceneContextMenuEvent *) override;

      virtual void mouseDoubleClickEvent(
          QGraphicsSceneMouseEvent *) override;

      Graph *const graph;

      const Graph::GraphBorder border;

      const qreal width;

     private:
      Q_OBJECT
    };  // end of struct GraphBorderItem

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPHBORDERITEM_H */
