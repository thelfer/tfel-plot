/*!
 * \file   ElegantGraphTheme.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#include<cstdlib>

#include<QtWidgets/QGraphicsTextItem>
#include<QtSvg/QGraphicsSvgItem>

#include"TFEL/Plot/ThemeManager.hxx"
#include"TFEL/Plot/GraphTextItem.hxx"
#include"TFEL/Plot/StandardGraphTheme.hxx"

namespace tfel
{

  namespace plot
  {
    
    struct ElegantGraphTheme
      : public StandardGraphTheme
    {

      void
      getGraphBorders(Graph&,
		      qreal& gl,qreal& gr,
		      qreal& gu,qreal& gd) const override
      {
	gl = 6;
	gr = 16;
	gu = 6;
	gd = 16;
      } // end of getGraphBorders

      void
      drawBackGround(Graph& g,
		     const GraphLayout&,
		     const GraphSize& s) const override
      {
	QGraphicsScene& cr = g.getScene();
	QLinearGradient grad(0.,0.,0.,s.width);
        QColor c1 = this->getFirstBoxColor();
        QColor c2 = this->getSecondBoxColor();
        grad.setColorAt(0,c1);
        grad.setColorAt(1,c2);
	cr.setBackgroundBrush(QBrush(grad));
	// QGraphicsSvgItem *i = new QGraphicsSvgItem("spacefun-wallpaper.svg");
	// cr.addItem(i);
      } // end of drawBackGround

      void
      drawGraphBackGround(Graph& g,
			  const GraphLayout&,
			  const GraphSize& s) const override
      {
	auto& cr = g.getScene();
	cr.addRect(s.xh0,s.yh0,
		   s.xh1-s.xh0,
		   s.yh1-s.yh0,
		   QPen(),Qt::white);
      } // end of drawBackGround

      virtual QColor getFirstBoxColor() const
      {
        return Qt::white;
      } // end of getFirstBoxColor

      virtual QColor getSecondBoxColor() const
      {
        return QColor(230,230,230);//QColor(114.75,114.75,114.75);
      } // end of getSecondBoxColor

      QColor getTextColor() const
      {
        return Qt::red;
      } // end of getTextColor

      virtual void
      drawHorizontalBox(QGraphicsScene& cr,
			const qreal x,
			const qreal y,
			const qreal w,
			const qreal h) const
      {
        QColor c1 = this->getFirstBoxColor();
        QColor c2 = this->getSecondBoxColor();
	QLinearGradient grad(0.,y,0.,y+0.75*h);
        grad.setColorAt(0,c1);
        grad.setColorAt(1,c2);
        QPainterPath path;
	const qreal x1 = x + h-10;
	const qreal x2 = x + w - h+10;
	path.moveTo(x1,y);
        path.lineTo(x2,y);
	path.arcTo(x2,y,h,h,90,-180);
        path.lineTo(x1,y+h);
	path.arcTo(x1,y+h,-h,-h,90,-180);
        path.lineTo(x1,y);
	cr.addPath(path,QPen(),QBrush(grad));
      } // end of drawHorizontalBox

      virtual void
      drawVerticalBox(QGraphicsScene& cr,
      				 const qreal x,
      				 const qreal y,
      				 const qreal w,
      				 const qreal h) const
      {
        QColor c1 = this->getFirstBoxColor();
        QColor c2 = this->getSecondBoxColor();
	QLinearGradient grad(0.,y,0.,y+0.75*h);
        grad.setColorAt(0,c1);
        grad.setColorAt(1,c2);
        QPainterPath path;
        path.moveTo(x,y);
        path.lineTo(x,y+h);
        path.arcTo(x,y+h,w,w,-180,180);
	path.lineTo(x+w,y);
	path.arcTo(x,y,w,-w,0,-180);
	path.lineTo(x,y);
	cr.addPath(path,QPen(),QBrush(grad));
      } // end of drawVerticalBox

      void
      printXLabel(Graph& g,
		  const GraphLayout& l,
		  const GraphSize& s,
		  const QString& xlabel) const override
      {
	QGraphicsScene& cr = g.getScene();
	QGraphicsTextItem *t = new GraphTextItem(xlabel,&g,
						 &Graph::setXLabel);
	const QRectF b = t->boundingRect();
	this->drawHorizontalBox(cr,s.xh0+0.5*(s.xh1-s.xh0)-0.5*b.width(),
				s.height-l.md-l.ttd-l.ld,
				b.width(),l.ld);
	t->setDefaultTextColor(this->getTextColor());
	t->setPos(s.xh0+0.5*(s.xh1-s.xh0)-0.5*b.width(),
		  s.height-l.md-l.ttd-0.5*(l.ld)-0.5*b.height());
	cr.addItem(t);
      } // end of ElegantGraphGraphTheme::printXLabel

      void
      printYLabel(Graph& g,
		  const GraphLayout& l,
		  const GraphSize& s,
		  const QString& ylabel) const override
      {
	auto& cr = g.getScene();
	auto * t = new QGraphicsTextItem(ylabel);
	t->setDefaultTextColor(this->getTextColor());
	t->setRotation(-90.);
	const QRectF b = t->boundingRect();
        this->drawVerticalBox(cr,l.ml+l.ttl,
			      s.yh0+0.5*(s.yh1-s.yh0)-0.5*b.width(),
			      l.ll,b.width());
	t->setPos(l.ml+l.ttl+0.5*(l.ll-b.height()),
		 s.yh0+0.5*(s.yh1-s.yh0)+0.5*b.width());
	cr.addItem(t);
      } // end of printYLabel

      void
      printX2Label(Graph& g,
		   const GraphLayout& l,
		   const GraphSize& s,
		   const QString& x2label) const override
      {
	auto& cr = g.getScene();
	auto*t = new QGraphicsTextItem(x2label);
	const QRectF b = t->boundingRect();
	t->setDefaultTextColor(this->getTextColor());
        this->drawHorizontalBox(cr,s.xh0+0.5*(s.xh1-s.xh0)-0.5*b.width(),
				l.mu+l.ttu,b.width(),l.lu);
	t->setPos(s.xh0+0.5*(s.xh1-s.xh0)-0.5*b.width(),
		  l.mu+l.ttu+0.5*l.lu-0.5*b.height()),
	cr.addItem(t);
      } // end of printX2Label

      void
      printY2Label(Graph& g,
		   const GraphLayout& l,
		   const GraphSize& s,
		   const QString& y2label) const override
      {
	auto& cr = g.getScene();
	auto* t  = new QGraphicsTextItem(y2label);
	t->setDefaultTextColor(this->getTextColor());
	t->setRotation(-90.);
	const QRectF b = t->boundingRect();
        this->drawVerticalBox(cr,s.width-l.mr-l.ttr-l.lr,
			      s.yh0+0.5*(s.yh1-s.yh0)-0.5*b.width(),
			      l.lr,b.width());
	t->setPos(s.width-l.mr-l.ttr-0.5*(l.lr+b.height()),
		  s.yh0+0.5*(s.yh1-s.yh0)+0.5*b.width());
	cr.addItem(t);
      } // end of printY2Label

      ~ElegantGraphTheme() override = default;

    };

    StandardThemeProxy<ElegantGraphTheme> elegantGraphThemeProxy("ElegantGraphTheme");

  } // end of namespace plot

} // end of namespace tfel


