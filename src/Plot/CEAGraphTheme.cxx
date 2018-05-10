/*!
 * \file   CEAGraphTheme.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */


#include<QtCore/QDebug>
#include<QtWidgets/QGraphicsTextItem>
#include<QtSvg/QGraphicsSvgItem>
#include<QtSvg/QSvgRenderer>

#include"TFEL/Plot/ThemeManager.hxx"
#include"TFEL/Plot/GraphTextItem.hxx"
#include"TFEL/Plot/StandardGraphTheme.hxx"

namespace tfel
{

  namespace plot
  {
    
    struct CEAGraphTheme
      : public StandardGraphTheme
    {

      void
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

      qreal
      getUpperTitleHeight(Graph&,const QString&) const override
      {
	// the title is placed in
	// the graph border, if one
	return 0u;
      } // end of StandardGraphTheme::getUpperTitleHeight;

      void
      printUpperTitle(Graph& g,
		      const GraphLayout& l,
		      const GraphSize& s,
		      const QString& t) const override
      {
	auto& cr = g.getScene();
	QGraphicsTextItem *text = new GraphTextItem(&g,&Graph::setUpperTitle);
	text->setDefaultTextColor(Qt::white);
	text->setHtml("<H2>"+t+"</H2>");
	const QRectF b = text->boundingRect();
	text->setPos(s.xh0+0.5*(s.xh1-s.xh0-b.width()),
		     0.5*(l.gu-b.height()));
	cr.addItem(text);
      } // end of StandardGraphTheme::printUpperTitle

      void
      drawBackGround(Graph& g,
		     const GraphLayout&,
		     const GraphSize& s) const override
      {
	auto& cr = g.getScene();
	// upper banner
	QGraphicsSvgItem * i;
	i = new QGraphicsSvgItem();
	QSvgRenderer *r = new QSvgRenderer(QString(":/CEABanner2012.svg"));
	i->setSharedRenderer(r);
	QRectF ir = i->boundingRect();
	qreal sv;
	qreal sh;
	if(!g.getUpperTitle().isEmpty()){
	  sh = g.getGraphWidth()/ir.width();
	  sv = 75u/ir.height();
	} else {
	  qreal gh = qAbs(s.xh1-s.xh0);
	  qreal gv = qMin(qAbs(s.yh1-s.yh0),75*gh/g.getGraphWidth());
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
	// add CEA logo if not in the upper banner
	// if(g.getUpperTitle().isEmpty()){
	//   QGraphicsSvgItem *l;
	//   l = new QGraphicsSvgItem(":/CEALogo2012.svg");
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

      ~CEAGraphTheme() override = default;

    };

    StandardThemeProxy<CEAGraphTheme> CEAGraphThemeProxy("CEAGraphTheme");

  } // end of namespace plot

} // end of namespace tfel


