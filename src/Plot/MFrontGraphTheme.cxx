/*!
 * \file   MFrontGraphTheme.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */


#include<QtCore/QDebug>
#ifdef TFEL_QT4
#include<QtGui/QGraphicsTextItem>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QGraphicsTextItem>
#endif /* TFEL_QT5 */
#include<QtSvg/QGraphicsSvgItem>
#include<QtSvg/QSvgRenderer>

#include"TFEL/Plot/ThemeManager.hxx"
#include"TFEL/Plot/GraphTextItem.hxx"
#include"TFEL/Plot/StandardGraphTheme.hxx"

namespace tfel
{

  namespace plot
  {
    
    struct MFrontGraphTheme
      : public StandardGraphTheme
    {

      virtual void
      getGraphBorders(Graph& g,
		      qreal& gl,qreal& gr,
		      qreal& gu,qreal& gd) const override
      {
	StandardGraphTheme::getGraphBorders(g,gl,gr,gu,gd);
	if(!g.getUpperTitle().isEmpty()){
	  gu = 75u;
	}
	//   gu = 30u;
	// }
      } // end of getGraphBorders

      virtual qreal
      getUpperTitleHeight(Graph&,const QString&) const override
      {
	// the title is placed in
	// the graph border, if one
	return 0u;
      } // end of StandardGraphTheme::getUpperTitleHeight;

      virtual void
      printUpperTitle(Graph& g,
		      const GraphLayout& l,
		      const GraphSize& s,
		      const QString& t) const override
      {
	QGraphicsScene& cr = g.getScene();
	QGraphicsTextItem *text = new GraphTextItem(&g,&Graph::setUpperTitle);
	text->setDefaultTextColor(Qt::black);
	text->setHtml("<H2>"+t+"</H2>");
	const QRectF b = text->boundingRect();
	text->setPos(s.xh0+0.5*(s.xh1-s.xh0-b.width()),
		     0.5*(l.gu-b.height()));
	cr.addItem(text);
      } // end of StandardGraphTheme::printUpperTitle

      virtual void
      drawBackGround(Graph& g,
		     const GraphLayout&,
		     const GraphSize& s) const override
      {
	QGraphicsScene& cr = g.getScene();
	// upper banner
	QGraphicsSvgItem * i;
	i = new QGraphicsSvgItem();
	QSvgRenderer *r = new QSvgRenderer(QString(":/mfront-en.svg"));
	i->setSharedRenderer(r);
	QRectF ir = i->boundingRect();
	qreal sv;
	qreal sh;
	if(!g.getUpperTitle().isEmpty()){
	  sh = sv = 75u/ir.height();
	} else {
	  qreal gh = qAbs(s.xh1-s.xh0);
	  qreal gv = qAbs(s.yh1-s.yh0);
	  sh = gh/ir.width();
	  sv = gv/ir.height();
	  i->setPos(s.xh0,s.yh0);
	}
	QTransform t;
	t.scale(sh,sv);
	i->setTransform(t);
	cr.addItem(i);
	  //	if(!g.getUpperTitle().isEmpty()){
	// } else {
	//   sv = 30u/r.height();
	// }
	// add MFront logo if not in the upper banner
	// if(g.getUpperTitle().isEmpty()){
	//   QGraphicsSvgItem *l;
	//   l = new QGraphicsSvgItem(":/MFrontLogo2012.svg");
	//   // hint of the graph size
	//   // size of logo
	//   QRectF lr = l->boundingRect();
	//   qreal  lw = gs*0.2;
	//   qreal  lh = lw*lr.height()/lr.width();
	//   QTransform t;
	//   t.scale(lw/lr.width(),lh/lr.height());
	//   l->setTransform(t);
	//   cr.addItem(l);
	// }	
      } // end of StandardGraphTheme::drawBackGround

      virtual ~MFrontGraphTheme()
      {} // end of ~MFrontGraphTheme

    };

    StandardThemeProxy<MFrontGraphTheme> MFrontGraphThemeProxy("MFrontGraphTheme");

  } // end of namespace plot

} // end of namespace tfel


