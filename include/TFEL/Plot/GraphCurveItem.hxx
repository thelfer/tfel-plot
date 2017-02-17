/*! 
 * \file  GraphCurveItem.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 mai 2012
 */

#ifndef _LIB_TFEL_PLOT_GRAPHCURVEITEM_H_
#define _LIB_TFEL_PLOT_GRAPHCURVEITEM_H_ 

#ifdef TFEL_QT4
#include<QtGui/QGraphicsPathItem>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QGraphicsPathItem>
#endif /* TFEL_QT5 */


namespace tfel
{

  namespace plot
  {

    class Curve;

    class Graph;
    
    class GraphCurveItem
      : public QObject,
	public QGraphicsPathItem
    {
      
      Q_OBJECT

    public:

      GraphCurveItem(Graph         *const,
		     QGraphicsItem *const,
		     Curve         *const);
      
      GraphCurveItem(const QPainterPath &,
		     Graph         *const,
		     QGraphicsItem *const,
		     Curve         *const);
      
      virtual QPainterPath
      shape() const override;

    protected slots:

      void configure(void);

      void remove(void);

    protected:

      virtual void
      mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override;

      virtual void
      contextMenuEvent(QGraphicsSceneContextMenuEvent *) override;

      Graph *const graph;

      Curve *const curve;

    }; // end of class GraphCurveItem

  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_GRAPHCURVEITEM_H */

