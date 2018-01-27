/*!
 * \file   StandardGraphTheme.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#include<sstream>
#include<cmath>
#include<limits>

#include<QtCore/QtDebug>
#include<QtWidgets/QGraphicsTextItem>

#include"TFEL/Plot/GraphCurveItem.hxx"
#include"TFEL/Plot/GraphTextItem.hxx"
#include"TFEL/Plot/GraphKeyItem.hxx"
#include"TFEL/Plot/GraphBorderItem.hxx"
#include"TFEL/Plot/ThemeManager.hxx"
#include"TFEL/Plot/StandardGraphTheme.hxx"

namespace tfel
{

  namespace plot
  {

    const qreal StandardGraphTheme::defaultRedColors[10]   = {255.,112.,255., 20.,204.,140.,  0.,125.,  0.,  0.};
    const qreal StandardGraphTheme::defaultGreenColors[10] = {179.,188.,  0., 30.,  0.,120.,160.,  0., 60.,  0.};
    const qreal StandardGraphTheme::defaultBlueColors[10]  = { 16., 16.,  0.,255.,192., 80.,160.,  0.,  0.,125.};
    
    const unsigned short
    StandardGraphTheme::defaultColorsNumber = 10u;

    static bool isInvalidValidQReal(const qreal x)
    {
      if(std::isnan(x)){
	return true;
      } else if(x>=std::numeric_limits<qreal>::max()){
	return true;
      } else if(x<=-std::numeric_limits<qreal>::max()){
	return true;
      }
      return false;      
    } // end of isInvalidValidQReal
    
    qreal StandardGraphTheme::getLeftMargin() const
    {
      return 15;
    } // end of StandardGraphTheme::getLeftMargin()

    qreal StandardGraphTheme::getRightMargin() const
    {
      return 15;
    } // end of StandardGraphTheme::getRightMargin

    qreal StandardGraphTheme::getUpperMargin() const
    {
      return 15;
    } // end of StandardGraphTheme::getUpperMargin

    qreal StandardGraphTheme::getDownMargin() const
    {
      return 15;
    } // end of StandardGraphTheme::getDownMargin
    
    qreal
    StandardGraphTheme::getYTicsWidth(Graph&,
				      const QMap<qreal,QString>& tics) const
    {
      if(tics.empty()){
      	return 0;
      }
      qreal res = 0.;
      for(const auto& tic : tics){
	QGraphicsTextItem t;
	t.setHtml(tic);
	auto s = t.boundingRect();
	res = std::max(res,s.width());
      }
      return static_cast<qreal>(res+8u);
    } // end of StandardGraphTheme::getYTicsWidth

    qreal
    StandardGraphTheme::getY2TicsWidth(Graph& g,
				       const QMap<qreal,QString>& tics) const
    {
      return this->getYTicsWidth(g,tics);
    } // end of StandardGraphTheme::getY2TicsWidth
    
    qreal
    StandardGraphTheme::getXTicsHeight(Graph&,
				       const QMap<qreal,QString>& tics) const
    {
      if(tics.empty()){
      	return 0;
      }
      qreal res = 0.;
      for(const auto& tic : tics){
	QGraphicsTextItem t;
	t.setHtml(tic);
	auto s = t.boundingRect();
	res = std::max(res,s.height());
      }
      return static_cast<qreal>(res+8u);
    } // end of StandardGraphTheme::getXTicsHeight

    qreal
    StandardGraphTheme::getX2TicsHeight(Graph& g,
					const QMap<qreal,QString>& tics) const
    {
      return this->getXTicsHeight(g,tics);
    } // end of StandardGraphTheme::getX2TicsHeight

    void
    StandardGraphTheme::getGraphBorders(Graph&,
					qreal& gl,qreal& gr,
					qreal& gu,qreal& gd) const
    {
      gl = 6u;
      gr = 6u;
      gu = 6u;
      gd = 6u;
    } // end of StandardGraphTheme::getGraphBorders

    void
    StandardGraphTheme::printXLabel(Graph& g,
				    const GraphLayout& l,
				    const GraphSize& s,
				    const QString& xlabel) const
    {
      auto& cr = g.getScene();
      auto* t = new GraphTextItem(xlabel,&g,
				  &Graph::setXLabel);
      const auto b = t->boundingRect();
      t->setPos(s.xh0+0.5*(s.xh1-s.xh0)-0.5*b.width(),
		s.height-l.md-l.ttd-0.5*(l.ld)-0.5*b.height());
      cr.addItem(t);
    } // end of StandardGraphTheme::printXLabel

    void
    StandardGraphTheme::printYLabel(Graph& g,
				    const GraphLayout& l,
				    const GraphSize& s,
				    const QString& ylabel) const
    {
      auto& cr = g.getScene();
      auto* t = new GraphTextItem(ylabel,&g,
				  &Graph::setYLabel);
      t->setRotation(-90.);
      const auto b = t->boundingRect();
      t->setPos(l.ml+l.ttl+0.5*(l.ll-b.height()),
		s.yh0+0.5*(s.yh1-s.yh0)+0.5*b.width());
      cr.addItem(t);
    } // end of StandardGraphTheme::printYLabel

    void
    StandardGraphTheme::printX2Label(Graph& g,
				     const GraphLayout& l,
				     const GraphSize& s,
				     const QString& x2label) const
    {
      auto& cr = g.getScene();
      auto* t = new GraphTextItem(x2label,&g,
				  &Graph::setX2Label);
      const auto b = t->boundingRect();
      t->setPos(s.xh0+0.5*(s.xh1-s.xh0)-0.5*b.width(),
		l.mu+l.ttu+0.5*l.lu-0.5*b.height());
      cr.addItem(t);
    } // end of StandardGraphTheme::printX2Label

    void
    StandardGraphTheme::printY2Label(Graph& g,
				     const GraphLayout& l,
				     const GraphSize& s,
				     const QString& y2label) const
    {
      auto& cr = g.getScene();
      auto* t = new GraphTextItem(y2label,&g,
				  &Graph::setY2Label);
      const auto b = t->boundingRect();
      t->setRotation(-90);
      t->setPos(s.width-l.mr-l.ttr-0.5*(l.lr+b.height()),
		s.yh0+0.5*(s.yh1-s.yh0)+0.5*b.width());
      cr.addItem(t);
    } // end of StandardGraphTheme::printY2Label

    void
    StandardGraphTheme::printXTics(Graph& g,
				   const GraphLayout& l,
				   const GraphSize& s,
				   const QMap<qreal,QString>& xtics) const
    {
      auto& cr = g.getScene();
      QMapIterator<qreal,QString> p(xtics);
      while(p.hasNext()){
	p.next();
	auto* t = cr.addText("");
	t->setHtml(p.value());
	t->setFont(g.getGraphFont());
	const auto b = t->boundingRect();
      	t->setPos((p.key()-l.bx)/l.ax-0.5*b.width(),
		  s.height-l.md-l.ttd-l.ld-0.5*(l.td)-0.5*b.height());
      }
    } // end of StandardGraphTheme::printXTics

    void
    StandardGraphTheme::printYTics(Graph& g,
				   const GraphLayout& l,
				   const GraphSize&,
				   const QMap<qreal,QString>& ytics) const
    {
      auto& cr = g.getScene();
      QMapIterator<qreal,QString> p(ytics);
      while(p.hasNext()){
	p.next();
	auto* t = cr.addText("");
	t->setHtml(p.value());
	t->setFont(g.getGraphFont());
	const auto b = t->boundingRect();
      	t->setPos(l.ml+l.ttl+l.ll+0.5*(l.tl)-0.5*b.width(),
		  l.ay*p.key()+l.by-0.5*b.height());
      }
    } // end of StandardGraphTheme::printYTics

    void
    StandardGraphTheme::printX2Tics(Graph& g,
				    const GraphLayout& l,
				    const GraphSize&,
				    const QMap<qreal,QString>& x2tics) const
    {
      auto& cr = g.getScene();
      QMapIterator<qreal,QString> p(x2tics);
      while(p.hasNext()){
	p.next();
	auto* t = cr.addText("");
	t->setHtml(p.value());
	t->setFont(g.getGraphFont());
	const auto b = t->boundingRect();
      	t->setPos((p.key()-l.bx2)/l.ax2-0.5*b.width(),
		  l.mu+l.ttu+l.lu+0.5*(l.tu)-0.5*b.height());
      }
    } // end of StandardGraphTheme::printX2Tics

    void
    StandardGraphTheme::printY2Tics(Graph& g,
				    const GraphLayout& l,
				    const GraphSize& s,
				    const QMap<qreal,QString>& y2tics) const
    {
      auto& cr = g.getScene();
      QMapIterator<qreal,QString> p(y2tics);
      while(p.hasNext()){
	p.next();
	auto* t = cr.addText("");
	t->setHtml(p.value());
	t->setFont(g.getGraphFont());
	const auto b = t->boundingRect();
      	t->setPos(s.width-l.mr-l.ttr-l.lr-0.5*(l.tr+b.width()),
		  l.ay2*(p.key())+l.by2-0.5*b.height());
      }
    } // end of StandardGraphTheme::printY2Tics

    void
    StandardGraphTheme::printUpperTitle(Graph& g,
					const GraphLayout& l,
					const GraphSize& s,
					const QString& t) const
    {
      auto& cr = g.getScene();
      auto* text = new GraphTextItem(&g,&Graph::setUpperTitle);
      text->setHtml("<H2>"+t+"</H2>");
      const auto b = text->boundingRect();
      text->setPos(s.xh0+0.5*(s.xh1-s.xh0-b.width()),
		   l.mu+0.5*(l.ttu-b.height()));
      cr.addItem(text);
    } // end of StandardGraphTheme::printUpperTitle

    void
    StandardGraphTheme::printDownTitle(Graph& g,
				       const GraphLayout& l,
				       const GraphSize& s,
				       const QString& t) const
    {
      auto& cr = g.getScene();
      auto* text = new GraphTextItem(&g,&Graph::setDownTitle);
      text->setHtml("<H2>"+t+"</H2>");
      const auto b = text->boundingRect();
      text->setPos(s.xh0+0.5*(s.xh1-s.xh0-b.width()),
		   s.height-l.md-0.5*(l.ttl+b.height()));
      cr.addItem(text);
    } // end of StandardGraphTheme::printDownTitle

    void
    StandardGraphTheme::printLeftTitle(Graph& g,
				       const GraphLayout& l,
				       const GraphSize& s,
				       const QString& t) const
    {
      auto& cr = g.getScene();
      auto* text = new GraphTextItem(&g,&Graph::setLeftTitle);
      text->setHtml("<H2>"+t+"</H2>");
      const auto b = text->boundingRect();
      text->setRotation(-90);
      text->setPos(l.ml+l.ttl-2-b.height(),
		   s.yh0+0.5*(s.yh1-s.yh0)+0.5*b.width());
      cr.addItem(text);
    } // end of StandardGraphTheme::printLeftTitle

    void
    StandardGraphTheme::printRightTitle(Graph& g,
					const GraphLayout& l,
					const GraphSize& s,
					const QString& t) const
    {
      auto& cr = g.getScene();
      auto* text = new GraphTextItem(&g,&Graph::setRightTitle);
      text->setHtml("<H2>"+t+"</H2>");
      const auto b = text->boundingRect();
      text->setRotation(-90.);
      text->setPos(s.width-l.mr-0.5*(l.ttr)-0.5*b.height(),
		   s.yh0+0.5*(s.yh1-s.yh0)+0.5*b.width());
      cr.addItem(text);
    } // end of StandardGraphTheme::printRightTitle

    void
    StandardGraphTheme::drawForeGround(Graph&,
				       const GraphLayout&,
				       const GraphSize&) const
    {} // end of StandardGraphTheme::drawForeGround

    void
    StandardGraphTheme::drawBackGround(Graph& g,
				       const GraphLayout&,
				       const GraphSize&) const
    {
      auto& cr = g.getScene();
      QBrush brush(QColor(255.,255.,255.));
      cr.setBackgroundBrush (brush);
    } // end of StandardGraphTheme::drawBackGround

    void
    StandardGraphTheme::drawGraphForeGround(Graph&,
					    const GraphLayout&,
					    const GraphSize&) const
    {} // end of StandardGraphTheme::drawGraphForeGround

    void
    StandardGraphTheme::drawGraphBackGround(Graph&,
					    const GraphLayout&,
					    const GraphSize&) const
    {} // end of StandardGraphTheme::drawGraphBackGround

    void
    StandardGraphTheme::drawXAxis(QGraphicsItem& d,
				  const GraphLayout& l,
				  const GraphSize& s) const
    {
      QPainterPath path;
      QPen pen;
      pen.setWidth(2.0);
      path.moveTo(s.xh0,l.by);
      path.lineTo(s.xh1,l.by);
      auto pi = new QGraphicsPathItem(&d);
      pi->setPath(path);
      pi->setPen(pen);
    } // end of StandardGraphTheme::drawXAxis

    void
    StandardGraphTheme::drawYAxis(QGraphicsItem& d,
				  const GraphLayout& l,
				  const GraphSize& s) const
    {
      QPainterPath path;
      QPen pen;
      pen.setWidth(2.0);
      path.moveTo(-l.bx/l.ax,s.yh0);
      path.lineTo(-l.bx/l.ax,s.yh1);
      auto pi = new QGraphicsPathItem(&d);
      pi->setPath(path);
      pi->setPen(pen);
    } // end of StandardGraphTheme::drawYAxis

    void
    StandardGraphTheme::drawGraphBorder(Graph& g,
					const GraphLayout&,
					const GraphSize& s) const
    {
      const qreal w = 2.0;
      QPen pen;
      pen.setWidth(w);
      // creating path
      QPainterPath py;
      py.moveTo(s.xh0,s.yh0);
      py.lineTo(s.xh0,s.yh1);
      QPainterPath px;
      px.moveTo(s.xh0,s.yh1);
      px.lineTo(s.xh1,s.yh1);
      QPainterPath py2;
      py2.moveTo(s.xh1,s.yh1);
      py2.lineTo(s.xh1,s.yh0);
      QPainterPath px2;
      px2.moveTo(s.xh1,s.yh0);
      px2.lineTo(s.xh0,s.yh0);
      //  
      auto& cr = g.getScene();
      auto *gpx  = new GraphBorderItem(px, &g,Graph::X, w);
      auto *gpx2 = new GraphBorderItem(px2,&g,Graph::X2,w);
      auto *gpy  = new GraphBorderItem(py, &g,Graph::Y, w);
      auto *gpy2 = new GraphBorderItem(py2,&g,Graph::Y2,w);
      gpx->setPen(pen);
      gpx2->setPen(pen);
      gpy->setPen(pen);
      gpy2->setPen(pen);
      cr.addItem(gpx);
      cr.addItem(gpx2);
      cr.addItem(gpy);
      cr.addItem(gpy2);
    } // end of StandardGraphTheme::drawGraphBorder

    void
    StandardGraphTheme::drawHorizontalGrid(Graph& g,
					   const QPen& pen,
					   const GraphLayout& l,
					   const GraphSize& s,
					   const QMap<qreal,QString>& tics,
					   const qreal tmin,const qreal tmax) const
    {
      auto& cr = g.getScene();
      QMapIterator<qreal,QString> p(tics);
      QPainterPath path;
      while(p.hasNext()){
	p.next();
      	if((p.key()>=tmin)&&(p.key()<=tmax)){
      	  path.moveTo((p.key()-l.bx)/l.ax,s.yh0);
      	  path.lineTo((p.key()-l.bx)/l.ax,s.yh1);
      	}
      }
      cr.addPath(path,pen);
    } // end of StandardGraphTheme::drawHorizontalGrid

    void
    StandardGraphTheme::drawVerticalGrid(Graph& g,
					 const QPen& pen,
					 const GraphLayout& l,
					 const GraphSize& s,
					 const QMap<qreal,QString>& tics,
					 const qreal tmin,const qreal tmax) const
    {
      auto& cr = g.getScene();
      QMapIterator<qreal,QString> p(tics);
      QPainterPath path;
      while(p.hasNext()){
	p.next();
      	if((p.key()>=tmin)&&(p.key()<=tmax)){
      	  path.moveTo(s.xh0,l.ay*(p.key())+l.by);
      	  path.lineTo(s.xh1,l.ay*(p.key())+l.by);
      	}
      }
      cr.addPath(path,pen);
    } // end of StandardGraphTheme::drawVerticalGrid

    void
    StandardGraphTheme::drawHorizontalGridMark(Graph& g,
					       const QPen& pen, 
					       const GraphLayout& l,
					       const GraphSize& s,
					       const QMap<qreal,QString>& tics,
					       const qreal tmin,const qreal tmax) const
    {
      auto& cr = g.getScene();
      QMapIterator<qreal,QString> p(tics);
      QPainterPath path;
      while(p.hasNext()){
	p.next();
      	if((p.key()>=tmin)&&(p.key()<=tmax)){
      	  path.moveTo((p.key()-l.bx)/l.ax,s.yh0);
      	  path.lineTo((p.key()-l.bx)/l.ax,s.yh0+10.);
      	  path.moveTo((p.key()-l.bx)/l.ax,s.yh1);
      	  path.lineTo((p.key()-l.bx)/l.ax,s.yh1-10.);
      	}
      }
      cr.addPath(path,pen);
    } // end of StandardGraphTheme::drawHorizontalGridMark

    void
    StandardGraphTheme::drawVerticalGridMark(Graph& g,
					     const QPen& pen,
					     const GraphLayout& l,
					     const GraphSize& s,
					     const QMap<qreal,QString>& tics,
					     const qreal tmin,const qreal tmax) const
    {
      auto& cr = g.getScene();
      QMapIterator<qreal,QString> p(tics);
      QPainterPath path;
      while(p.hasNext()){
	p.next();
      	if((p.key()>=tmin)&&(p.key()<=tmax)){
      	  path.moveTo(s.xh0,l.ay*(p.key())+l.by);
      	  path.lineTo(s.xh0+10,l.ay*(p.key())+l.by);
      	  path.moveTo(s.xh1,l.ay*(p.key())+l.by);
      	  path.lineTo(s.xh1-10,l.ay*(p.key())+l.by);
      	}
      }
      cr.addPath(path,pen);
    } // end of StandardGraphTheme::drawVerticalGridMark

    void
    StandardGraphTheme::drawGrid(Graph& g,
				 const GraphLayout& l,
				 const GraphSize& s,
				 const bool showGrid,
				 const unsigned short grid,
				 const QMap<qreal,QString>& xtics,
				 const QMap<qreal,QString>& ytics,
				 const QMap<qreal,QString>& x2tics,
				 const QMap<qreal,QString>& y2tics,
				 const qreal xmin,  const qreal xmax,
				 const qreal ymin,  const qreal ymax,
				 const qreal x2min, const qreal x2max,
				 const qreal y2min, const qreal y2max,
				 const bool x , const bool y,
				 const bool x2, const bool y2) const
    {
      if(showGrid){
	QPen pen;
	pen.setDashPattern(QVector<qreal>() << 4. << 8.);
	pen.setWidth(1.0);
	pen.setColor(QColor(75.,87.,85.));
      	if((grid&Grid::X)&&(x)){
      	  this->drawHorizontalGrid(g,pen,l,s,xtics,xmin,xmax);
      	}
      	if((grid&Grid::Y)&&(y)){
      	  this->drawVerticalGrid(g,pen,l,s,ytics,ymin,ymax);
      	}
      	if((grid&Grid::X2)&&(x2)){
      	  this->drawHorizontalGrid(g,pen,l,s,x2tics,x2min,x2max);
      	}
      	if((grid&Grid::Y2)&&(y2)){
      	  this->drawVerticalGrid(g,pen,l,s,y2tics,y2min,y2max);
      	}
      }
      QPen pen;
      pen.setWidth(2.0);
      pen.setColor(QColor(0.,0.,0.));
      if(x){
      	this->drawHorizontalGridMark(g,pen,l,s,xtics,xmin,xmax);
      }
      if(y){
      	this->drawVerticalGridMark(g,pen,l,s,ytics,ymin,ymax);
      }
      if(x2){
      	this->drawHorizontalGridMark(g,pen,l,s,x2tics,x2min,x2max);
      }
      if(y2){
      	this->drawVerticalGridMark(g,pen,l,s,y2tics,y2min,y2max);
      }
    } // end of StandardGraphTheme::drawGrid

    void
    StandardGraphTheme::fillQPainterPath(QPainterPath& path,
					 const Curve& curve,
					 const QVector<Point>& pts,
					 const qreal ax,
					 const qreal bx,
					 const qreal ay,
					 const qreal by) const
    {
      const qreal w  = curve.getWidth();
      const qreal w2 = w*w;
      bool found;
      bool previous;
      qreal flcx = 0.; // local coordinate of the first point
      qreal flcy = 0.; // local coordinate of the first point
      qreal plcx; // local coordinate of the previous point
      qreal plcy; // local coordinate of the previous point
      if(pts.empty()){
      	return;
      }
      auto p  = pts.begin();
      const auto pe = pts.end();
      found=false;
      previous=false;
      while((p!=pe)&&(!found)){
      	if(!(isInvalidValidQReal(p->x)||isInvalidValidQReal(p->y))){
	  flcx = (p->x-bx)/ax;
	  flcy = ay*(p->y)+by;
	  path.moveTo(flcx,flcy);
	  found=true;
      	  previous=true;
      	}
      	++p;
      }
      if(!found){
	return;
      }
      bool closed = true;
      plcx = flcx;
      plcy = flcy;
      while(p!=pe){
      	if(!(isInvalidValidQReal(p->x)||isInvalidValidQReal(p->y))){
	  const qreal lcx = (p->x-bx)/ax;  // local coordinate
	  const qreal lcy = ay*(p->y)+by; // local coordinate
	  const qreal dx = (lcx-plcx);
	  const qreal dy = (lcy-plcy);
	  if(previous){
	    if(dx*dx+dy*dy>w2){
	      path.lineTo(lcx,lcy);
	      plcx = lcx;
	      plcy = lcy;
	    }
	    closed = false;
	  } else {
	    path.moveTo(lcx,lcy);
	    previous = true;
	    closed   = true;
	  }
	} else {
      	  previous=false;
      	}
      	++p;
      }
      if(curve.isFilled()){
	path.lineTo(plcx,by);
	path.lineTo(flcx,by);
	path.lineTo(flcx,flcy);
      } else {
	if(!closed){
	  path.moveTo(plcx,plcy);
	}
      }
    } // end of StandardGraphTheme::fillQPainterPath

    void
    StandardGraphTheme::drawCurveWithLines(Graph& g,
					   QGraphicsItem& d,
					   Curve *const curve,
					   const QVector<Point>& pts,
					   const QColor& c,
					   const qreal ax,
					   const qreal bx,
					   const qreal ay,
					   const qreal by,
					   const Qt::PenStyle s) const
    {
      QPainterPath path;
      this->fillQPainterPath(path,*curve,pts,ax,bx,ay,by);
      const qreal w  = curve->getWidth();
      QPen pen(c);
      pen.setWidth(2*w);
      pen.setStyle(s);
      auto* pi = new GraphCurveItem(&g,&d,curve);
      pi->setPath(path);
      pi->setPen(pen);
      if(curve->isFilled()){
	QColor fc(c);
	fc.setAlpha(c.alpha()*0.75);
	QBrush b(fc);
	pi->setBrush(b);
      }
    } // end of StandardGraphTheme::drawCurveWithLines

    void
    StandardGraphTheme::drawCurveWithSquares(Graph& g,
					     QGraphicsItem& d,
					     Curve *const curve,
					     const QVector<Point>& pts,
					     const QColor& c,
					     const qreal ax,
					     const qreal bx,
					     const qreal ay,
					     const qreal by) const
    {
      const qreal w = curve->getWidth();
      const auto& pen = QPen(c);
      const auto& brush = QBrush(c);
      const qreal l = 3*w;
      if(pts.empty()){
      	return;
      }
      auto p=pts.begin();
      QPainterPath path;
      while(p!=pts.end()){
      	if(!(isInvalidValidQReal(p->x)||isInvalidValidQReal(p->y))){
	  const qreal x0 = (p->x-bx)/ax;
	  const qreal x1 = ay*(p->y)+by;
	  path.addRect(x0-l,x1-l,2*l,2*l);
	}
      	++p;
      }
      auto* pi = new GraphCurveItem(&g,&d,curve);
      pi->setPath(path);
      pi->setPen(pen);
      pi->setBrush(brush);
    } // end of StandardGraphTheme::drawCurveWithSquares

    void
    StandardGraphTheme::drawCurveWithTriangles(Graph& g,
					       QGraphicsItem& d,
					       Curve *const curve,
					       const QVector<Point>& pts,
					       const QColor& c,
					       const qreal ax,
					       const qreal bx,
					       const qreal ay,
					       const qreal by) const
    {
      const qreal w = curve->getWidth();
      const qreal extent = 3u*w;
      if(pts.empty()){
      	return;
      }
      QPainterPath path;
      auto p=pts.begin();
      while(p!=pts.end()){
      	if(!(isInvalidValidQReal(p->x)||isInvalidValidQReal(p->y))){
	  const qreal x0 = (p->x-bx)/ax;
	  const qreal x1 = ay*(p->y)+by;
	  path.moveTo(x0,x1-extent);
	  path.lineTo(x0-extent,x1+extent);
	  path.lineTo(x0+extent,x1+extent);
	  path.lineTo(x0,x1-extent);
	}
      	++p;
      }
      auto* pi = new GraphCurveItem(&g,&d,curve);
      pi->setPath(path);
      pi->setPen(QPen(c));
      pi->setBrush(QBrush(c));
    } // end of StandardGraphTheme::drawCurveWithTriangles

    void
    StandardGraphTheme::drawCurveWithDiamonds(Graph& g,
					      QGraphicsItem& d,
					      Curve *const curve,
					      const QVector<Point>& pts,
					      const QColor& c,
					      const qreal ax,
					      const qreal bx,
					      const qreal ay,
					      const qreal by) const
    {
      const qreal w = curve->getWidth();
      const qreal extent = 3u*w;
      if(pts.empty()){
      	return;
      }
      QPainterPath path;
      auto p=pts.begin();
      while(p!=pts.end()){
      	if(!(isInvalidValidQReal(p->x)||isInvalidValidQReal(p->y))){
	  const qreal x0 = (p->x-bx)/ax;
	  const qreal x1 = ay*(p->y)+by;
	  path.moveTo(x0,x1-extent);
	  path.lineTo(x0-extent,x1);
	  path.lineTo(x0,x1+extent);
	  path.lineTo(x0+extent,x1);
	  path.lineTo(x0,x1-extent);
	}
      	++p;
      }
      auto* pi = new GraphCurveItem(&g,&d,curve);
      pi->setPath(path);
      pi->setPen(QPen(c));
      pi->setBrush(QBrush(c));
    } // end of StandardGraphTheme::drawCurveWithDiamonds

    void
    StandardGraphTheme::drawCurveWithCrosses(Graph& g,
					     QGraphicsItem& d,
					     Curve *const curve,
					     const QVector<Point>& pts,
					     const QColor& c,
					     const qreal ax,
					     const qreal bx,
					     const qreal ay,
					     const qreal by) const
    {
      const qreal w = curve->getWidth();
      const qreal extent = 3u*w;
      if(pts.empty()){
      	return;
      }
      QPainterPath path;
      auto p=pts.begin();
      while(p!=pts.end()){
      	if(!(isInvalidValidQReal(p->x)||isInvalidValidQReal(p->y))){
	  const qreal x0 = (p->x-bx)/ax;
	  const qreal x1 = ay*(p->y)+by;
	  path.moveTo(x0-extent,x1-extent);
	  path.lineTo(x0+extent,x1+extent);
	  path.moveTo(x0-extent,x1+extent);
	  path.lineTo(x0+extent,x1-extent);
	}
	++p;
      }
      QPen pen(c);
      pen.setWidth(w);
      auto* pi = new GraphCurveItem(&g,&d,curve);
      pi->setPath(path);
      pi->setPen(pen);
      pi->setBrush(QBrush(c));
    } // end of StandardGraphTheme::drawCurveWithCrosses

    void
    StandardGraphTheme::drawCurveWithDots(Graph& g,
					  QGraphicsItem& d,
					  Curve *const curve,
					  const QVector<Point>& pts,
					  const QColor& c,
					  const qreal ax,
					  const qreal bx,
					  const qreal ay,
					  const qreal by) const
    {
      const qreal w = curve->getWidth();
      const qreal extent = 5u*w;
      QPainterPath path;
      if(pts.empty()){
      	return;
      }
      auto p=pts.begin();
      while(p!=pts.end()){
      	if(!(isInvalidValidQReal(p->x)||(isInvalidValidQReal(p->y)))){
	  const qreal x0 = (p->x-bx)/ax;
	  const qreal x1 = ay*(p->y)+by;
	  path.addEllipse(x0-0.5*extent,x1-0.5*extent,
			  extent,extent);
	}
      	++p;
      }
      QPen pen(c);
      pen.setWidth(w);
      auto* pi = new GraphCurveItem(&g,&d,curve);
      pi->setPath(path);
      pi->setPen(QPen(c));
      pi->setBrush(QBrush(c));
    } // end of StandardGraphTheme::drawCurveWithDots

    void
    StandardGraphTheme::drawCurveWithPlus(Graph& g,
					  QGraphicsItem& d,
					  Curve *const curve,
					  const QVector<Point>& pts,
					  const QColor& c,
					  const qreal ax,
					  const qreal bx,
					  const qreal ay,
					  const qreal by) const
    {
      const qreal w = curve->getWidth();
      const qreal extent = 3u*w;
      QPainterPath path;
      if(pts.empty()){
      	return;
      }
      auto p=pts.begin();
      while(p!=pts.end()){
      	if(!(isInvalidValidQReal(p->x)||isInvalidValidQReal(p->y))){
	  const qreal x0 = (p->x-bx)/ax;
	  const qreal x1 = ay*(p->y)+by;
	  path.moveTo(x0-extent,x1);
	  path.lineTo(x0+extent,x1);
	  path.moveTo(x0,x1+extent);
	  path.lineTo(x0,x1-extent);
	}
	++p;
      }
      QPen pen(c);
      pen.setWidth(w);
      auto* pi = new GraphCurveItem(&g,&d,curve);
      pi->setPath(path);
      pi->setPen(QPen(c));
    } // end of StandardGraphTheme::drawCurveWithPlus


    Curve::Style
    StandardGraphTheme::getCurveStyleFromThemeStyle(const unsigned short t) const
    {
      return Curve::intToCurveStyle(t%3);
    } // end of StandardGraphTheme::getCurveStyleFromThemeStyle

    QColor
    StandardGraphTheme::getCurveColorFromThemeStyle(const unsigned short t) const
    {
      const unsigned short index = t%10;
      return QColor(StandardGraphTheme::defaultRedColors[index],
		    StandardGraphTheme::defaultGreenColors[index],
		    StandardGraphTheme::defaultBlueColors[index]);
    } // end of StandardGraphTheme::getCurveColorFromThemeStyle

    void
    StandardGraphTheme::drawCurve(Graph& g,
				  QGraphicsItem& d,
				  Curve *const c,
				  const QVector<Point>& pts,
				  const GraphSize&,
				  const qreal ax,
				  const qreal bx,
				  const qreal ay,
				  const qreal by,
				  const unsigned short nbr) const
    {
      Curve::Style style;
      QColor color;
      if(c->shouldFollowThemeLineStyle()){
	auto t = c->getThemeLineStyle();
	style = this->getCurveStyleFromThemeStyle(t);
	if(c->hasSpecifiedColor()){
	  color = c->getColor();
	} else {
	  color = this->getCurveColorFromThemeStyle(t);	
	}
      } else {
	style = c->getStyle();
	if(c->hasSpecifiedColor()){
	  color = c->getColor();
	} else {
	  this->getDefaultColor(color,nbr);	
	}
      }
      if(style==Curve::SOLIDLINE){
	this->drawCurveWithLines(g,d,c,pts,color,ax,bx,ay,by,
				 Qt::SolidLine);
      } else if(style==Curve::DOTLINE){
	this->drawCurveWithLines(g,d,c,pts,color,ax,bx,ay,by,
				 Qt::DotLine);
      } else if(style==Curve::DASHLINE){
	this->drawCurveWithLines(g,d,c,pts,color,ax,bx,ay,by,
				 Qt::DashLine);
      } else if(style==Curve::DASHDOTLINE){
	this->drawCurveWithLines(g,d,c,pts,color,ax,bx,ay,by,
				 Qt::DashDotLine);
      } else if(style==Curve::DASHDOTDOTLINE){
	this->drawCurveWithLines(g,d,c,pts,color,ax,bx,ay,by,
				 Qt::DashDotDotLine);
      } else if(style==Curve::DOT){
	this->drawCurveWithDots(g,d,c,pts,color,ax,bx,ay,by);
      } else if(style==Curve::LINEPLUS){
	this->drawCurveWithLines(g,d,c,pts,color,ax,bx,ay,by,
				 Qt::SolidLine);
	this->drawCurveWithPlus(g,d,c,pts,color,ax,bx,ay,by);
      } else if(style==Curve::PLUS){
	this->drawCurveWithPlus(g,d,c,pts,color,ax,bx,ay,by);
      } else if(style==Curve::DIAMOND){
	this->drawCurveWithDiamonds(g,d,c,pts,color,ax,bx,ay,by);
      } else if(style==Curve::SQUARE){
	this->drawCurveWithSquares(g,d,c,pts,color,ax,bx,ay,by);
      } else if(style==Curve::TRIANGLE){
	this->drawCurveWithTriangles(g,d,c,pts,color,ax,bx,ay,by);
      } else {
	this->drawCurveWithCrosses(g,d,c,pts,color,ax,bx,ay,by);
      }
    } // end of StandardGraphTheme::drawCurves

    void
    StandardGraphTheme::getDefaultColor(QColor & color,
					const unsigned short nbr) const
    {
      const unsigned short index = static_cast<unsigned short>((nbr == 0) ? 0 :
							       (nbr-1)%(StandardGraphTheme::defaultColorsNumber));
      color = QColor(StandardGraphTheme::defaultRedColors[index],
		     StandardGraphTheme::defaultGreenColors[index],
		     StandardGraphTheme::defaultBlueColors[index]);
    } // end of StandardGraphTheme::getDefaultColor

    void
    StandardGraphTheme::printKeys(Graph& g,
				  QVector<Graph::CurveHandler>& curves,
				  const GraphLayout& l,
				  const GraphSize& s,
				  const Graph::KeyHorizontalPosition& hp,
				  const Graph::KeyVerticalPosition&   vp,
				  const Graph::KeyAlignment& a) const
    {
      auto& cr = g.getScene();
      auto* gr = new QGraphicsItemGroup();
      gr->setHandlesChildEvents(false);
      QVector<Graph::CurveHandler>::iterator p;
      unsigned short nbr = 1u;
      gr->setFlag(QGraphicsItem::ItemIsMovable);
      gr->setFlag(QGraphicsItem::ItemAcceptsInputMethod);
      qreal w = 0;
      qreal h = 0;
      qreal wa = 0;
      if((a==Graph::ALIGNLEFT)||(a==Graph::ALIGNCENTER)){
	for(p=curves.begin();p!=curves.end();++p){
	  if(p->curve->hasKey()){
	    GraphKeyItem t(p->curve->getKey(),p->curve.get(),&g);
	    auto b = t.boundingRect();
	    if(wa<=b.width()){
	      wa=b.width();
	    }
	  }
	}
      }
      for(p=curves.begin();p!=curves.end();++nbr){
	Curve::Style style;
	qreal h1;
	if(p->curve->shouldFollowThemeLineStyle()){
	  auto t = p->curve->getThemeLineStyle();
	  style = this->getCurveStyleFromThemeStyle(t);
	} else {
	  style = p->curve->getStyle();
	}
	if(p->curve->hasKey()){
	  QString key = p->curve->getKey();
	  if(!key.isEmpty()){
	    if(p->curve->hasSpecifiedColor()){
	      auto size = this->printKey(g,*gr,p->curve.get(),
					 l,s,key,
					 style,p->curve->getColor(),
					 a,wa,h);
	      if(w<=size.width()){
		w = size.width();
	      }
	      h1 = size.height();
	    } else {
	      auto size = this->printKey(g,*gr,p->curve.get(),l,s,key,
					 style,a,wa,h,nbr);
	      if(w<=size.width()){
		w = size.width();
	      }
	      h1 = size.height();
	    }
	    if(++p!=curves.end()){
	      h += h1;
	    }
	  } else {
	    ++p;
	  }
	} else {
	  ++p;
	}
      }
      qreal xk = 0.;
      qreal yk = 0.;
      if(hp==Graph::RIGHT){
	xk = s.xh1-20;
      } else if(hp==Graph::LEFT){
	xk = s.xh0+w+20;
      } else if(hp==Graph::HCENTER){
	xk = 0.5*(s.xh0+s.xh1+w);
      }
      if(vp==Graph::TOP){
	yk = s.yh0+20;
      } else if(vp==Graph::BOTTOM){
	yk = s.yh1-20-h;
      } else if(vp==Graph::VCENTER){
	yk = 0.5*(s.yh0+s.yh1-h);
      }
      gr->setPos(xk,yk);
      // auto *grb = new QGraphicsRectItem(gr->boundingRect());
      // grb->setPen(QPen(Qt::black));
      // grb->setBrush(QBrush(Qt::black));
      // grb->setPos(xk,yk);
      // cr.addItem(grb);
      cr.addItem(gr);
    } // end of StandardGraphTheme::printKeys
    
    QSize StandardGraphTheme::printKey(Graph& g,
				       QGraphicsItemGroup& gr,
				       Curve *const c,
				       const GraphLayout& l,
				       const GraphSize& s,
				       const QString& legend,
				       const Curve::Style style,
				       const Graph::KeyAlignment& a,
				       const qreal wa,
				       const qreal h,
				       const unsigned short nbr) const
    {
      QColor color;
      if(c->shouldFollowThemeLineStyle()){
	auto t = c->getThemeLineStyle();
	if(c->hasSpecifiedColor()){
	  color = c->getColor();
	} else {
	  color = this->getCurveColorFromThemeStyle(t);	
	}
      } else {
	if(c->hasSpecifiedColor()){
	  color = c->getColor();
	} else {
	  this->getDefaultColor(color,nbr);	
	}
      }
      return this->printKey(g,gr,c,l,s,legend,style,color,a,wa,h);
    } // end of StandardGraphTheme::printKey

    QSize StandardGraphTheme::printKey(Graph& g,
				       QGraphicsItemGroup& gr,
				       Curve * const c,
				       const GraphLayout&,
				       const GraphSize&,
				       const QString& l,
				       const Curve::Style style,
				       const QColor& color,
				       const Graph::KeyAlignment& a,
				       const qreal wa,
				       const qreal yh) const
    {
      auto *t = new GraphKeyItem(l,c,&g,&gr);
      auto  b = t->boundingRect();
      if(a==Graph::ALIGNLEFT){
	t->setPos(-60.-wa,yh-0.5*b.height());
      } else if (a==Graph::ALIGNCENTER){
	t->setPos(-60.-0.5*wa-0.5*b.width(),yh-0.5*b.height());
      } else if (a==Graph::ALIGNRIGHT){
	t->setPos(-60.-b.width(),yh-0.5*b.height());
      }
      this->drawCurveKey(gr,color,style,0,yh,
			 std::min(qreal(c->getWidth()),0.9*b.height()));
      return QSize(b.width()+36,b.height()+2);
    } // end of StandardGraphTheme::printKey

    qreal StandardGraphTheme::getXLabelHeight(Graph&,
					      const QString& l) const
    {
      QGraphicsTextItem text(l);
      return text.boundingRect().height()+4;
    } // end of StandardGraphTheme::getXLabelHeight;

    qreal StandardGraphTheme::getX2LabelHeight(Graph&,
					       const QString& l) const
    {
      QGraphicsTextItem text(l);
      return text.boundingRect().height()+4;
    } // end of StandardGraphTheme::getX2LabelHeight;

    qreal StandardGraphTheme::getYLabelWidth(Graph&,
					     const QString& l) const
    {
      QGraphicsTextItem text(l);
      return text.boundingRect().height()+4;
    } // end of StandardGraphTheme::getYLabelWidth;

    qreal StandardGraphTheme::getY2LabelWidth(Graph&,
					      const QString& l) const
    {
      QGraphicsTextItem text(l);
      return text.boundingRect().height()+4;
    } // end of StandardGraphTheme::getY2LabelWidth;

    qreal StandardGraphTheme::getDownTitleHeight(Graph&,
						 const QString& l) const
    {
      if(l.size()==0){
      	return 0;
      }
      QGraphicsTextItem text;
      text.setHtml("<H2>"+l+"</H2>");
      return text.boundingRect().height()+4;
    } // end of StandardGraphTheme::getDownTitleHeight;

    qreal StandardGraphTheme::getUpperTitleHeight(Graph&,
						  const QString& l) const
    {
      if(l.size()==0){
      	return 0;
      }
      QGraphicsTextItem text;
      text.setHtml("<H2>"+l+"</H2>");
      return text.boundingRect().height()+4;
    } // end of StandardGraphTheme::getUpperTitleHeight;

    qreal StandardGraphTheme::getLeftTitleWidth(Graph&,
						const QString& l) const
    {
      if(l.size()==0){
      	return 0;
      }
      QGraphicsTextItem text;
      text.setHtml("<H2>"+l+"</H2>");
      return text.boundingRect().height()+4;
    } // end of StandardGraphTheme::getLeftTitleWidth;

    qreal StandardGraphTheme::getRightTitleWidth(Graph&,
						 const QString& l) const
    {
      if(l.size()==0){
      	return 0;
      }
      QGraphicsTextItem text;
      text.setHtml("<H2>"+l+"</H2>");
      return text.boundingRect().height()+4;
    } // end of StandardGraphTheme::getRightTitleWidth;

    void
    StandardGraphTheme::drawCurveKey(QGraphicsItemGroup& gr,
				     const QColor& c,
				     const Curve::Style style,
				     const qreal xh,
				     const qreal yh,
				     const qreal w) const
    {
      const qreal radius = 2u;
      const qreal we     = std::max(1.2*radius,w);
      const qreal extent = std::max(qreal(3),w/2);
      QPen pen(c);
      if((style==Curve::SOLIDLINE)||(style==Curve::DOTLINE)||
	 (style==Curve::DASHLINE) ||(style==Curve::DASHDOTLINE)||
	 (style==Curve::DASHDOTDOTLINE)||
	 (style==Curve::LINEPLUS)){
	if(style==Curve::DOTLINE){
	  pen.setStyle(Qt::DotLine);
	} else if(style==Curve::DASHLINE){
	  pen.setStyle(Qt::DashLine);
	} else if(style==Curve::DASHDOTLINE){
	  pen.setStyle(Qt::DashDotLine);
	} else if(style==Curve::DASHDOTDOTLINE){
	  pen.setStyle(Qt::DashDotDotLine);
	}
	pen.setWidth(we);
	QPainterPath path;
      	path.moveTo(xh,yh);
      	path.lineTo(xh-40,yh);
	auto pi = new QGraphicsPathItem(&gr);
	pi->setPath(path);
	pi->setPen(pen);
      } else if(style==Curve::DOT){
	const QBrush brush(c);
	auto pi = new QGraphicsEllipseItem(&gr);
	pen.setWidth(we/2);
	pi->setRect(xh-20-radius,yh-radius,2*radius,2*radius);
	pi->setPen(pen);
	pi->setBrush(brush);
      } else if((style==Curve::PLUS)||(style==Curve::LINEPLUS)){
      	const qreal x0 = xh-20;
      	const qreal x1 = yh;
	QPainterPath path;
      	path.moveTo(x0-extent,x1);
      	path.lineTo(x0+extent,x1);
      	path.moveTo(x0,x1+extent);
      	path.lineTo(x0,x1-extent);
	auto pi = new QGraphicsPathItem(&gr);
	pi->setPath(path);
	pi->setPen(pen);
      } else if(style==Curve::DIAMOND){
      	const qreal x0 = xh-20;
      	const qreal x1 = yh;
	QPainterPath path;
	const QBrush brush(c);
      	path.moveTo(x0,x1-extent);
      	path.lineTo(x0-extent,x1);
      	path.lineTo(x0,x1+extent);
      	path.lineTo(x0+extent,x1);
      	path.lineTo(x0,x1-extent);
	auto pi = new QGraphicsPathItem(&gr);
	pi->setPath(path);
	pi->setPen(pen);
	pi->setBrush(brush);
      } else if(style==Curve::SQUARE){
      	const qreal x0 = xh-20;
      	const qreal x1 = yh;
	const QBrush brush(c);
	auto pi = new QGraphicsRectItem(&gr);
	pi->setPen(pen);
	pi->setBrush(brush);
	pi->setRect(x0-extent,x1-extent,2*extent,2*extent);
      } else if(style==Curve::TRIANGLE){
      	const qreal x0 = xh-20;
      	const qreal x1 = yh;
	QPainterPath path;
	const QBrush brush(c);
      	path.moveTo(x0,x1-extent);
      	path.lineTo(x0-extent,x1+extent);
      	path.lineTo(x0+extent,x1+extent);
      	path.lineTo(x0,x1-extent);
	auto pi = new QGraphicsPathItem(&gr);
	pi->setPath(path);
	pi->setPen(pen);
	pi->setBrush(brush);
      } else {
      	const qreal x0 = xh-20;
      	const qreal x1 = yh;
	QPainterPath path;
      	path.moveTo(x0-extent,x1-extent);
      	path.lineTo(x0+extent,x1+extent);
      	path.moveTo(x0-extent,x1+extent);
      	path.lineTo(x0+extent,x1-extent);
	auto pi = new QGraphicsPathItem(&gr);
	pi->setPath(path);
	pi->setPen(pen);
      }
    } // end of StandardGraphTheme::drawCurveKey

    void
    StandardGraphTheme::printLabel(Graph&,
				   const qreal,
				   const qreal,
				   const QString&) const
    {
      // cr->save();
      // cr->move_to(x0,y0);
      // cr->show_text(label);
      // cr->restore();      
    } // end of StandardGraphTheme::printLabel

    void StandardGraphTheme::drawArrow(Graph& g,
				       const qreal x0,
				       const qreal y0,
				       const qreal x1,
				       const qreal y1,
				       const ArrowStyle) const
    {
      const qreal radius = 2u;
      QPen pen;
      pen.setColor(Qt::red);
      pen.setWidth(1.2*radius);
      auto& cr = g.getScene();
      cr.addLine(x0,y0,x1,y1,pen);
    } // end of StandardGraphTheme::drawArrow

    void StandardGraphTheme::drawRubberBand(Graph& g,
					    const QRectF& r) const
    {
      auto& cr = g.getScene();
      QPen   p;
      p.setColor(Qt::black);
      p.setStyle(Qt::DashLine);
      QBrush b;
      b.setColor(QColor(0,0,255, 127));
      b.setStyle(Qt::SolidPattern);
      cr.addRect(r,p,b);
    }

    StandardGraphTheme::~StandardGraphTheme() = default;
    
  } // end of namespace plot

} // end of namespace tfel


