/*!
 * \file   StandardGraphTheme.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   13 jan 2008
 */

#ifndef LIB_TFEL_PLOT_STANDARDGRAPHTHEME_HXX_
#define LIB_TFEL_PLOT_STANDARDGRAPHTHEME_HXX_

#include <QtWidgets/QGraphicsView>
#include "TFEL/Plot/Config.hxx"
#include "TFEL/Plot/Curve.hxx"
#include "TFEL/Plot/GraphLayout.hxx"
#include "TFEL/Plot/GraphTheme.hxx"

namespace tfel {

  namespace plot {

    struct TFELPLOT_VISIBILITY_EXPORT StandardGraphTheme
        : public GraphTheme {
      virtual Curve::Style getCurveStyleFromThemeStyle(
          const unsigned short) const;

      virtual QColor getCurveColorFromThemeStyle(
          const unsigned short) const;

      void getDefaultColor(QColor &,
                           const unsigned short) const override;

      qreal getYTicsWidth(
          Graph &, const std::map<qreal, QString> &) const override;

      qreal getY2TicsWidth(
          Graph &, const std::map<qreal, QString> &) const override;

      qreal getXTicsHeight(
          Graph &, const std::map<qreal, QString> &) const override;

      qreal getX2TicsHeight(
          Graph &, const std::map<qreal, QString> &) const override;

      qreal getLeftMargin() const override;

      qreal getRightMargin() const override;

      qreal getUpperMargin() const override;

      qreal getDownMargin() const override;

      void printUpperTitle(Graph &,
                           const GraphLayout &,
                           const GraphSize &,
                           const QString &) const override;

      void printDownTitle(Graph &,
                          const GraphLayout &,
                          const GraphSize &,
                          const QString &) const override;

      void printRightTitle(Graph &,
                           const GraphLayout &,
                           const GraphSize &,
                           const QString &) const override;

      void printLeftTitle(Graph &,
                          const GraphLayout &,
                          const GraphSize &,
                          const QString &) const override;

      void printXLabel(Graph &,
                       const GraphLayout &,
                       const GraphSize &,
                       const QString &) const override;

      void printYLabel(Graph &,
                       const GraphLayout &,
                       const GraphSize &,
                       const QString &) const override;

      void printX2Label(Graph &,
                        const GraphLayout &,
                        const GraphSize &,
                        const QString &) const override;

      void printY2Label(Graph &,
                        const GraphLayout &,
                        const GraphSize &,
                        const QString &) const override;

      void printXTics(Graph &,
                      const GraphLayout &,
                      const GraphSize &,
                      const std::map<qreal, QString> &) const override;

      void printYTics(Graph &,
                      const GraphLayout &,
                      const GraphSize &,
                      const std::map<qreal, QString> &) const override;

      void printX2Tics(Graph &,
                       const GraphLayout &,
                       const GraphSize &,
                       const std::map<qreal, QString> &) const override;

      void printY2Tics(Graph &,
                       const GraphLayout &,
                       const GraphSize &,
                       const std::map<qreal, QString> &) const override;

      void getGraphBorders(
          Graph &, qreal &, qreal &, qreal &, qreal &) const override;

      void drawXAxis(QGraphicsItem &,
                     const GraphLayout &,
                     const GraphSize &) const override;

      void drawGrid(Graph &,
                    const GraphLayout &,
                    const GraphSize &,
                    const bool,
                    const unsigned short,
                    const std::map<qreal, QString> &,
                    const std::map<qreal, QString> &,
                    const std::map<qreal, QString> &,
                    const std::map<qreal, QString> &,
                    const qreal,
                    const qreal,
                    const qreal,
                    const qreal,
                    const qreal,
                    const qreal,
                    const qreal,
                    const qreal,
                    const bool,
                    const bool,
                    const bool,
                    const bool) const override;

      void drawYAxis(QGraphicsItem &,
                     const GraphLayout &,
                     const GraphSize &) const override;

      void drawGraphBorder(Graph &,
                           const GraphLayout &,
                           const GraphSize &) const override;

      void drawForeGround(Graph &,
                          const GraphLayout &,
                          const GraphSize &) const override;

      void drawBackGround(Graph &,
                          const GraphLayout &,
                          const GraphSize &) const override;

      void drawGraphForeGround(Graph &,
                               const GraphLayout &,
                               const GraphSize &) const override;

      void drawGraphBackGround(Graph &,
                               const GraphLayout &,
                               const GraphSize &) const override;

      void drawCurve(Graph &,
                     QGraphicsItem &,
                     Curve *const,
                     const std::vector<Point> &,
                     const GraphSize &,
                     const qreal,
                     const qreal,
                     const qreal,
                     const qreal,
                     const unsigned short) const override;

      void printLabel(Graph &,
                      const qreal,
                      const qreal,
                      const QString &) const override;

      void drawArrow(Graph &,
                     const qreal,
                     const qreal,
                     const qreal,
                     const qreal,
                     const ArrowStyle) const override;

      qreal getXLabelHeight(Graph &, const QString &) const override;

      qreal getX2LabelHeight(Graph &, const QString &) const override;

      qreal getYLabelWidth(Graph &, const QString &) const override;

      qreal getY2LabelWidth(Graph &, const QString &) const override;

      qreal getDownTitleHeight(Graph &, const QString &) const override;

      qreal getUpperTitleHeight(Graph &,
                                const QString &) const override;

      qreal getLeftTitleWidth(Graph &, const QString &) const override;

      qreal getRightTitleWidth(Graph &, const QString &) const override;

      void printKeys(Graph &,
                     std::vector<Graph::CurveHandler> &,
                     const GraphLayout &,
                     const GraphSize &,
                     const Graph::KeyHorizontalPosition &,
                     const Graph::KeyVerticalPosition &,
                     const Graph::KeyAlignment &) const override;

      ~StandardGraphTheme() override;

     protected:
      virtual void fillQPainterPath(QPainterPath &,
                                    const Curve &,
                                    const std::vector<Point> &,
                                    const qreal,
                                    const qreal,
                                    const qreal,
                                    const qreal) const;

      virtual QSize printKey(Graph &,
                             QGraphicsItemGroup &,
                             Curve *const,
                             const GraphLayout &,
                             const GraphSize &,
                             const QString &,
                             const Curve::Style,
                             const Graph::KeyAlignment &,
                             const qreal,
                             const qreal,
                             const unsigned short) const;

      virtual QSize printKey(Graph &,
                             QGraphicsItemGroup &,
                             Curve *const,
                             const GraphLayout &,
                             const GraphSize &,
                             const QString &,
                             const Curve::Style,
                             const QColor &,
                             const Graph::KeyAlignment &,
                             const qreal,
                             const qreal) const;

      virtual void drawHorizontalGridMark(
          Graph &,
          const QPen &,
          const GraphLayout &,
          const GraphSize &,
          const std::map<qreal, QString> &,
          const qreal,
          const qreal) const;

      virtual void drawVerticalGridMark(
          Graph &,
          const QPen &,
          const GraphLayout &,
          const GraphSize &,
          const std::map<qreal, QString> &,
          const qreal,
          const qreal) const;

      virtual void drawHorizontalGrid(Graph &,
                                      const QPen &,
                                      const GraphLayout &,
                                      const GraphSize &,
                                      const std::map<qreal, QString> &,
                                      const qreal,
                                      const qreal) const;

      virtual void drawVerticalGrid(Graph &,
                                    const QPen &,
                                    const GraphLayout &,
                                    const GraphSize &,
                                    const std::map<qreal, QString> &,
                                    const qreal,
                                    const qreal) const;

      virtual void drawCurveWithLines(Graph &,
                                      QGraphicsItem &,
                                      Curve *const,
                                      const std::vector<Point> &,
                                      const QColor &,
                                      const qreal,
                                      const qreal,
                                      const qreal,
                                      const qreal,
                                      const Qt::PenStyle) const;

      virtual void drawCurveWithDots(Graph &,
                                     QGraphicsItem &,
                                     Curve *const,
                                     const std::vector<Point> &,
                                     const QColor &,
                                     const qreal,
                                     const qreal,
                                     const qreal,
                                     const qreal) const;

      virtual void drawCurveWithSquares(Graph &,
                                        QGraphicsItem &,
                                        Curve *const,
                                        const std::vector<Point> &,
                                        const QColor &,
                                        const qreal,
                                        const qreal,
                                        const qreal,
                                        const qreal) const;

      virtual void drawCurveWithTriangles(Graph &,
                                          QGraphicsItem &,
                                          Curve *const,
                                          const std::vector<Point> &,
                                          const QColor &,
                                          const qreal,
                                          const qreal,
                                          const qreal,
                                          const qreal) const;

      virtual void drawCurveWithCrosses(Graph &,
                                        QGraphicsItem &,
                                        Curve *const,
                                        const std::vector<Point> &,
                                        const QColor &,
                                        const qreal,
                                        const qreal,
                                        const qreal,
                                        const qreal) const;

      virtual void drawCurveWithPlus(Graph &,
                                     QGraphicsItem &,
                                     Curve *const,
                                     const std::vector<Point> &,
                                     const QColor &,
                                     const qreal,
                                     const qreal,
                                     const qreal,
                                     const qreal) const;

      virtual void drawCurveWithDiamonds(Graph &,
                                         QGraphicsItem &,
                                         Curve *const,
                                         const std::vector<Point> &,
                                         const QColor &,
                                         const qreal,
                                         const qreal,
                                         const qreal,
                                         const qreal) const;

      void drawRubberBand(Graph &, const QRectF &) const override;

     protected:
      virtual void drawCurveKey(QGraphicsItemGroup &,
                                const QColor &,
                                const Curve::Style,
                                const qreal,
                                const qreal,
                                const qreal) const;

     private:
      static const qreal defaultRedColors[10];
      static const qreal defaultGreenColors[10];
      static const qreal defaultBlueColors[10];
      static const unsigned short defaultColorsNumber;

    };  // end of struct StandardGraphTheme

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_STANDARDGRAPHTHEME_HXX */
