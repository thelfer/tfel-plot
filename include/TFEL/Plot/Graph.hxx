/*!
 * \file   Graph.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   04 jan 2008
 */

#ifndef LIB_TFEL_PLOT_GRAPH_HXX_
#define LIB_TFEL_PLOT_GRAPH_HXX_

#include <vector>
#include <QtCore/QMap>
#include <QtGui/QFont>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsScene>
#include <QtPrintSupport/QPrinter>
#include "TFEL/Plot/Config.hxx"
#include "TFEL/Plot/ArrowStyle.hxx"
#include "TFEL/Plot/Point.hxx"
#include "TFEL/Plot/Curve.hxx"
#include "TFEL/Plot/Grid.hxx"
#include "TFEL/Plot/GraphCoordinates.hxx"

namespace tfel {

  namespace plot {

    struct GraphTheme;
    struct GraphLayout;
    class GraphConfigurationDialog;

    class TFELPLOT_VISIBILITY_EXPORT Graph : public QGraphicsView {
      Q_OBJECT

     public:
      enum GraphBorder {
        X,   // lower border
        X2,  // upper border
        Y,   // left  border
        Y2   // right border
      };     // end of enum GraphBorder

      enum GraphAxis {
        XY = 5,
        X2Y = 6,
        XY2 = 9,
        X2Y2 = 10
      };  // end of enum GraphAxis

      enum KeyHorizontalPosition {
        LEFT,
        RIGHT,
        HCENTER
      };  // end of enum KeyHorizontalPosition

      enum KeyVerticalPosition {
        TOP,
        BOTTOM,
        VCENTER
      };  // end of enum KeyVerticalPosition

      enum KeyAlignment {
        ALIGNLEFT,
        ALIGNRIGHT,
        ALIGNCENTER
      };  // end of enum KeyVerticalPosition

      virtual QImage exportToQImage();

      virtual QByteArray exportToSVG();

      virtual QString exportToTable();

     public slots:

      virtual void print(QPrinter*);

      virtual void replot();

      virtual void setGraphWidth(const qreal);

      virtual void setGraphHeight(const qreal);

      virtual void unsetGraphWidth();

      virtual void unsetGraphHeight();

      virtual void updatedCurve(Curve*);

      virtual void exportToTable(const QString&);

      virtual void insertImage(const QPixmap&);

      virtual void exportToPDF(const QString&);

      virtual void exportToSVG(const QString&);

      virtual void exportToPNG(const QString&);

      virtual void exportToJPEG(const QString&);

      virtual void exportToBMP(const QString&);

      virtual void exportToPPM(const QString&);

      virtual void exportToXBM(const QString&);

      virtual void exportToXPM(const QString&);

      virtual void showGrid(const bool = true);

      virtual void showBorder(const bool = true);

      virtual void hideGrid(const bool = true);

      virtual void hideBorder(const bool = true);

      virtual void toggleGrid(const bool = true);

      virtual void removeCurve(Curve*, const bool = true);

      virtual void removeCurves(const bool = true);

      virtual void setGrid(const unsigned short, const bool = true);

      virtual void setXTics(const QMap<qreal, QString>&,
                            const bool = true);

      virtual void setX2Tics(const QMap<qreal, QString>&,
                             const bool = true);

      virtual void setYTics(const QMap<qreal, QString>&,
                            const bool = true);

      virtual void setY2Tics(const QMap<qreal, QString>&,
                             const bool = true);

      virtual void unsetXTics(const bool = true);

      virtual void unsetX2Tics(const bool = true);

      virtual void unsetYTics(const bool = true);

      virtual void unsetY2Tics(const bool = true);

      virtual void removeArrow(const QString&, const bool = true);

      virtual void removeArrows(const bool = true);

      virtual void setXLabel(const QString&, const bool = true);

      virtual void setYLabel(const QString&, const bool = true);

      virtual void setX2Label(const QString&, const bool = true);

      virtual void setY2Label(const QString&, const bool = true);

      virtual void setUpperTitle(const QString&, const bool = true);

      virtual void setDownTitle(const QString&, const bool = true);

