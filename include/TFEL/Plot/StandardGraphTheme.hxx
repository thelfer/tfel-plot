/*!
 * \file   StandardGraphTheme.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#ifndef LIB_TFEL_PLOT_STANDARDGRAPHTHEME_HXX_
#define LIB_TFEL_PLOT_STANDARDGRAPHTHEME_HXX_ 

#ifdef TFEL_QT4
#include <QtGui/QGraphicsView>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include <QtWidgets/QGraphicsView>
#endif /* TFEL_QT5 */

#include"TFEL/Plot/Config.hxx"
#include"TFEL/Plot/Curve.hxx"
#include"TFEL/Plot/GraphLayout.hxx"
#include"TFEL/Plot/GraphTheme.hxx"

namespace tfel{

  namespace plot
  {

    struct TFELPLOT_VISIBILITY_EXPORT StandardGraphTheme
      : public GraphTheme
    {

      virtual void
      getDefaultColor(QColor&,const unsigned short) const override;
      
      virtual Curve::Style
      getCurveStyleFromThemeStyle(const unsigned short) const;

      virtual QColor
      getCurveColorFromThemeStyle(const unsigned short) const;
      
      virtual qreal
      getYTicsWidth(Graph&,
		    const QMap<qreal,QString>&) const override;

      virtual qreal
      getY2TicsWidth(Graph&,
		     const QMap<qreal,QString>&) const override;

      virtual qreal
      getXTicsHeight(Graph&,
		     const QMap<qreal,QString>&) const override;

      virtual qreal
      getX2TicsHeight(Graph&,
		      const QMap<qreal,QString>&) const override;

      virtual qreal
      getLeftMargin(void) const override;

      virtual qreal
	getRightMargin(void) const override;

      virtual qreal
	getUpperMargin(void) const override;

      virtual qreal
	getDownMargin(void) const override;

      virtual void
	printUpperTitle(Graph&,
			const GraphLayout&,
			const GraphSize&,
			const QString&) const override;

      virtual void
	printDownTitle(Graph&,
		       const GraphLayout&,
		       const GraphSize&,
		       const QString&) const override;

      virtual void
	printRightTitle(Graph&,
			const GraphLayout&,
			const GraphSize&,
			const QString&) const override;

      virtual void
	printLeftTitle(Graph&,
		       const GraphLayout&,
		       const GraphSize&,
		       const QString&) const override;

      virtual void
	printXLabel(Graph&,
		    const GraphLayout&,
		    const GraphSize&,
		    const QString&) const override;

      virtual void
      printYLabel(Graph&,
		  const GraphLayout&,
		  const GraphSize&,
		  const QString&) const override;

      virtual void
	printX2Label(Graph&,
		     const GraphLayout&,
		     const GraphSize&,
		     const QString&) const override;

      virtual void
	printY2Label(Graph&,
		     const GraphLayout&,
		     const GraphSize&,
		     const QString&) const override;

      virtual void
	printXTics(Graph&,
		   const GraphLayout&,
		   const GraphSize&,
		   const QMap<qreal,QString>&) const override;

      virtual void
	printYTics(Graph&,
		   const GraphLayout&,
		   const GraphSize&,
		   const QMap<qreal,QString>&) const override;

      virtual void
	printX2Tics(Graph&,
		    const GraphLayout&,
		    const GraphSize&,
		    const QMap<qreal,QString>&) const override;

      virtual void
	printY2Tics(Graph&,
		    const GraphLayout&,
		    const GraphSize&,
		    const QMap<qreal,QString>&) const override;

      virtual void
	getGraphBorders(Graph&,
			qreal&,qreal&,
			qreal&,qreal&) const override;

      virtual void
	drawXAxis(QGraphicsItem&,
		  const GraphLayout&,
		  const GraphSize&) const override;

      virtual void
      drawGrid(Graph&,
	       const GraphLayout&,
	       const GraphSize&,
	       const bool,
	       const unsigned short,
	       const QMap<qreal,QString>&,
	       const QMap<qreal,QString>&,
	       const QMap<qreal,QString>&,
	       const QMap<qreal,QString>&,
	       const qreal,const qreal,
	       const qreal,const qreal,
	       const qreal,const qreal,
	       const qreal,const qreal,
	       const bool, const bool,
	       const bool,const bool) const override;

      virtual void
	drawYAxis(QGraphicsItem&,
		  const GraphLayout&,
		  const GraphSize&) const override;

      virtual void
	drawGraphBorder(Graph&,
			const GraphLayout&,
			const GraphSize&) const override;

      virtual void
	drawForeGround(Graph&,
		       const GraphLayout&,
		       const GraphSize&) const override;

      
      virtual void
	drawBackGround(Graph&,
		       const GraphLayout&,
		       const GraphSize&) const override;

      
      virtual void
	drawGraphForeGround(Graph&,
			    const GraphLayout&,
			    const GraphSize&) const override;


      virtual void
      drawGraphBackGround(Graph&,
			  const GraphLayout&,
			  const GraphSize&) const override;
      
      virtual void
	drawCurve(Graph&,
		  QGraphicsItem&,
		  Curve *const,
		  const QVector<Point>&,
		  const GraphSize&,
		  const qreal,
		  const qreal,
		  const qreal,
		  const qreal,
		  const unsigned short) const override;

      virtual void
	printLabel(Graph&,
		   const qreal,
		   const qreal,
		   const QString&) const override;

      virtual void
	drawArrow(Graph&,
		  const qreal,
		  const qreal,
		  const qreal,
		  const qreal,
		  const ArrowStyle) const override;


      virtual qreal
	getXLabelHeight(Graph&,
			const QString&) const override;

      virtual qreal
	getX2LabelHeight(Graph&,
			 const QString&) const override;

      virtual qreal
	getYLabelWidth(Graph&,
		       const QString&) const override;

      virtual qreal
	getY2LabelWidth(Graph&,
			const QString&) const override;

      virtual qreal
	getDownTitleHeight(Graph&,
			   const QString&) const override;

      virtual qreal
	getUpperTitleHeight(Graph&,
			    const QString&) const override;

      virtual qreal
	getLeftTitleWidth(Graph&,
			  const QString&) const override;

      virtual qreal
	getRightTitleWidth(Graph&,
			   const QString&) const override;

      virtual void
      printKeys(Graph&,
		QVector<Graph::CurveHandler>&,
		const GraphLayout&,
		const GraphSize&,
		const Graph::KeyHorizontalPosition&,
		const Graph::KeyVerticalPosition&,
		const Graph::KeyAlignment&) const override;
      
      virtual ~StandardGraphTheme();

    protected :

      virtual void
      fillQPainterPath(QPainterPath&,
		       const Curve&,
		       const QVector<Point>&,
		       const qreal,
		       const qreal,
		       const qreal,
		       const qreal) const;

      virtual QSize
      printKey(Graph&,
	       QGraphicsItemGroup&,
	       Curve *const,
	       const GraphLayout&,
	       const GraphSize&,
	       const QString&,
	       const Curve::Style,
	       const Graph::KeyAlignment&,
	       const qreal,
	       const qreal,
	       const unsigned short) const;
      
      virtual QSize
      printKey(Graph&,
	       QGraphicsItemGroup&,
	       Curve *const,
	       const GraphLayout&,
	       const GraphSize&,
	       const QString&,
	       const Curve::Style,
	       const QColor&,
	       const Graph::KeyAlignment&,
	       const qreal,
	       const qreal) const;

      virtual void
	drawHorizontalGridMark(Graph&,
			       const QPen&,
			       const GraphLayout&,
			       const GraphSize&,
			       const QMap<qreal,QString>&,
			       const qreal,const qreal) const;

      virtual void
	drawVerticalGridMark(Graph&,
			     const QPen&,
			     const GraphLayout&,
			     const GraphSize&,
			     const QMap<qreal,QString>&,
			     const qreal,const qreal) const;

      virtual void
	drawHorizontalGrid(Graph&,
			   const QPen&,
			   const GraphLayout&,
			   const GraphSize&,
			   const QMap<qreal,QString>&,
			   const qreal,const qreal) const;

      virtual void
	drawVerticalGrid(Graph&,
			 const QPen&,
			 const GraphLayout&,
			 const GraphSize&,
			 const QMap<qreal,QString>&,
			 const qreal,const qreal) const;

      virtual void
	drawCurveWithLines(Graph&,
			   QGraphicsItem&,
			   Curve *const,
			   const QVector<Point>&,
			   const QColor&,
			   const qreal,
			   const qreal,
			   const qreal,
			   const qreal,
			   const Qt::PenStyle) const;

      virtual void
	drawCurveWithDots(Graph&,
			  QGraphicsItem&,Curve *const,
			  const QVector<Point>&,
			  const QColor&,
			  const qreal,
			  const qreal,
			  const qreal,
			  const qreal) const;

      virtual void
	drawCurveWithSquares(Graph&,
			     QGraphicsItem&,Curve *const,
			     const QVector<Point>&,
			     const QColor&,
			     const qreal,
			     const qreal,
			     const qreal,
			     const qreal) const;

      virtual void
	drawCurveWithTriangles(Graph&,
			       QGraphicsItem&,  Curve *const,
			       const QVector<Point>&,
			       const QColor&,
			       const qreal,
			       const qreal,
			       const qreal,
			       const qreal) const;

      virtual void
	drawCurveWithCrosses(Graph&,
			     QGraphicsItem&,Curve * const,
			     const QVector<Point>&,
			     const QColor&,
			     const qreal,
			     const qreal,
			     const qreal,
			     const qreal) const;

      virtual void
	drawCurveWithPlus(Graph&,
			  QGraphicsItem&,
			  Curve * const,
			  const QVector<Point>&,
			  const QColor&,
			  const qreal,
			  const qreal,
			  const qreal,
			  const qreal) const;

      virtual void
	drawCurveWithDiamonds(Graph&,
			      QGraphicsItem&,
			      Curve * const,
			      const QVector<Point>&,
			      const QColor&,
			      const qreal,
			      const qreal,
			      const qreal,
			      const qreal) const;

      virtual void drawRubberBand(Graph&,
				  const QRectF&) const override;

    protected:

      virtual void
      drawCurveKey(QGraphicsItemGroup&,
		   const QColor&,
		   const Curve::Style,
		   const qreal,
		   const qreal) const;

    private:

      static const qreal defaultRedColors[10];
      static const qreal defaultGreenColors[10];
      static const qreal defaultBlueColors[10];
      static const unsigned short defaultColorsNumber;

    }; // end of struct StandardGraphTheme

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_STANDARDGRAPHTHEME_HXX */

