/*! 
 * \file  GraphBorderItem.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 29 mai 2012
 */

#include<QtCore/QtDebug>
#ifdef TFEL_QT4
#include<QtGui/QMenu>
#include<QtGui/QAction>
#include<QtGui/QGraphicsSceneContextMenuEvent>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QMenu>
#include<QtWidgets/QAction>
#include<QtWidgets/QGraphicsSceneContextMenuEvent>
#endif /* TFEL_QT5 */

#include"TFEL/Plot/GraphBorderItem.hxx"

namespace tfel
{

  namespace plot
  {

    static QString
    getGraphBorderName(const Graph::GraphBorder b)
    {
      if(b==Graph::X){
	return "X";
      } else if(b==Graph::X2){
	return "X2";
      } else if(b==Graph::Y){
	return "Y";
      } else if(b==Graph::Y2){
	return "Y2";
      }
      return "";
    } // end of getGraphBorderName

    GraphBorderItem::GraphBorderItem(const QPainterPath& qpath,
				     Graph         *const g,
				     const Graph::GraphBorder b,
				     const qreal w)
      : QGraphicsPathItem(qpath),
	graph(g),
	border(b),
	width(w)
    {}

    void
    GraphBorderItem::configure()
    {
      this->graph->configureGraphRange(this->border);
    } // edn of GraphBorderItem::configure

    QPainterPath
    GraphBorderItem::shape() const
    {
      QPainterPathStroker ps;
      return ps.createStroke(this->path());
    }

    void
    GraphBorderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * e)
    {
      if(e->button()==Qt::LeftButton){
	QPointF pev = e->pos();
	const qreal w = this->width;
	pev.rx() -= 0.5*w+1;
	pev.ry() -= 0.5*w+1;
	QRectF r(pev,QSizeF(w+2,w+2));
	if(this->shape().intersects(r)){
	  this->configure();
	} else {
	  e->ignore();
	}
      } else {
	e->ignore();
      }
    } // end of GraphBorderItem::mouseDoubleClickEvent

    void
    GraphBorderItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * ev)
    {
      QPointF pev   = ev->pos();
      const qreal w = this->width;
      pev.rx() -= 2.*w;
      pev.ry() -= 2.*w;
      QRectF r(pev,QSizeF(4*w,4*w));
      if(this->shape().intersects(r)){
	QMenu   menu;
	QAction *ca  = menu.addAction(tr("Configure")+" "+
				      getGraphBorderName(this->border)+" range");
	QObject::connect(ca, SIGNAL(triggered()),
			 this, SLOT(configure()));
	QAction *ca2  = menu.addAction(tr("Configure graph ranges"));
	QObject::connect(ca2, SIGNAL(triggered()),
			 this->graph, SLOT(configureGraphRanges()));
	menu.exec(ev->screenPos());
      } else {
	ev->ignore();
      }
    }
    
  } // end of namespace plot

} // end of namespace tfel
