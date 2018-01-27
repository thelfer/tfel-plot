/*!
 * \file   GraphTheme.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 jan 2008
 */

#ifndef LIB_TFEL_PLOT_GRAPHTHEME_HXX_
#define LIB_TFEL_PLOT_GRAPHTHEME_HXX_ 

#include<QtCore/QMap>

#include<QtCore/QVector>
#include<QtCore/QString>

#include"TFEL/Plot/Grid.hxx"
#include"TFEL/Plot/Point.hxx"
#include"TFEL/Plot/ArrowStyle.hxx"
#include"TFEL/Plot/Curve.hxx"
#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/GraphSize.hxx"
#include"TFEL/Plot/GraphLayout.hxx"

namespace tfel{

  namespace plot
  {

    /*!
     * abstract class for graph themes
     */
    struct TFELPLOT_VISIBILITY_EXPORT GraphTheme
    {
      virtual void getDefaultColor(QColor&,const unsigned short) const = 0;
      virtual qreal getYTicsWidth(Graph&,
				  const QMap<qreal,QString>&) const = 0;
      virtual qreal getY2TicsWidth(Graph&,
				   const QMap<qreal,QString>&) const = 0;
      virtual qreal getXTicsHeight(Graph&,
				   const QMap<qreal,QString>&) const = 0;
      virtual qreal getX2TicsHeight(Graph&,
				    const QMap<qreal,QString>&) const = 0;
      virtual qreal getLeftMargin() const = 0;
      virtual qreal getRightMargin() const = 0;
      virtual qreal getUpperMargin() const = 0;
      virtual qreal getDownMargin() const = 0;
      virtual void printUpperTitle(Graph&,
				   const GraphLayout&,
				   const GraphSize&,
				   const QString&) const = 0;
      virtual void printDownTitle(Graph&,
				  const GraphLayout&,
				  const GraphSize&,
				  const QString&) const = 0;
      virtual void printRightTitle(Graph&,
				   const GraphLayout&,
				   const GraphSize&,
				   const QString&) const = 0;
      virtual void printLeftTitle(Graph&,
				  const GraphLayout&,
				  const GraphSize&,
				  const QString&) const = 0;
      virtual void printXLabel(Graph&,
			       const GraphLayout&,
			       const GraphSize&,
			       const QString&) const = 0;
      virtual void printYLabel(Graph&,
			       const GraphLayout&,
			       const GraphSize&,
			       const QString&) const = 0;
      virtual void printX2Label(Graph&,
			 	const GraphLayout&,
			 	const GraphSize&,
			 	const QString&) const = 0;
      virtual void printY2Label(Graph&,
				const GraphLayout&,
				const GraphSize&,
				const QString&) const = 0;
      virtual void printXTics(Graph&,
			      const GraphLayout&,
			      const GraphSize&,
			      const QMap<qreal,QString>&) const = 0;
      virtual void printYTics(Graph&,
			      const GraphLayout&,
			      const GraphSize&,
			      const QMap<qreal,QString>&) const = 0;
      virtual void printX2Tics(Graph&,
			       const GraphLayout&,
			       const GraphSize&,
			       const QMap<qreal,QString>&) const = 0;
      virtual void printY2Tics(Graph&,
			       const GraphLayout&,
			       const GraphSize&,
			       const QMap<qreal,QString>&) const = 0;

      virtual void getGraphBorders(Graph&,
				   qreal&,qreal&,
				   qreal&,qreal&) const = 0;
      virtual void drawGrid(Graph&,
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
			    const bool,const bool) const = 0;
      virtual void drawXAxis(QGraphicsItem&,
			     const GraphLayout&,
			     const GraphSize&) const = 0;
      virtual void drawYAxis(QGraphicsItem&,
			     const GraphLayout&,
			     const GraphSize&) const = 0;
      virtual void drawGraphBorder(Graph&,
				   const GraphLayout&,
				   const GraphSize&) const = 0;
      virtual void drawForeGround(Graph&,
				  const GraphLayout&,
				  const GraphSize&) const = 0;
      virtual void drawBackGround(Graph&,
				  const GraphLayout&,
				  const GraphSize&) const = 0;
      virtual void drawGraphForeGround(Graph&,
				       const GraphLayout&,
				       const GraphSize&) const =  0;
      virtual void drawGraphBackGround(Graph&,
				       const GraphLayout&,
				       const GraphSize&) const =  0;
      virtual void printLabel(Graph&,
			      const qreal,
			      const qreal,
			      const QString&) const = 0;
      virtual void drawArrow(Graph&,
			     const qreal,
			     const qreal,
			     const qreal,
			     const qreal,
			     const ArrowStyle) const = 0;
      virtual void drawCurve(Graph&,
			     QGraphicsItem&,
			     Curve *const,
			     const QVector<Point>&,
			     const GraphSize&,
			     const qreal,
			     const qreal,
			     const qreal,
			     const qreal,
			     const unsigned short) const = 0;
      virtual void printKeys(Graph&,
			     QVector<Graph::CurveHandler>&,
			     const GraphLayout&,
			     const GraphSize&,
			     const Graph::KeyHorizontalPosition&,
			     const Graph::KeyVerticalPosition&,
			     const Graph::KeyAlignment&) const = 0;
      virtual qreal getXLabelHeight(Graph&,
				    const QString&) const = 0;
      virtual qreal getX2LabelHeight(Graph&,
				     const QString&) const = 0;
      virtual qreal getYLabelWidth(Graph&,
				   const QString&) const = 0;
      virtual qreal getY2LabelWidth(Graph&,
				    const QString&) const = 0;

      virtual qreal getDownTitleHeight(Graph&,
				       const QString&) const = 0;
      virtual qreal getUpperTitleHeight(Graph&,
					const QString&) const = 0;
      virtual qreal getLeftTitleWidth(Graph&,
				      const QString&) const = 0;
      virtual qreal getRightTitleWidth(Graph&,
				       const QString&) const = 0;

      virtual void drawRubberBand(Graph&,
				  const QRectF&) const = 0;
      //! destructor
      virtual ~GraphTheme();
    }; // end of struct GraphTheme

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPHTHEME_HXX */