      virtual void setRightTitle(const QString&, const bool = true);

      virtual void setLeftTitle(const QString&, const bool = true);

      virtual void setGraphFont(const QFont&);

      virtual void setGraphFontFamily(const QString&);

      virtual void setGraphFontSize(const qreal);

      virtual QFont getGraphFont() const;

      virtual void setXRange(const qreal,
                             const qreal,
                             const bool = true);

      virtual void setYRange(const qreal,
                             const qreal,
                             const bool = true);

      virtual void setX2Range(const qreal,
                              const qreal,
                              const bool = true);

      virtual void setY2Range(const qreal,
                              const qreal,
                              const bool = true);

      virtual void setXAxisLogScale(const bool = true);

      virtual void unsetXAxisLogScale(const bool = true);

      virtual void setX2AxisLogScale(const bool = true);

      virtual void unsetX2AxisLogScale(const bool = true);

      virtual void setYAxisLogScale(const bool = true);

      virtual void unsetYAxisLogScale(const bool = true);

      virtual void setY2AxisLogScale(const bool = true);

      virtual void unsetY2AxisLogScale(const bool = true);

      virtual void unsetXRange(const bool = true);

      virtual void unsetYRange(const bool = true);

      virtual void unsetX2Range(const bool = true);

      virtual void unsetY2Range(const bool = true);

      virtual void setKeyHorizontalPosition(
          const KeyHorizontalPosition&, const bool = true);

      virtual void setKeyVerticalPosition(const KeyVerticalPosition&,
                                          const bool = true);

      virtual void setKeyPositions(const KeyHorizontalPosition&,
                                   const KeyVerticalPosition&,
                                   const bool = true);

      virtual void setKeyAlignment(const KeyAlignment&,
                                   const bool = true);

      virtual void removeLabel(const QString&, const bool = true);

      virtual void removeLabels(const bool = true);

      virtual void addLabel(const QString&,
                            const QString&,
                            const GraphCoordinates,
                            const GraphCoordinates,
                            const bool = true);

      virtual void addLabel(const QString&,
                            const GraphCoordinates,
                            const GraphCoordinates,
                            const bool = true);

      virtual void addArrow(const GraphCoordinates,
                            const GraphCoordinates,
                            const GraphCoordinates,
                            const GraphCoordinates,
                            const ArrowStyle,
                            const bool = true);

      virtual void addArrow(const QString&,
                            const GraphCoordinates,
                            const GraphCoordinates,
                            const GraphCoordinates,
                            const GraphCoordinates,
                            const ArrowStyle,
                            const bool = true);

      virtual void configureGraphRange(const GraphBorder&);

      virtual void configureGraphRanges();

      virtual void reset();

     signals:

      void curveAdded(Curve*);

      void curveRemoved(Curve*);

      void curvesRemoved();

      void gridChanged();

      void gridSet(const unsigned short);

      void gridChanged(const unsigned short);

      void xTicsChanged();

      void x2TicsChanged();

      void yTicsChanged();

      void y2TicsChanged();

      void xTicsUnSet();

      void x2TicsUnSet();

      void yTicsUnSet();

      void y2TicsUnSet();

      void arrowRemoved(const QString&);

      void arrowsRemoved();

      void xLabelChanged(const QString&);

      void yLabelChanged(const QString&);

      void x2LabelChanged(const QString&);

      void y2LabelChanged(const QString&);

      void upperTitleChanged(const QString&);

      void downTitleChanged(const QString&);

      void rightTitleChanged(const QString&);

      void leftTitleChanged(const QString&);

      void xRangeChanged(const qreal, const qreal);

      void yRangeChanged(const qreal, const qreal);

      void x2RangeChanged(const qreal, const qreal);

      void y2RangeChanged(const qreal, const qreal);

      void xRangeUnSet();

      void yRangeUnSet();

      void x2RangeUnSet();

      void y2RangeUnSet();

      void xAxisLogScaleSet();

      void xAxisLogScaleUnSet();

      void x2AxisLogScaleSet();

      void x2AxisLogScaleUnSet();

      void yAxisLogScaleSet();

