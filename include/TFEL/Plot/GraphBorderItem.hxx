/*! 
 * \file  GraphBorderItem.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 oct. 2012
 */

#ifndef LIB_TFEL_PLOT_GRAPHBORDERITEM_H_
#define LIB_TFEL_PLOT_GRAPHBORDERITEM_H_ 

#ifdef TFEL_QT4
#include<QtGui/QGraphicsPathItem>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QGraphicsPathItem>
#endif /* TFEL_QT5 */

#include"TFEL/Plot/Graph.hxx"

namespace tfel
{

  namespace plot
  {

    class GraphBorderItem
      : public QObject,
	public QGraphicsPathItem
    {
      
      Q_OBJECT

    public:

      GraphBorderItem(const QPainterPath&,
		      Graph *const,
		      const Graph::GraphBorder,
		      const qreal);

      virtual QPainterPath
      shape() const override;

    protected slots:

      void configure(void);

    protected:

      virtual void
      contextMenuEvent(QGraphicsSceneContextMenuEvent *) override;

      virtual void
      mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;

      Graph *const graph;

      const Graph::GraphBorder border;

      const qreal width;

    }; // end of class GraphBorderItem

  } // end of namespace plot

} // end of namespace tfel


#endif /* LIB_TFEL_PLOT_GRAPHBORDERITEM_H */