      void yAxisLogScaleUnSet();

      void y2AxisLogScaleSet();

      void y2AxisLogScaleUnSet();

      void keyHorizontalPositionChanged(const KeyHorizontalPosition&);

      void keyVerticalPositionChanged(const KeyVerticalPosition&);

      void keyPositionsChanged(const KeyHorizontalPosition&,
                               const KeyVerticalPosition&);

      void keyAlignmentChanged(const KeyAlignment&);

      void labelRemoved(const QString&);

      void labelsRemoved();

      void labelAdded(const QString&, const QString&);

      void labelAdded(const QString&);

      void arrowAdded();

      void arrowAdded(const QString&);

      void updated();

      void fontChanged(const QFont&);

     public:
      static constexpr const unsigned short xaxis = 1u;
      static constexpr const unsigned short x2axis = 2u;
      static constexpr const unsigned short yaxis = 4u;
      static constexpr const unsigned short y2axis = 8u;

      /*!
       * return the qcolor associated to the given name
       */
      static QColor getColorByName(const QString&);

      struct Axis {
        enum AxisScale {
          LINEARSCALE,
          LOGSCALE
        };  // end of enum AxisScale
        explicit Axis(const unsigned short);
        void reset();
        const unsigned short id;
        QMap<qreal, QString> tics;
        QString label;
        qreal min;
        qreal max;
        bool userDefinedRange;
        bool userDefinedTics;
        AxisScale scale;
      };  // end of struct Axis
      struct CurveHandler {
        std::shared_ptr<Curve> curve;
        std::vector<Point> points;
        GraphAxis axis;
      };

      Graph(QWidget* const = nullptr);

      GraphConfigurationDialog& getConfigurationDialog();

      /*!
       * keep aspect ratio
       */
      int heightForWidth(int) const override;

      qreal getGraphWidth() const;

      qreal getGraphHeight() const;

      /*!
       * \return the graph underlying QGraphicsScene
       */
      QGraphicsScene& getScene();
      /*!
       * retrieve curve title
       */
      void getCurvesTitles(std::vector<QString>&) const;
      /*!
       * retrieve curves
       */
      std::vector<std::shared_ptr<Curve>> getCurves() const;

      void getRange(qreal&, qreal&, const unsigned short) const;

      /*!
       * \param[in] t : new theme
       * \param[in] b : replot the graph
       */
      void setTheme(const std::shared_ptr<GraphTheme>&, const bool);

      std::shared_ptr<GraphTheme> getTheme();
      std::shared_ptr<GraphTheme> getTheme() const;
      bool hasGrid() const;
      unsigned short getGrid() const;
      virtual void addCurve(std::shared_ptr<Curve>,
                            const GraphAxis = XY);
      qreal getXMin() const;
      qreal getXMax() const;
      qreal getYMin() const;
      qreal getYMax() const;
      qreal getX2Min() const;
      qreal getX2Max() const;
      qreal getY2Min() const;
      qreal getY2Max() const;

      const QString& getUpperTitle() const;

      const QString& getDownTitle() const;

      const QString& getLeftTitle() const;

      const QString& getRightTitle() const;

      const QString& getXLabel() const;

      const QString& getX2Label() const;

      const QString& getYLabel() const;

      const QString& getY2Label() const;
      //! destructor
      ~Graph() override;

     protected:
      static const qreal defaultLinearScaleMinValue;
      static const qreal defaultLinearScaleMaxValue;
      static const qreal defaultLogScaleMinValue;
      static const qreal defaultLogScaleMaxValue;
      static const qreal defaultWidth;
      static const qreal defaultHeight;
      static qreal log10(const qreal);
      struct Label;
      struct Arrow;
      // Override default signal handler to draw the rubber band (zoom
      // rectangle)
      //      virtual void contextMenuEvent(QContextMenuEvent *);
      virtual void mousePressEvent(QMouseEvent*) override;
      virtual void mouseMoveEvent(QMouseEvent*) override;
      virtual void mouseReleaseEvent(QMouseEvent*) override;
      virtual void resizeEvent(QResizeEvent*) override;
      // internal methods
      std::vector<std::shared_ptr<Graph::Label>>::iterator findLabel(
          const QString&);
      std::vector<std::shared_ptr<Graph::Arrow>>::iterator findArrow(
          const QString&);
      std::vector<CurveHandler>::iterator findCurveHandler(Curve* const);
      void exportToFile(const QString&, const char* const);
      void setTics(Graph::Axis&, const QMap<qreal, QString>&);
      void unsetTics(Graph::Axis&);
      void addCurve(CurveHandler&);
      void addCurve(CurveHandler&, Graph::Axis&);
      void setLogScale(Graph::Axis&);
      void unsetLogScale(Graph::Axis&);
      void plot(QGraphicsScene&,
                const unsigned short,
                const unsigned short);
      QString convertToString(const qreal);
      QString convertTicsToString(const qreal,
                                  const Graph::Axis::AxisScale);
      bool computeMinMax(qreal&, qreal&, Graph::Axis&);
      bool computeMinMax(Graph::Axis&);
      qreal convertHorizontalGraphCoordinatesToAbsolutePosition(
          const GraphLayout&, const GraphCoordinates) const;
      qreal convertVerticalGraphCoordinatesToAbsolutePosition(
          const GraphLayout&, const GraphCoordinates) const;
      void setRange(qreal&,
                    qreal&,
                    const qreal,
                    const qreal,
                    const Graph::Axis::AxisScale);
      void setRange(Graph::Axis&,
                    const qreal,
                    const qreal,
                    const bool = true);
      void updateBordersSize();
      void updateCurves(const Graph::Axis&);
      void updateCurve(CurveHandler&, const Graph::Axis&);
      void updateRange(Graph::Axis&);
      void updateRange(Graph::Axis&, const qreal, const qreal);
      void computePoints(CurveHandler&, const qreal, const qreal);
      void computeRange(qreal&, qreal&, const unsigned short);
      void computeRange2(qreal&, qreal&, const unsigned short);
      qreal computeTicsIncrements(const qreal, const qreal) const;
      void insertIntoTics(QMap<qreal, QString>&,
                          const qreal,
                          const qreal);
      void computeLinearScaleTics(Graph::Axis&);
      void computeLogScaleTics(QMap<qreal, QString>&, qreal&, qreal&);
      void computeTics(Graph::Axis&);
      void computeGraphLayout(const qreal, const qreal, GraphLayout&);

      QGraphicsScene* scene;
      GraphConfigurationDialog* gcd;

      mutable QGraphicsTextItem* cursorPosition;

      std::vector<CurveHandler> curves;
      std::shared_ptr<GraphTheme> theme;
      std::vector<std::shared_ptr<Label>> labels;
      std::vector<std::shared_ptr<Arrow>> arrows;
      Graph::Axis xAxis;
      Graph::Axis yAxis;
      Graph::Axis x2Axis;
      Graph::Axis y2Axis;
      QString lTitle;
      QString rTitle;
      QString uTitle;
      QString dTitle;
      QFont font;
      qreal zw;
      qreal zh;
      qreal zx1;
      qreal zx2;
      qreal zy1;
      qreal zy2;
      unsigned short width = Graph::defaultWidth;
      unsigned short height = Graph::defaultHeight;
      unsigned short samples;  // default number of sample for curve
      unsigned short grid = Grid::X | Grid::Y;
      KeyHorizontalPosition keyHorizontalPosition;
      KeyVerticalPosition keyVerticalPosition;
      KeyAlignment keyAlignment;
      QRect rubberBandRect;
      QPainter::RenderHints defaultRenderHints;
      QPainter::RenderHints highQualityRenderHints;
      bool showGraphGrid = false;
      bool showXAxis = false;
      bool showYAxis = false;
      bool showGraphBorder = true;
      bool hasXAxisCurve = false;
      bool hasX2AxisCurve = false;
      bool hasYAxisCurve = false;
      bool hasY2AxisCurve = false;
      bool showRubberBand = false;
      bool drawZoomRectangle = false;
    };  // end of struct graph

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPH_HXX */
