/*!
 * \file   Graph.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   04 jan 2008
 */

#include <cmath>
#include <limits>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <QtCore/QtDebug>
#include <QtCore/QBuffer>
#include <QtCore/QTextStream>
#include <QtCore/QtAlgorithms>
#include <QtWidgets/QMenu>
#include <QtWidgets/QGraphicsSceneContextMenuEvent>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsItem>
#include <QtPrintSupport/QPrinter>
#include <QtGui/QAction>
#include <QtGui/QMouseEvent>
#include <QtGui/QResizeEvent>
#include <QtSvg/QSvgGenerator>
#include "TFEL/Raise.hxx"
#include "TFEL/Plot/Grid.hxx"
#include "TFEL/Plot/Graph.hxx"
#include "TFEL/Plot/GraphSize.hxx"
#include "TFEL/Plot/ThemeManager.hxx"
#include "TFEL/Plot/GraphConfigurationDialog.hxx"

namespace tfel {

  namespace plot {

    const qreal Graph::defaultLinearScaleMinValue = -1.;
    const qreal Graph::defaultLinearScaleMaxValue = 1.;
    const qreal Graph::defaultLogScaleMinValue = 1.;
    const qreal Graph::defaultLogScaleMaxValue = 2.;

    const qreal Graph::defaultWidth = 800.;
    const qreal Graph::defaultHeight = 600.;

    QColor Graph::getColorByName(const QString& n) {
      if (n == "white") {
        return Qt::white;
      } else if (n == "black") {
        return Qt::black;
      } else if (n == "red") {
        return Qt::red;
      } else if (n == "darkRed") {
        return Qt::darkRed;
      } else if (n == "blue") {
        return Qt::blue;
      } else if (n == "darkBlue") {
        return Qt::darkBlue;
      } else if (n == "green") {
        return Qt::green;
      } else if (n == "darkGreen") {
        return Qt::darkGreen;
      } else if (n == "cyan") {
        return Qt::cyan;
      } else if (n == "darkCyan") {
        return Qt::darkCyan;
      } else if (n == "magenta") {
        return Qt::magenta;
      } else if (n == "darkMagenta") {
        return Qt::darkMagenta;
      } else if (n == "yellow") {
        return Qt::yellow;
      } else if (n == "darkYellow") {
        return Qt::darkYellow;
      } else if (n == "gray") {
        return Qt::gray;
      } else if (n == "darkGray") {
        return Qt::darkGray;
      } else if (n == "lightGray") {
        return Qt::lightGray;
      }
      return QColor();
    }  // end of Graph::getColorByName

    qreal Graph::log10(const qreal d) {
      if (d < std::numeric_limits<qreal>::min()) {
        std::ostringstream msg;
        msg << "Graph::log10 : "
            << "negative or zero value detected (" << d << ")";
        throw(std::runtime_error(msg.str()));
      }
      return std::log10(d);
    }  // end of Graph::log10

    Graph::Axis::Axis(const unsigned short id_) : id(id_) {
      this->reset();
    }  // end of Graph::Axis::Axis()

    void Graph::Axis::reset() {
      this->min = Graph::defaultLinearScaleMinValue;
      this->max = Graph::defaultLinearScaleMaxValue;
      this->label.clear();
      this->scale = Graph::Axis::LINEARSCALE;
      this->userDefinedRange = false;
      this->userDefinedTics = false;
    }  // end of Graph::Axis::reset

    struct Graph::Label {
      Label(const QString& l,
            const GraphCoordinates x_,
            const GraphCoordinates y_)
          : label(l), x(x_), y(y_) {}  // end of Label()
      Label(const QString& n,
            const QString& l,
            const GraphCoordinates x_,
            const GraphCoordinates y_)
          : name(n), label(l), x(x_), y(y_) {}  // end of Label()
      QString name;
      QString label;
      GraphCoordinates x;
      GraphCoordinates y;
    };  // end of struct Graph::Label

    struct Graph::Arrow {
      Arrow(const GraphCoordinates x0_,
            const GraphCoordinates y0_,
            const GraphCoordinates x1_,
            const GraphCoordinates y1_,
            const ArrowStyle s)
          : x0(x0_), y0(y0_), x1(x1_), y1(y1_), style(s) {}  // end of Arrow()
      Arrow(const QString& n,
            const GraphCoordinates x0_,
            const GraphCoordinates y0_,
            const GraphCoordinates x1_,
            const GraphCoordinates y1_,
            const ArrowStyle s)
          : name(n),
            x0(x0_),
            y0(y0_),
            x1(x1_),
            y1(y1_),
            style(s) {}  // end of Arrow()
      QString name;
      GraphCoordinates x0;
      GraphCoordinates y0;
      GraphCoordinates x1;
      GraphCoordinates y1;
      ArrowStyle style;
    };  // end of struct Graph::Arrow

    Graph::Graph(QWidget* const p)
        : QGraphicsView(p),
          gcd(new GraphConfigurationDialog(*this)),
          cursorPosition(nullptr),
          xAxis(Graph::xaxis),
          yAxis(Graph::yaxis),
          x2Axis(Graph::x2axis),
          y2Axis(Graph::y2axis) {
      this->reset();
      this->setTheme(
          ThemeManager::getThemeManager().getTheme("StandardGraphTheme"),
          false);
      this->scene = new QGraphicsScene(0, 0, this->width, this->height, this);
      this->setMinimumSize(this->width, this->height);
      this->defaultRenderHints = this->renderHints();
      this->setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform |
                           QPainter::TextAntialiasing);
      this->highQualityRenderHints = this->renderHints();
      // this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      // this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      this->setScene(scene);
    }  // end of Graph::Graph()

    int Graph::heightForWidth(int w) const {
      return static_cast<int>((static_cast<qreal>(this->width) / this->height) *
                              w);
    }

    void Graph::getCurvesTitles(std::vector<QString>& t) const {
      t.clear();
      for (const auto& ch : this->curves) {
        t.push_back(ch.curve->getKey());
      }
    }  // end of Graph::getCurvesTitles

    std::vector<std::shared_ptr<Curve>> Graph::getCurves() const {
      auto c = std::vector<std::shared_ptr<Curve>>{};
      for (const auto& ch : this->curves) {
        c.push_back(ch.curve);
      }
      return c;
    }  // end of Graph::getCurvesTitles

    void Graph::reset() {
      this->width = Graph::defaultWidth;
      this->height = Graph::defaultHeight;
      this->keyHorizontalPosition = RIGHT;
      this->keyVerticalPosition = TOP;
      this->keyAlignment = ALIGNRIGHT;
      this->samples = 100;
      this->font = QFont();
      this->font.setPointSize(12);
      // this->weight   = Cairo::FONT_WEIGHT_NORMAL;
      // this->slant    = Cairo::FONT_SLANT_NORMAL;
      this->grid = Grid::X | Grid::Y;
      this->showGraphGrid = false;
      this->showXAxis = false;
      this->showYAxis = false;
      this->showGraphBorder = true;
      this->hasXAxisCurve = false;
      this->hasX2AxisCurve = false;
      this->hasYAxisCurve = false;
      this->hasY2AxisCurve = false;
      this->curves.clear();
      this->labels.clear();
      this->arrows.clear();
      this->xAxis.reset();
      this->yAxis.reset();
      this->x2Axis.reset();
      this->y2Axis.reset();
      this->computeTics(this->xAxis);
      this->computeTics(this->yAxis);
      this->computeTics(this->x2Axis);
      this->computeTics(this->y2Axis);
    }  // end of Graph::reset

    void Graph::setGraphWidth(const qreal w) {
      if (w <= 0) {
        throw(
            std::runtime_error("Graph::setGraphWidth: "
                               "negative or null width specified"));
      }
      this->width = w;
    }  // end of Graph::setWidth

    void Graph::setGraphHeight(const qreal h) {
      if (h <= 0) {
        throw(
            std::runtime_error("Graph::setWidth: "
                               "negative or null width specified"));
      }
      this->height = h;
    }  // end of Graph::setHeight

    void Graph::unsetGraphWidth() {
      this->width = Graph::defaultWidth;
    }  // end of Graph::unsetWidth

    void Graph::unsetGraphHeight() {
      this->height = Graph::defaultHeight;
    }  // end of Graph::unsetHeight

    void Graph::setLogScale(Graph::Axis& axis) {
      bool abscissa = (axis.id & Graph::xaxis) || (axis.id & Graph::x2axis);
      bool found = false;
      if (axis.scale == Graph::Axis::LOGSCALE) {
        return;
      }
      for (const auto& ch : this->curves) {
        if (ch.axis & axis.id) {
          found = true;
          for (const auto& pt : ch.points) {
            const qreal value = (abscissa) ? pt.x : pt.y;
            if (value < std::numeric_limits<qreal>::min()) {
              if (ch.curve->hasKey()) {
                QMessageBox::critical(this, tr("Graph::setLogScale"),
                                      tr("negative value detected  (%1)"
                                         "for curve '%2'")
                                          .arg(value)
                                          .arg(ch.curve->getKey()));
                return;
              } else {
                QMessageBox::critical(this, tr("Graph::setLogScale"),
                                      tr("negative value detected  (%1)"
                                         "for an untitled curve")
                                          .arg(value));
                return;
              }
            }
          }
        }
      }
      for (auto& ch : this->curves) {
        if (ch.axis & axis.id) {
          for (auto& pt : ch.points) {
            if (abscissa) {
              pt.x = Graph::log10(pt.x);
            } else {
              pt.y = Graph::log10(pt.y);
            }
          }
        }
      }
      if (!axis.userDefinedRange) {
        if (found) {
          this->computeMinMax(axis);
        } else {
          axis.min = Graph::defaultLogScaleMinValue;
          axis.max = Graph::defaultLogScaleMaxValue;
          ;
        }
      } else {
        axis.min = Graph::log10(axis.min);
        axis.max = Graph::log10(axis.max);
      }
      axis.scale = Graph::Axis::LOGSCALE;
      this->setRange(axis, axis.min, axis.max, axis.userDefinedRange);
    }  // end of Graph::setLogScale

    void Graph::getRange(qreal& r0, qreal& r1, const unsigned short a) const {
      const Graph::Axis* axis;
      if (a == Graph::xaxis) {
        axis = &(this->xAxis);
      } else if (a == Graph::yaxis) {
        axis = &(this->yAxis);
      } else if (a == Graph::x2axis) {
        axis = &(this->x2Axis);
      } else if (a == Graph::y2axis) {
        axis = &(this->y2Axis);
      } else {
        throw(
            std::runtime_error("Graph::getRange: "
                               "invalid axis identifier"));
      }
      r0 = axis->min;
      r1 = axis->max;
    }  // end of Graph::getRange

    void Graph::unsetLogScale(Graph::Axis& axis) {
      bool abscissa = (axis.id & Graph::xaxis) || (axis.id & Graph::x2axis);
      bool found = false;
      if (axis.scale == Graph::Axis::LINEARSCALE) {
        return;
      }
      for (auto& ch : this->curves) {
        if (ch.axis & axis.id) {
          found = true;
          for (auto& pt : ch.points) {
            if (abscissa) {
              pt.x = std::pow(10., pt.x);
            } else {
              pt.y = std::pow(10., pt.y);
            }
          }
        }
      }
      if (!axis.userDefinedRange) {
        if (found) {
          this->computeMinMax(axis);
        } else {
          axis.min = Graph::defaultLinearScaleMinValue;
          axis.max = Graph::defaultLinearScaleMaxValue;
        }
      } else {
        axis.min = pow(10., axis.min);
        axis.max = pow(10., axis.max);
      }
      axis.scale = Graph::Axis::LINEARSCALE;
      this->setRange(axis, axis.min, axis.max, axis.userDefinedRange);
    }  // end of Graph::unsetLogScale

    void Graph::setXAxisLogScale(const bool b) {
      this->setLogScale(this->xAxis);
      if (b) {
        this->replot();
      }
      emit xAxisLogScaleSet();
    }  // end of Graph::setXAxisLogScale

    void Graph::unsetXAxisLogScale(const bool b) {
      this->unsetLogScale(this->xAxis);
      if (b) {
        this->replot();
      }
      emit xAxisLogScaleUnSet();
    }  // end of Graph::unsetXAxisLogScale

    void Graph::setX2AxisLogScale(const bool b) {
      this->setLogScale(this->x2Axis);
      if (b) {
        this->replot();
      }
      emit x2AxisLogScaleSet();
    }  // end of Graph::setX2AxisLogScale

    void Graph::unsetX2AxisLogScale(const bool b) {
      this->unsetLogScale(this->x2Axis);
      if (b) {
        this->replot();
      }
      emit x2AxisLogScaleUnSet();
    }  // end of Graph::unsetX2AxisLogScale

    void Graph::setYAxisLogScale(const bool b) {
      this->setLogScale(this->yAxis);
      if (b) {
        this->replot();
      }
      emit yAxisLogScaleSet();
    }  // end of Graph::setYAxisLogScale

    void Graph::unsetYAxisLogScale(const bool b) {
      this->unsetLogScale(this->yAxis);
      if (b) {
        this->replot();
      }
      emit yAxisLogScaleUnSet();
    }  // end of Graph::unsetYAxisLogScale

    void Graph::setY2AxisLogScale(const bool b) {
      this->setLogScale(this->y2Axis);
      if (b) {
        this->replot();
      }
      emit y2AxisLogScaleSet();
    }  // end of Graph::setY2AxisLogScale

    void Graph::unsetY2AxisLogScale(const bool b) {
      this->unsetLogScale(this->y2Axis);
      if (b) {
        this->replot();
      }
      emit y2AxisLogScaleUnSet();
    }  // end of Graph::unsetY2AxisLogScale

    qreal Graph::convertHorizontalGraphCoordinatesToAbsolutePosition(
        const GraphLayout& l, const GraphCoordinates c) const {
      qreal cx = c.c->getValue();
      qreal xp = 0.;
      switch (c.coordinateType) {
        case GraphCoordinates::FIRST:
          if (this->xAxis.scale == Graph::Axis::LOGSCALE) {
            cx = Graph::log10(cx);
          }
          xp = (cx - l.bx) / l.ax;
          break;
        default:
          throw(std::runtime_error(
              "Graph::"
              "convertHorizontalGraphCoordinatesToAbsolutePosition: "
              "unsupported coordinate type"));
          break;
      }
      return xp;
    }  // end of
       // Graph::convertHorizontalGraphCoordinatesToAbsolutePosition

    qreal Graph::convertVerticalGraphCoordinatesToAbsolutePosition(
        const GraphLayout& l, const GraphCoordinates c) const {
      qreal cy = c.c->getValue();
      qreal yp = 0.;
      switch (c.coordinateType) {
        case GraphCoordinates::FIRST:
          if (this->yAxis.scale == Graph::Axis::LOGSCALE) {
            cy = Graph::log10(cy);
          }
          yp = l.ay * cy + l.by;
          break;
        default:
          tfel::raise(
              "Graph::"
              "convertVerticalGraphCoordinatesToAbsolutePosition: "
              "unsupported coordinate type");
      }
      return yp;
    }  // end of
       // Graph::convertVerticalGraphCoordinatesToAbsolutePosition

    void Graph::setTheme(const std::shared_ptr<GraphTheme>& t, const bool b) {
      this->theme = t;
      if (b) {
        this->replot();
      }
    }  // end of Graph::setTheme

    Graph::~Graph() { this->removeCurves(false); }  // end of Graph::~Graph()

    qreal Graph::computeTicsIncrements(const qreal min, const qreal max) const {
      using namespace std;
      const qreal guide = 20.;
      const qreal e = max - min;
      // magnitude
      const qreal n = pow(10., floor(Graph::log10(e)));
      // normalised range
      const qreal xnorm = e / n;
      // dnorm shall be between 0 and 10
      qreal posns = guide / xnorm; /* approx number of tic posns per
                                      decade */
      /* with guide=20, we expect 2 <= posns <= 20 */
      qreal d;
      if (posns > 40) {
        d = 0.05; /* eg 0, .05, .10, ... */
      } else if (posns > 20) {
        d = 0.1; /* eg 0, .1, .2, ... */
      } else if (posns > 10) {
        d = 0.2; /* eg 0,0.2,0.4,... */
      } else if (posns > 4) {
        d = 0.5; /* 0,0.5,1, */
      } else if (posns > 2) {
        d = 1; /* 0,1,2,.... */
      } else if (posns > 0.5) {
        d = 2; /* 0, 2, 4, 6 */
      } else {
        d = ceil(xnorm);
      }
      return (d * n);
    }  // end of Graph::computeTicsIncrements

    QString Graph::convertToString(const qreal d) {
      using namespace std;
      if (abs(d) < numeric_limits<qreal>::min()) {
        return "0";
      }
      const qreal ad = abs(d);
      const qreal m = floor(log10(ad));
      if (abs(m) > 3.5) {
        QLocale l;
        const qreal v = ad / pow(10, m);
        QString r;
        if (d < 0) {
          r = QChar('-');
        }
        if (abs(v - 1) > 10 * numeric_limits<qreal>::epsilon()) {
          r += l.toString(v);
          r += " &times; ";
        }
        r += "10<sup>" + QString::number(static_cast<int>(m)) + "</sup>";
        return r;
      }
      ostringstream res;
      res << d;
      return QString::fromStdString(res.str());
    }  // end of Graph::convertToString

    QString Graph::convertTicsToString(const qreal d,
                                       const Graph::Axis::AxisScale s) {
      if (s == Graph::Axis::LOGSCALE) {
        return Graph::convertToString(pow(10., d));
      }
      return Graph::convertToString(d);
    }

    void Graph::computeLinearScaleTics(Graph::Axis& axis) {
      using namespace std;
      const qreal d = this->computeTicsIncrements(axis.min, axis.max);
      const qreal start = d * floor(axis.min / d);
      const qreal end = d * ceil(axis.max / d);
      const qreal prec = 1e-3 * abs(d);
      qreal t = start;
      axis.tics.clear();
      while (t < end + 0.5 * d) {
        if (!(((t < axis.min) && (!(qAbs(t - axis.min) < prec))) ||
              ((t > axis.max) && (!(qAbs(t - axis.max) < prec))))) {
          if (abs(ceil(t) - t) < prec) {
            if (abs(t) < prec) {
              axis.tics.insert({0., convertToString(0.)});
            } else {
              axis.tics.insert({ceil(t), convertToString(ceil(t))});
            }
          } else if (abs(floor(t) - t) < prec) {
            if (abs(t) < prec) {
              axis.tics.insert({0., convertToString(0.)});
            } else {
              axis.tics.insert({floor(t), convertToString(floor(t))});
            }
          } else {
            axis.tics.insert({t, convertToString(t)});
          }
        }
        t += d;
      }
    }  // end of Graph::computeLinearScaleTics

    void Graph::insertIntoTics(std::map<qreal, QString>& tics,
                               const qreal d,
                               const qreal d2) {
      tics.insert({d, Graph::convertToString(d2)});
    }  // end of Graph::insertIntoTics

    void Graph::computeLogScaleTics(std::map<qreal, QString>& tics,
                                    qreal& logmin,
                                    qreal& logmax) {
      using namespace std;
      static const qreal log10_2 = log10(2);
      static const qreal log10_3 = log10(3);
      static const qreal log10_4 = log10(4);
      static const qreal log10_5 = log10(5);
      static const qreal log10_6 = log10(6);
      static const qreal log10_7 = log10(7);
      static const qreal log10_8 = log10(8);
      static const qreal log10_9 = log10(9);
      logmin = floor(logmin);
      logmax = ceil(logmax);
      qreal min = pow(10., logmin);
      qreal deca = ceil(logmax - logmin);
      qreal nb = deca / 4;
      unsigned short i;
      tics.clear();
      if (nb > 1) {
        for (i = 0; i != 5; ++i) {
          this->insertIntoTics(tics, logmin + ceil(nb) * i,
                               pow(10, logmin + ceil(nb) * i));
          // tics.insert(logmin+ceil(nb)*(i+1)/5,"");
          // tics.insert(logmin+ceil(nb)*(i+1)/5,"");
          // tics.insert(logmin+ceil(nb)*(i+1)/5,"");
          // tics.insert(log10_2+logmin+i,"");
          // tics.insert(log10_3+logmin+i,"");
          // tics.insert(log10_4+logmin+i,"");
          // tics.insert(log10_5+logmin+i,"");
          // tics.insert(log10_6+logmin+i,"");
          // tics.insert(log10_7+logmin+i,"");
          // tics.insert(log10_8+logmin+i,"");
          // tics.insert(log10_9+logmin+i,"");
        }
        this->insertIntoTics(tics, logmin + ceil(nb) * 5,
                             pow(10, logmin + ceil(nb) * 5));
      } else if (nb >= 0.75) {
        for (i = 0; i != 4; ++i) {
          this->insertIntoTics(tics, logmin + ceil(nb) * i,
                               pow(10, logmin + ceil(nb) * i));
          // tics.insert(log10_2+logmin+i,"");
          // tics.insert(log10_3+logmin+i,"");
          // tics.insert(log10_4+logmin+i,"");
          // tics.insert(log10_5+logmin+i,"");
          // tics.insert(log10_6+logmin+i,"");
          // tics.insert(log10_7+logmin+i,"");
          // tics.insert(log10_8+logmin+i,"");
          // tics.insert(log10_9+logmin+i,"");
        }
        this->insertIntoTics(tics, logmin + ceil(nb) * 4,
                             pow(10, logmin + ceil(nb) * 4));
      } else if (nb >= 0.5) {
        this->insertIntoTics(tics, logmin, min);
        tics.insert({log10_2 + logmin, ""});
        tics.insert({log10_3 + logmin, ""});
        tics.insert({log10_4 + logmin, ""});
        tics.insert({log10_5 + logmin, ""});
        tics.insert({log10_6 + logmin, ""});
        tics.insert({log10_7 + logmin, ""});
        tics.insert({log10_8 + logmin, ""});
        tics.insert({log10_9 + logmin, ""});
        this->insertIntoTics(tics, logmin + 1, min * 10);
        tics.insert({log10_2 + logmin + 1., ""});
        tics.insert({log10_3 + logmin + 1., ""});
        tics.insert({log10_4 + logmin + 1., ""});
        tics.insert({log10_5 + logmin + 1., ""});
        tics.insert({log10_6 + logmin + 1., ""});
        tics.insert({log10_7 + logmin + 1., ""});
        tics.insert({log10_8 + logmin + 1., ""});
        tics.insert({log10_9 + logmin + 1., ""});
        this->insertIntoTics(tics, logmin + 2, min * 100);
      } else {
        this->insertIntoTics(tics, logmin, min);
        tics.insert({log10_2 + logmin, ""});
        tics.insert({log10_3 + logmin, ""});
        tics.insert({log10_4 + logmin, ""});
        tics.insert({log10_5 + logmin, ""});
        tics.insert({log10_6 + logmin, ""});
        tics.insert({log10_7 + logmin, ""});
        tics.insert({log10_8 + logmin, ""});
        tics.insert({log10_9 + logmin, ""});
        this->insertIntoTics(tics, logmin + 1, min * 10);
      }
    }  // end of Graph::computeLogScaleTics

    void Graph::computeTics(Graph::Axis& axis) {
      if (axis.scale == Graph::Axis::LOGSCALE) {
        this->computeLogScaleTics(axis.tics, axis.min, axis.max);
      } else {
        this->computeLinearScaleTics(axis);
      }
    }  // end of Graph::computeTics

    void Graph::computeGraphLayout(const qreal gwidth,
                                   const qreal gheight,
                                   GraphLayout& l) {
      // margins
      l.ml = this->theme->getLeftMargin();
      l.mr = this->theme->getRightMargin();
      l.mu = this->theme->getUpperMargin();
      l.md = this->theme->getDownMargin();
      // titles
      l.ttl = this->theme->getLeftTitleWidth(*this, this->lTitle);
      l.ttr = this->theme->getRightTitleWidth(*this, this->rTitle);
      l.ttu = this->theme->getUpperTitleHeight(*this, this->uTitle);
      l.ttd = this->theme->getDownTitleHeight(*this, this->dTitle);
      // // labels
      l.ll = this->theme->getYLabelWidth(*this, this->yAxis.label);
      l.lr = this->theme->getY2LabelWidth(*this, this->y2Axis.label);
      l.ld = this->theme->getXLabelHeight(*this, this->xAxis.label);
      l.lu = this->theme->getX2LabelHeight(*this, this->x2Axis.label);
      // tics
      if (this->hasYAxisCurve) {
        l.tl = this->theme->getYTicsWidth(*this, this->yAxis.tics);
      } else {
        l.tl = 0u;
      }
      if (this->hasY2AxisCurve) {
        l.tr = this->theme->getY2TicsWidth(*this, this->y2Axis.tics);
      } else {
        l.tr = 0u;
      }
      if (this->hasXAxisCurve) {
        l.td = this->theme->getXTicsHeight(*this, this->xAxis.tics);
      } else {
        l.td = 0u;
      }
      if (this->hasX2AxisCurve) {
        l.tu = this->theme->getX2TicsHeight(*this, this->x2Axis.tics);
      } else {
        l.tu = 0u;
      }
      // graph borders
      this->theme->getGraphBorders(*this, l.gl, l.gr, l.gu, l.gd);
      // summing-up
      l.bl = l.ml + l.ttl + l.ll + l.tl + l.gl;
      l.br = l.mr + l.ttr + l.lr + l.tr + l.gr;
      l.bu = l.mu + l.ttu + l.lu + l.tu + l.gu;
      l.bd = l.md + l.ttd + l.ld + l.td + l.gd;
      // transformations
      const qreal xh0 = l.bl;            // left border
      const qreal xh1 = gwidth - l.br;   // right border
      const qreal yh0 = l.bu;            // up border
      const qreal yh1 = gheight - l.bd;  // down border
      l.ax = (this->xAxis.max - this->xAxis.min) / (xh1 - xh0);
      l.bx = 0.5 * (this->xAxis.max + this->xAxis.min - l.ax * (xh1 + xh0));
      l.ay = (yh0 - yh1) / (this->yAxis.max - this->yAxis.min);
      l.by = 0.5 * (yh0 + yh1 - l.ay * (this->yAxis.max + this->yAxis.min));
      l.ax2 = (this->x2Axis.max - this->x2Axis.min) / (xh1 - xh0);
      l.bx2 = 0.5 * (this->x2Axis.max + this->x2Axis.min - l.ax2 * (xh1 + xh0));
      l.ay2 = (yh0 - yh1) / (this->y2Axis.max - this->y2Axis.min);
      l.by2 = 0.5 * (yh0 + yh1 - l.ay2 * (this->y2Axis.max + this->y2Axis.min));
    }  // end of Graph::computeGraphLayout

    void Graph::setGrid(const unsigned short g, const bool b) {
      this->grid = g;
      if (b) {
        this->replot();
      }
      emit gridSet(g);
    }  // end of Graph::setGrid

    unsigned short Graph::getGrid() const {
      return this->grid;
    }  // end of Graph::getGrid

    void Graph::plot(QGraphicsScene&,
                     const unsigned short pwidth,
                     const unsigned short pheight) {
      using namespace std;

      qDeleteAll(this->scene->items());
      this->cursorPosition = nullptr;

      GraphSize s;
      GraphLayout l;
      std::vector<CurveHandler>::iterator p;
      std::vector<shared_ptr<Label>>::const_iterator p3;
      std::vector<shared_ptr<Arrow>>::const_iterator p4;
      unsigned short nbr;

      this->computeGraphLayout(pwidth, pheight, l);

      s.width = pwidth;
      s.height = pheight;
      s.xh0 = l.bl;             // left border
      s.xh1 = s.width - l.br;   // right border
      s.yh0 = l.bu;             // up border
      s.yh1 = s.height - l.bd;  // down border

      // draw the graph back ground
      this->theme->drawBackGround(*this, l, s);

      // Titles
      if (!this->uTitle.isEmpty()) {
        this->theme->printUpperTitle(*this, l, s, this->uTitle);
      }
      if (!this->dTitle.isEmpty()) {
        this->theme->printDownTitle(*this, l, s, this->dTitle);
      }
      if (!this->lTitle.isEmpty()) {
        this->theme->printLeftTitle(*this, l, s, this->lTitle);
      }
      if (!this->rTitle.isEmpty()) {
        this->theme->printRightTitle(*this, l, s, this->rTitle);
      }

      // Labels
      if (!this->xAxis.label.isEmpty()) {
        this->theme->printXLabel(*this, l, s, this->xAxis.label);
      }
      if (!this->yAxis.label.isEmpty()) {
        this->theme->printYLabel(*this, l, s, this->yAxis.label);
      }
      if (!this->x2Axis.label.isEmpty()) {
        this->theme->printX2Label(*this, l, s, this->x2Axis.label);
      }
      if (!this->y2Axis.label.isEmpty()) {
        this->theme->printY2Label(*this, l, s, this->y2Axis.label);
      }

      // draw tics
      if (this->hasXAxisCurve) {
        this->theme->printXTics(*this, l, s, this->xAxis.tics);
      }
      if (this->hasYAxisCurve) {
        this->theme->printYTics(*this, l, s, this->yAxis.tics);
      }
      if (this->hasX2AxisCurve) {
        this->theme->printX2Tics(*this, l, s, this->x2Axis.tics);
      }
      if (this->hasY2AxisCurve) {
        this->theme->printY2Tics(*this, l, s, this->y2Axis.tics);
      }

      // // draw the graph back ground
      this->theme->drawGraphBackGround(*this, l, s);

      // draw curves
      nbr = 1u;
      auto* d =
          this->scene->addRect(s.xh0, s.yh0, s.xh1 - s.xh0, s.yh1 - s.yh0);
      d->setPen(Qt::NoPen);
      d->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
      for (p = this->curves.begin(); p != this->curves.end(); ++p, ++nbr) {
        if (p->axis == Graph::XY) {
          this->theme->drawCurve(*this, *d, p->curve.get(), p->points, s, l.ax,
                                 l.bx, l.ay, l.by, nbr);
        } else if (p->axis == Graph::X2Y) {
          this->theme->drawCurve(*this, *d, p->curve.get(), p->points, s, l.ax2,
                                 l.bx2, l.ay, l.by, nbr);
        } else if (p->axis == Graph::XY2) {
          this->theme->drawCurve(*this, *d, p->curve.get(), p->points, s, l.ax,
                                 l.bx, l.ay2, l.by2, nbr);
        } else {
          this->theme->drawCurve(*this, *d, p->curve.get(), p->points, s, l.ax2,
                                 l.bx2, l.ay2, l.by2, nbr);
        }
      }

      this->theme->drawGrid(
          *this, l, s, this->showGraphGrid, this->grid, this->xAxis.tics,
          this->yAxis.tics, this->x2Axis.tics, this->y2Axis.tics,
          this->xAxis.min, this->xAxis.max, this->yAxis.min, this->yAxis.max,
          this->x2Axis.min, this->x2Axis.max, this->y2Axis.min,
          this->y2Axis.max, this->hasXAxisCurve, this->hasYAxisCurve,
          this->hasX2AxisCurve, this->hasY2AxisCurve);

      // x-axis
      if (this->showXAxis) {
        this->theme->drawXAxis(*d, l, s);
      }

      // y-axis
      if (this->showYAxis) {
        this->theme->drawYAxis(*d, l, s);
      }

      // legends
      this->theme->printKeys(*this, this->curves, l, s,
                             this->keyHorizontalPosition,
                             this->keyVerticalPosition, this->keyAlignment);

      // border
      if (this->showGraphBorder) {
        this->theme->drawGraphBorder(*this, l, s);
      }

      // draw the graph fore ground
      this->theme->drawGraphForeGround(*this, l, s);

      // print labels
      for (p3 = this->labels.begin(); p3 != this->labels.end(); ++p3) {
        const Label& pl = *(*p3);
        this->theme->printLabel(
            *this,
            this->convertHorizontalGraphCoordinatesToAbsolutePosition(l, pl.x),
            this->convertVerticalGraphCoordinatesToAbsolutePosition(l, pl.y),
            pl.label);
      }

      // draw arrows
      for (p4 = this->arrows.begin(); p4 != this->arrows.end(); ++p4) {
        const Arrow& pa = *(*p4);
        this->theme->drawArrow(
            *this,
            this->convertHorizontalGraphCoordinatesToAbsolutePosition(l, pa.x0),
            this->convertVerticalGraphCoordinatesToAbsolutePosition(l, pa.y0),
            this->convertHorizontalGraphCoordinatesToAbsolutePosition(l, pa.x1),
            this->convertVerticalGraphCoordinatesToAbsolutePosition(l, pa.y1),
            pa.style);
      }
      if (this->showRubberBand) {
        this->theme->drawRubberBand(*this, this->rubberBandRect);
      }
      emit updated();
    }  // end of Graph::plot

    QByteArray Graph::exportToSVG() {
      QBuffer b;
      if (this->cursorPosition != nullptr) {
        this->scene->removeItem(this->cursorPosition);
        delete this->cursorPosition;
        this->cursorPosition = nullptr;
      }
      QSvgGenerator p;
      p.setOutputDevice(&b);
      p.setSize(QSize(800, 600));
      p.setViewBox(QRect(0, 0, 800, 600));
      QPainter painter;
      painter.begin(&p);
      painter.setRenderHint(QPainter::Antialiasing);
      this->scene->render(&painter);
      painter.end();
      return b.buffer();
    }

    QImage Graph::exportToQImage() {
      // Selections would also render to the file
      this->scene->clearSelection();
      // remove cursor position
      if (this->cursorPosition != nullptr) {
        this->scene->removeItem(this->cursorPosition);
        delete this->cursorPosition;
        this->cursorPosition = nullptr;
      }
      // Create the image with the exact size of the shrunk scene
      QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
      image.fill(Qt::transparent);
      QPainter painter;
      painter.begin(&image);
      painter.setRenderHint(QPainter::Antialiasing);
      this->scene->render(&painter);
      painter.end();
      return image;
    }

    void Graph::insertImage(const QPixmap& i) { this->scene->addPixmap(i); }

    void Graph::exportToTable(const QString& file) {
      std::ofstream f(file.toStdString().c_str());
      if (!f) {
        QMessageBox::critical(this, tr("Graph::exportToTable"),
                              tr("Can't open file '%1'").arg(file));
        return;
      }
      for (const auto& c : this->curves) {
        f << "#" << c.curve->getKey().toStdString() << '\n';
        for (auto& pt : c.points) {
          f << pt.x << " " << pt.y << '\n';
        }
        f << '\n';
      }
    }

    QString Graph::exportToTable() {
      QString out;
      QTextStream table(&out);
      int m = 0;
      for (const auto& c : this->curves) {
        m = std::max(m, static_cast<int>(c.points.size()));
      }
      for (int i = 0; i != m; ++i) {
        for (auto p = this->curves.begin(); p != this->curves.end();) {
          if (p->points.size() > i) {
            table << p->points[i].x << " " << p->points[i].y;
          }
          if (++p != this->curves.end()) {
            table << " ";
          }
        }
        table << "\n";
      }
      return out;
    }  // end of Graph::exportToTable

    void Graph::exportToSVG(const QString& file) {
      if (this->cursorPosition != nullptr) {
        this->scene->removeItem(this->cursorPosition);
        delete this->cursorPosition;
        this->cursorPosition = nullptr;
      }
      QSvgGenerator p;
      p.setFileName(file);
      p.setSize(QSize(800, 600));
      p.setViewBox(QRect(0, 0, 800, 600));
      QPainter painter;
      painter.begin(&p);
      painter.setRenderHint(QPainter::Antialiasing);
      this->scene->render(&painter);
      painter.end();
    }  // end of Graph::exportToSVG

    void Graph::exportToFile(const QString& file, const char* const f) {
      if (this->cursorPosition != nullptr) {
        this->scene->removeItem(this->cursorPosition);
        delete this->cursorPosition;
        this->cursorPosition = nullptr;
      }
      QPixmap p(640, 480);
      QPainter painter(&p);
      painter.setRenderHint(QPainter::Antialiasing);
      this->scene->render(&painter);
      painter.end();
      if (!p.save(file, f)) {
        QMessageBox::critical(this, tr("Graph::export"),
                              tr("can't export to file '%1'"
                                 "using format '%2'")
                                  .arg(file)
                                  .arg(f));
      }
    }  // end of Graph::export

    void Graph::exportToPNG(const QString& file) {
      this->exportToFile(file, "PNG");
    }  // end of Graph::exportToPNG

    void Graph::exportToBMP(const QString& file) {
      this->exportToFile(file, "BMP");
    }  // end of Graph::exportToBMP

    void Graph::exportToJPEG(const QString& file) {
      this->exportToFile(file, "JPEG");
    }  // end of Graph::exportToJPEG

    void Graph::exportToPPM(const QString& file) {
      this->exportToFile(file, "PPM");
    }  // end of Graph::exportToPPM

    void Graph::exportToXBM(const QString& file) {
      this->exportToFile(file, "XBM");
    }  // end of Graph::exportToXBM

    void Graph::exportToXPM(const QString& file) {
      this->exportToFile(file, "XPM");
    }  // end of Graph::exportToXPM

    void Graph::exportToPDF(const QString& file) {
      if (this->cursorPosition != nullptr) {
        this->scene->removeItem(this->cursorPosition);
        delete this->cursorPosition;
        this->cursorPosition = nullptr;
      }
      QPrinter printer(QPrinter::HighResolution);
      printer.setOutputFormat(QPrinter::PdfFormat);
      printer.setOutputFileName(file);
      //      printer.setPaperSize(QPrinter::Custom);
#pragma "warning"
      // printer.setPageSize(this->scene->sceneRect().size().toSize(),
      // 			  QPrinter::Point);
      //      printer.setPageMargins(0, 0, 0, 0, QPrinter::Point);
      QPainter painter(&printer);
      this->scene->render(&painter);
    }  // end of Graph::print

    void Graph::print(QPrinter* p) {
      if (this->cursorPosition != nullptr) {
        this->scene->removeItem(this->cursorPosition);
        delete this->cursorPosition;
        this->cursorPosition = nullptr;
      }
      QPainter painter(p);
      this->scene->render(&painter);
    }  // end of Graph::printer

    void Graph::removeCurves(const bool b) {
      this->curves.clear();
      this->hasXAxisCurve = false;
      this->hasX2AxisCurve = false;
      this->hasYAxisCurve = false;
      this->hasY2AxisCurve = false;
      if (b) {
        this->replot();
        emit curvesRemoved();
      }
    }  // end of Graph::removeCurves

    void Graph::computeRange(qreal& x0, qreal& x1, const unsigned short axis) {
      bool found = false;
      for (const auto& c : this->curves) {
        if ((c.curve->hasRange()) && (c.axis & axis)) {
          const qreal pmin = c.curve->minRange();
          const qreal pmax = c.curve->maxRange();
          if (!found) {
            x0 = pmin;
            x1 = pmax;
          } else {
            x0 = qMin(x0, pmin);
            x1 = qMax(x1, pmax);
          }
          found = true;
        }
      }  // end of for p
      if (!found) {
        x0 = Graph::defaultLinearScaleMinValue;
        x1 = Graph::defaultLinearScaleMaxValue;
      }
    }  // end of Graph::computeRange()

    void Graph::computeRange2(qreal& x0, qreal& x1, const unsigned short axis) {
      bool logx = false;
      if (this->xAxis.id == axis) {
        logx = this->xAxis.scale == Graph::Axis::LOGSCALE;
      } else {
        logx = this->x2Axis.scale == Graph::Axis::LOGSCALE;
      }
      for (const auto& c : this->curves) {
        if ((c.curve->hasRange()) && (c.axis & axis)) {
          auto pmin = c.curve->minRange();
          auto pmax = c.curve->maxRange();
          if (logx) {
            pmin = Graph::log10(pmin);
            pmax = Graph::log10(pmax);
          }
          x0 = qMin(pmin, x0);
          x1 = qMax(pmax, x1);
        }
      }  // end of for p
    }    // end of Graph::computeRange2

    void Graph::computePoints(CurveHandler& h,
                              const qreal amin,
                              const qreal amax) {
      bool logx = false;
      bool logy = false;
      if (h.axis & Graph::xaxis) {
        logx = this->xAxis.scale == Graph::Axis::LOGSCALE;
      } else {
        logx = this->x2Axis.scale == Graph::Axis::LOGSCALE;
      }
      if (h.axis & Graph::yaxis) {
        logy = this->yAxis.scale == Graph::Axis::LOGSCALE;
      } else {
        logy = this->y2Axis.scale == Graph::Axis::LOGSCALE;
      }
      if (h.curve->hasSpecifiedNumberOfSamples()) {
        h.curve->getValues(h.points, amin, amax, h.curve->getNumberOfSamples());
      } else {
        h.curve->getValues(h.points, amin, amax, this->samples);
      }
      if (logx) {
        for (auto& pt : h.points) {
          pt.x = Graph::log10(pt.x);
        }
      }
      if (logy) {
        for (auto& pt : h.points) {
          pt.y = Graph::log10(pt.y);
        }
      }
    }  // end of Graph::computePoints

    void Graph::addCurve(CurveHandler& h) {
      this->curves.push_back(h);
      Graph::Axis& a1 = h.axis & Graph::xaxis ? this->xAxis : this->x2Axis;
      Graph::Axis& a2 = h.axis & Graph::yaxis ? this->yAxis : this->y2Axis;
      if (a2.userDefinedRange) {
        this->updateCurve(this->curves.back(), a1);
      } else {
        this->updateCurve(this->curves.back(), a1);
        this->updateRange(a2);
      }
    }  // end of Graph::addCurve

    void Graph::addCurve(CurveHandler& h, Graph::Axis& axis) {
      if (!axis.userDefinedRange) {
        if (h.curve->hasRange()) {
          qreal x0 = h.curve->minRange();
          qreal x1 = h.curve->maxRange();
          if (axis.scale == Graph::Axis::LOGSCALE) {
            x0 = Graph::log10(x0);
            x1 = Graph::log10(x1);
          }
          this->computeRange2(x0, x1, axis.id);
          this->curves.push_back(h);
          this->updateRange(axis, x0, x1);
        } else {
          this->addCurve(h);
        }
      } else {
        this->addCurve(h);
      }
    }  // end of Graph::addCurve

    void Graph::addCurve(std::shared_ptr<Curve> c, const GraphAxis axis) {
      using namespace std;
      CurveHandler h;
      if (c.get() == nullptr) {
        return;
      }
      h.curve = c;
      h.axis = axis;
      if (axis & Graph::xaxis) {
        this->hasXAxisCurve = true;
      } else {
        this->hasX2AxisCurve = true;
      }
      if (axis & Graph::yaxis) {
        this->hasYAxisCurve = true;
      } else {
        this->hasY2AxisCurve = true;
      }
      if (axis & Graph::xaxis) {
        this->addCurve(h, this->xAxis);
      } else {
        this->addCurve(h, this->x2Axis);
      }
      // if(!c->hasKey()){
      // 	ostringstream nbr;
      // 	nbr << "Curve " << this->curves.size();
      // 	c->setKey(QString::fromStdString(nbr.str()),true);
      // }
      QObject::connect(c.get(), &Curve::updated, this, &Graph::updatedCurve);
      QObject::connect(c.get(), &Curve::remove, this,
                       [this](Curve* rc) { this->removeCurve(rc); });
      emit curveAdded(c.get());
    }  // end of Graph::addCurve

    void Graph::setUpperTitle(const QString& l, const bool b) {
      this->uTitle = l;
      if (b) {
        this->replot();
      }
      emit upperTitleChanged(l);
    }  // end of Graph::setUpperTitle

    void Graph::setDownTitle(const QString& l, const bool b) {
      this->dTitle = l;
      if (b) {
        this->replot();
      }
      emit downTitleChanged(l);
    }  // end of Graph::setDownTitle

    void Graph::setLeftTitle(const QString& l, const bool b) {
      this->lTitle = l;
      if (b) {
        this->replot();
      }
      emit leftTitleChanged(l);
    }  // end of Graph::setLeftTitle

    void Graph::setRightTitle(const QString& l, const bool b) {
      this->rTitle = l;
      if (b) {
        this->replot();
      }
      emit rightTitleChanged(l);
    }  // end of Graph::setRightTitle

    void Graph::setXLabel(const QString& l, const bool b) {
      this->xAxis.label = l;
      if (b) {
        this->replot();
      }
      emit xLabelChanged(l);
    }  // end of Graph::setXLabel

    void Graph::setYLabel(const QString& l, const bool b) {
      this->yAxis.label = l;
      if (b) {
        this->replot();
      }
      emit yLabelChanged(l);
    }  // end of Graph::setYLabel

    void Graph::setX2Label(const QString& l, const bool b) {
      this->x2Axis.label = l;
      if (b) {
        this->replot();
      }
      emit x2LabelChanged(l);
    }  // end of Graph::setX2Label

    void Graph::setY2Label(const QString& l, const bool b) {
      this->y2Axis.label = l;
      if (b) {
        this->replot();
      }
      emit y2LabelChanged(l);
    }  // end of Graph::setY2Label

    QFont Graph::getGraphFont() const {
      return this->font;
    }  // end of Graph::getGraphFont

    void Graph::setGraphFont(const QFont& f) {
      this->font = f;
      emit fontChanged(this->font);
      emit updated();
    }

    void Graph::setGraphFontFamily(const QString& family) {
      this->font.setFamily(family);
      emit fontChanged(this->font);
      emit updated();
    }  // end of Graph::setFontFamily

    void Graph::setGraphFontSize(const qreal fs) {
      this->font.setPointSize(fs);
      emit fontChanged(this->font);
      emit updated();
    }  // end of Graph::setFontFamily

    void Graph::unsetXRange(const bool b) {
      this->xAxis.userDefinedRange = false;
      this->computeRange(this->xAxis.min, this->xAxis.max, Graph::xaxis);
      if (this->xAxis.scale == Graph::Axis::LOGSCALE) {
        this->updateRange(this->xAxis, Graph::log10(this->xAxis.min),
                          Graph::log10(this->xAxis.max));
      } else {
        this->updateRange(this->xAxis, this->xAxis.min, this->xAxis.max);
      }
      if (b) {
        this->replot();
      }
      emit xRangeUnSet();
    }  // end of Graph::unsetXRange()

    void Graph::unsetX2Range(const bool b) {
      this->x2Axis.userDefinedRange = false;
      this->computeRange(this->x2Axis.min, this->x2Axis.max, Graph::x2axis);
      if (this->x2Axis.scale == Graph::Axis::LOGSCALE) {
        this->updateRange(this->x2Axis, Graph::log10(this->x2Axis.min),
                          Graph::log10(this->x2Axis.max));
      } else {
        this->updateRange(this->x2Axis, this->x2Axis.min, this->x2Axis.max);
      }
      if (b) {
        this->replot();
      }
      emit x2RangeUnSet();
    }  // end of Graph::unsetX2Range(const bool b)

    void Graph::unsetYRange(const bool b) {
      this->yAxis.userDefinedRange = false;
      this->updateRange(this->yAxis);
      if (b) {
        this->replot();
      }
      emit yRangeUnSet();
    }  // end of Graph::unsetYRange(const bool b)

    void Graph::unsetY2Range(const bool b) {
      this->y2Axis.userDefinedRange = false;
      this->updateRange(this->y2Axis);
      if (b) {
        this->replot();
      }
      emit yRangeUnSet();
    }  // end of Graph::unsetY2Range()

    void Graph::setXRange(const qreal x0, const qreal x1, const bool b) {
      this->xAxis.userDefinedRange = true;
      if (this->xAxis.scale == Graph::Axis::LOGSCALE) {
        this->updateRange(this->xAxis, Graph::log10(x0), Graph::log10(x1));
      } else {
        this->updateRange(this->xAxis, x0, x1);
      }
      if (b) {
        this->replot();
      }
      emit xRangeChanged(x0, x1);
    }  // end of Graph::setXRange

    void Graph::setX2Range(const qreal x0, const qreal x1, const bool b) {
      this->x2Axis.userDefinedRange = true;
      if (this->x2Axis.scale == Graph::Axis::LOGSCALE) {
        this->updateRange(this->x2Axis, Graph::log10(x0), Graph::log10(x1));
      } else {
        this->updateRange(this->x2Axis, x0, x1);
      }
      if (b) {
        this->replot();
      }
      emit x2RangeChanged(x0, x1);
    }  // end of Graph::setX2Range

    void Graph::setTics(Graph::Axis& axis,
                        const std::map<qreal, QString>& tics) {
      axis.userDefinedTics = true;
      axis.tics.clear();
      axis.tics = tics;
    }  // end of Graph::setXTics

    void Graph::setXTics(const std::map<qreal, QString>& tics, const bool b) {
      this->setTics(this->xAxis, tics);
      if (b) {
        this->replot();
      }
      emit xTicsChanged();
    }  // end of Graph::setXTics

    void Graph::setX2Tics(const std::map<qreal, QString>& tics, const bool b) {
      this->setTics(this->x2Axis, tics);
      if (b) {
        this->replot();
      }
      emit x2TicsChanged();
    }  // end of Graph::setX2Tics

    void Graph::setYTics(const std::map<qreal, QString>& tics, const bool b) {
      this->setTics(this->yAxis, tics);
      if (b) {
        this->replot();
      }
      emit yTicsChanged();
    }  // end of Graph::setYTics

    void Graph::setY2Tics(const std::map<qreal, QString>& tics, const bool b) {
      this->setTics(this->y2Axis, tics);
      if (b) {
        this->replot();
      }
      emit y2TicsChanged();
    }  // end of Graph::setY2Tics

    void Graph::unsetTics(Graph::Axis& axis) {
      axis.userDefinedTics = false;
      this->computeTics(axis);
    }  // end of Graph::unsetXTics

    void Graph::unsetXTics(const bool b) {
      this->unsetTics(this->xAxis);
      if (b) {
        this->replot();
      }
      emit xTicsUnSet();
    }  // end of Graph::unsetXTics

    void Graph::unsetX2Tics(const bool b) {
      this->unsetTics(this->x2Axis);
      if (b) {
        this->replot();
      }
      emit x2TicsUnSet();
    }  // end of Graph::unsetX2Tics

    void Graph::unsetYTics(const bool b) {
      this->unsetTics(this->yAxis);
      if (b) {
        this->replot();
      }
      emit yTicsUnSet();
    }  // end of Graph::unsetXTics

    void Graph::unsetY2Tics(const bool b) {
      this->unsetTics(this->y2Axis);
      if (b) {
        this->replot();
      }
      emit y2TicsUnSet();
    }  // end of Graph::unsetY2Tics

    void Graph::updateCurve(CurveHandler& c, const Graph::Axis& axis) {
      if (axis.scale == Graph::Axis::LOGSCALE) {
        this->computePoints(c, std::pow(10., axis.min),
                            std::pow(10., axis.max));
      } else {
        this->computePoints(c, axis.min, axis.max);
      }
    }  // end of Graph::updateCurve

    void Graph::updateCurves(const Graph::Axis& axis) {
      for (auto& c : this->curves) {
        if (c.axis & axis.id) {
          this->updateCurve(c, axis);
        }
      }
    }  // end of Graph::updateCurves

    bool Graph::computeMinMax(Graph::Axis& axis) {
      return this->computeMinMax(axis.min, axis.max, axis);
    }  // end of Graph::computeMinMax

    bool Graph::computeMinMax(qreal& fv0, qreal& fv1, Graph::Axis& axis) {
      qreal v0 = std::numeric_limits<qreal>::max();
      qreal v1 = -std::numeric_limits<qreal>::max();
      const bool abscissa =
          (axis.id & Graph::xaxis) || (axis.id & Graph::x2axis);
      bool found = false;
      for (const auto& c : this->curves) {
        if (c.axis & axis.id) {
          if (!c.points.empty()) {
            found = true;
            for (const auto& pt : c.points) {
              if (abscissa) {
                if (pt.x < v0) {
                  v0 = pt.x;
                }
                if (pt.x > v1) {
                  v1 = pt.x;
                }
              } else {
                if (pt.y < v0) {
                  v0 = pt.y;
                }
                if (pt.y > v1) {
                  v1 = pt.y;
                }
              }
            }
          }
        }
      }
      if (found) {
        fv0 = v0;
        fv1 = v1;
      }
      return found;
    }  // end of Graph::computeMinMax

    void Graph::updateRange(Graph::Axis& axis) {
      bool found;
      if (!axis.userDefinedRange) {
        qreal y0;
        qreal y1;
        found = this->computeMinMax(y0, y1, axis);
        if (found) {
          this->setRange(axis.min, axis.max, y0, y1, axis.scale);
          if (!axis.userDefinedTics) {
            this->computeTics(axis);
          }
        } else {
          if (axis.scale == Graph::Axis::LOGSCALE) {
            this->setRange(axis.min, axis.max, Graph::defaultLogScaleMinValue,
                           Graph::defaultLogScaleMaxValue, axis.scale);
          } else {
            this->setRange(axis.min, axis.max,
                           Graph::defaultLinearScaleMinValue,
                           Graph::defaultLinearScaleMaxValue, axis.scale);
          }
          if (!axis.userDefinedTics) {
            this->computeTics(axis);
          }
        }
      }
    }  // end of Graph::updateRange

    void Graph::updateRange(Graph::Axis& axis, const qreal x0, const qreal x1) {
      this->setRange(axis.min, axis.max, x0, x1, axis.scale);
      if (!axis.userDefinedTics) {
        this->computeTics(axis);
      }
      this->updateCurves(axis);
      this->updateRange(this->yAxis);
      this->updateRange(this->y2Axis);
    }  // end of Graph::updateXRange

    void Graph::setRange(qreal& rmin,
                         qreal& rmax,
                         const qreal r0,
                         const qreal r1,
                         const Graph::Axis::AxisScale s) {
      using namespace std;
      if (s == Graph::Axis::LOGSCALE) {
        if ((abs(r1 - r0) < 100 * abs(r1) * numeric_limits<qreal>::min()) ||
            (abs(r1 - r0) < 100 * numeric_limits<qreal>::min())) {
          rmin = Graph::defaultLogScaleMinValue;
          rmax = Graph::defaultLogScaleMaxValue;
        } else {
          rmin = r0;
          rmax = r1;
        }
      } else {
        if ((abs(r1 - r0) < 100 * abs(r1) * numeric_limits<qreal>::min()) ||
            (abs(r1 - r0) < 100 * numeric_limits<qreal>::min())) {
          if (abs(r1) < 100 * numeric_limits<qreal>::min()) {
            // ok drawing (almost) zero
            rmin = Graph::defaultLinearScaleMinValue;
            rmax = Graph::defaultLinearScaleMaxValue;
          } else {
            rmin = r1 - abs(r1) * 0.1;
            rmax = r1 + abs(r1) * 0.1;
          }
        } else {
          rmin = r0;
          rmax = r1;
        }
      }
    }  // end of Graph::setRange

    void Graph::setRange(Graph::Axis& axis,
                         const qreal y0,
                         const qreal y1,
                         const bool b) {
      axis.userDefinedRange = b;
      this->setRange(axis.min, axis.max, y0, y1, axis.scale);
      if (!axis.userDefinedTics) {
        this->computeTics(axis);
      }
    }  // end of Graph::setRange

    void Graph::setYRange(const qreal y0, const qreal y1, const bool b) {
      if (this->yAxis.scale == Graph::Axis::LOGSCALE) {
        this->setRange(this->yAxis, Graph::log10(y0), Graph::log10(y1));
      } else {
        this->setRange(this->yAxis, y0, y1);
      }
      if (b) {
        this->replot();
      }
      emit yRangeChanged(y0, y1);
    }  // end of Graph::setYRange

    void Graph::setY2Range(const qreal y0, const qreal y1, const bool b) {
      if (this->y2Axis.scale == Graph::Axis::LOGSCALE) {
        this->setRange(this->y2Axis, Graph::log10(y0), Graph::log10(y1));
      } else {
        this->setRange(this->y2Axis, y0, y1);
      }
      if (b) {
        this->replot();
      }
      emit y2RangeChanged(y0, y1);
    }  // end of Graph::setY2Range

    void Graph::addLabel(const QString& label,
                         const GraphCoordinates xc,
                         const GraphCoordinates yc,
                         const bool b) {
      this->labels.push_back(std::shared_ptr<Label>(new Label(label, xc, yc)));
      if (b) {
        this->replot();
      }
      emit labelAdded(label);
    }  // end of Graph::addLabel

    void Graph::addLabel(const QString& name,
                         const QString& label,
                         const GraphCoordinates xc,
                         const GraphCoordinates yc,
                         const bool b) {
      auto p = this->findLabel(name);
      if (p == this->labels.end()) {
        this->labels.push_back(std::make_shared<Label>(name, label, xc, yc));
      } else {
        *p = std::make_shared<Label>(name, label, xc, yc);
      }
      if (b) {
        this->replot();
      }
      emit labelAdded(name, label);
    }  // end of Graph::addLabel

    void Graph::addArrow(const GraphCoordinates x0,
                         const GraphCoordinates y0,
                         const GraphCoordinates x1,
                         const GraphCoordinates y1,
                         const ArrowStyle s,
                         const bool b) {
      this->arrows.push_back(std::make_shared<Arrow>(x0, y0, x1, y1, s));
      if (b) {
        this->replot();
      }
      emit arrowAdded();
    }  // end of Graph::addArrow

    void Graph::addArrow(const QString& name,
                         const GraphCoordinates x0,
                         const GraphCoordinates y0,
                         const GraphCoordinates x1,
                         const GraphCoordinates y1,
                         const ArrowStyle s,
                         const bool b) {
      auto p = this->findArrow(name);
      if (p == this->arrows.end()) {
        this->arrows.push_back(
            std::make_shared<Arrow>(name, x0, y0, x1, y1, s));
      } else {
        *p = std::make_shared<Arrow>(name, x0, y0, x1, y1, s);
      }
      if (b) {
        this->replot();
      }
      emit arrowAdded(name);
    }  // end of Graph::addArrow

    std::vector<std::shared_ptr<Graph::Label>>::iterator Graph::findLabel(
        const QString& n) {
      return std::find_if(
          this->labels.begin(), this->labels.end(),
          [&n](const std::shared_ptr<Label>& p) { return p->name == n; });
    }  // end of Graph::findLabel

    std::vector<std::shared_ptr<Graph::Arrow>>::iterator Graph::findArrow(
        const QString& n) {
      for (auto p = this->arrows.begin(); p != this->arrows.end(); ++p) {
        if ((*p)->name == n) {
          return p;
        }
      }
      return this->arrows.end();
    }  // end of Graph::findArrow

    void Graph::removeLabel(const QString& n, const bool b) {
      auto p = this->findLabel(n);
      if (p == this->labels.end()) {
        throw(
            std::runtime_error("Graph::removeLabel: "
                               "unknown label '" +
                               n.toStdString() + "'"));
      }
      this->labels.erase(p);
      if (b) {
        this->replot();
      }
      emit labelRemoved(n);
    }  // end of Graph::removeLabels()

    void Graph::removeLabels(const bool b) {
      this->labels.clear();
      if (b) {
        this->replot();
      }
      emit labelsRemoved();
    }  // end of Graph::removeLabels()

    void Graph::removeArrow(const QString& n, const bool b) {
      auto p = this->findArrow(n);
      if (p == this->arrows.end()) {
        throw(
            std::runtime_error("Graph::removeArrow: "
                               "unknown arrow '" +
                               n.toStdString() + "'"));
      }
      this->arrows.erase(p);
      if (b) {
        this->replot();
      }
      emit arrowRemoved(n);
    }  // end of Graph::removeArrows()

    void Graph::removeArrows(const bool b) {
      this->arrows.clear();
      if (b) {
        this->replot();
      }
      emit arrowsRemoved();
    }  // end of Graph::removeArrows()

    void Graph::showGrid(const bool b) {
      this->showGraphGrid = true;
      if (b) {
        this->replot();
      }
      emit gridChanged();
    }  // end of Graph::showGrid

    void Graph::showBorder(const bool b) {
      this->showGraphBorder = true;
      if (b) {
        this->replot();
      }
    }  // end of Graph::showBorder

    void Graph::hideBorder(const bool b) {
      this->showGraphBorder = false;
      if (b) {
        this->replot();
      }
    }  // end of Graph::hideBorder

    void Graph::toggleGrid(const bool b) {
      if (this->showGraphGrid) {
        this->hideGrid(false);
      } else {
        this->showGrid(false);
      }
      if (b) {
        this->replot();
      }
      emit gridChanged();
    }  // end of Graph::toggleGrid

    void Graph::hideGrid(const bool b) {
      this->showGraphGrid = false;
      if (b) {
        this->replot();
      }
      emit gridChanged();
    }  // end of Graph::hideGrid

    bool Graph::hasGrid() const {
      return this->showGraphGrid;
    }  // end of Graph::showGrid

    qreal Graph::getXMin() const {
      return this->xAxis.min;
    }  // end of Graph::getXMin

    qreal Graph::getXMax() const {
      return this->xAxis.max;
    }  // end of Graph::getXMax

    qreal Graph::getYMin() const {
      return this->yAxis.min;
    }  // end of Graph::getYMin

    qreal Graph::getYMax() const {
      return this->yAxis.max;
    }  // end of Graph::getYMax

    qreal Graph::getX2Min() const {
      return this->x2Axis.min;
    }  // end of Graph::getX2Min

    qreal Graph::getX2Max() const {
      return this->x2Axis.max;
    }  // end of Graph::getX2Max

    qreal Graph::getY2Min() const {
      return this->y2Axis.min;
    }  // end of Graph::getY2Min

    qreal Graph::getY2Max() const {
      return this->y2Axis.max;
    }  // end of Graph::getY2Max

    std::shared_ptr<GraphTheme> Graph::getTheme() {
      return this->theme;
    }  // end of Graph::getTheme

    std::shared_ptr<GraphTheme> Graph::getTheme() const {
      return this->theme;
    }  // end of Graph::getTheme

    void Graph::setKeyHorizontalPosition(const KeyHorizontalPosition& p,
                                         const bool b) {
      this->keyHorizontalPosition = p;
      if (b) {
        this->replot();
      }
      emit keyHorizontalPositionChanged(p);
    }  // end of Graph::setKeyHorizontalPosition

    void Graph::setKeyVerticalPosition(const KeyVerticalPosition& p,
                                       const bool b) {
      this->keyVerticalPosition = p;
      if (b) {
        this->replot();
      }
      emit keyVerticalPositionChanged(p);
    }  // end of Graph::setKeyVerticalPosition

    void Graph::setKeyPositions(const KeyHorizontalPosition& hp,
                                const KeyVerticalPosition& vp,
                                const bool b) {
      this->setKeyHorizontalPosition(hp, false);
      this->setKeyVerticalPosition(vp, false);
      if (b) {
        this->replot();
      }
      emit keyPositionsChanged(hp, vp);
    }  // end of Graph::setKeyPositions

    void Graph::setKeyAlignment(const KeyAlignment& a, const bool b) {
      this->keyAlignment = a;
      if (b) {
        this->replot();
      }
      emit keyAlignmentChanged(a);
    }  // end of Graph::setKeyAlignment

    void Graph::mousePressEvent(QMouseEvent* ev) {
      QGraphicsView::mousePressEvent(ev);
      if (!ev->isAccepted()) {
        if (ev->button() == Qt::LeftButton) {
          QPointF pev = this->mapToScene(ev->pos());
          this->rubberBandRect = QRect(pev.x(), pev.y(), 0., 0.);
          this->setRenderHints(this->defaultRenderHints);
          this->showRubberBand = true;
          ev->accept();
        }
      }
    }

    void Graph::mouseMoveEvent(QMouseEvent* ev) {
      QPointF pev = this->mapToScene(ev->pos());
      const qreal xc = pev.x();
      const qreal yc = pev.y();
      QString plabel;
      GraphLayout l;
      this->computeGraphLayout(this->width, this->height, l);
      const qreal xh0 = l.bl;                 // left border
      const qreal xh1 = this->width - l.br;   // right border
      const qreal yh0 = l.bu;                 // up border
      const qreal yh1 = this->height - l.bd;  // down border
      if ((xc >= xh0) && (xc <= xh1) && (yc >= yh0) && (yc <= yh1)) {
        if (this->xAxis.scale == Graph::Axis::LOGSCALE) {
          plabel = "(" + QString::number(pow(10, l.ax * xc + l.bx)) + ",";
        } else {
          plabel = "(" + QString::number(l.ax * xc + l.bx) + ",";
        }
        if (this->yAxis.scale == Graph::Axis::LOGSCALE) {
          plabel += QString::number(pow(10, (yc - l.by) / l.ay)) + ")";
        } else {
          plabel += QString::number((yc - l.by) / l.ay) + ")";
        }
        if (this->cursorPosition != nullptr) {
          this->scene->removeItem(this->cursorPosition);
          delete this->cursorPosition;
          this->cursorPosition = nullptr;
        }
      }
      if (this->showRubberBand) {
        this->rubberBandRect.setWidth(xc - this->rubberBandRect.x());
        this->rubberBandRect.setHeight(yc - this->rubberBandRect.y());
        this->replot();
      }
      if (!plabel.isEmpty()) {
        this->cursorPosition = this->scene->addText(plabel);
        const QRectF b = this->cursorPosition->boundingRect();
        this->cursorPosition->setPos(0, this->height - b.height());
      }
      ev->accept();
    }

    void Graph::mouseReleaseEvent(QMouseEvent* ev) {
      if (ev->button() == Qt::LeftButton) {
        if (this->showRubberBand) {
          QPointF pev = this->mapToScene(ev->pos());
          this->rubberBandRect.setWidth(pev.x() - this->rubberBandRect.x());
          this->rubberBandRect.setHeight(pev.y() - this->rubberBandRect.y());
          QRectF r = this->rubberBandRect.normalized();
          if ((r.width() > 0.05 * (this->width)) &&
              (r.width() > 0.05 * (this->height))) {
            qreal x0 = r.x();
            qreal y0 = r.y();
            qreal x1 = r.x() + r.width();
            qreal y1 = r.y() + r.height();
            GraphLayout l;
            GraphSize s;
            this->computeGraphLayout(this->width, this->height, l);
            s.width = this->width;
            s.height = this->height;
            s.xh0 = l.bl;             // left border
            s.xh1 = s.width - l.br;   // right border
            s.yh0 = l.bu;             // up border
            s.yh1 = s.height - l.bd;  // down border
            x0 = qMax(x0, s.xh0);
            y0 = qMax(y0, s.yh0);
            x1 = qMin(x1, s.xh1);
            y1 = qMin(y1, s.yh1);
            qreal nx0;
            qreal nx1;
            qreal ny0;
            qreal ny1;
            if (this->xAxis.scale == Graph::Axis::LOGSCALE) {
              nx0 = pow(10, l.ax * x0 + l.bx);
              nx1 = pow(10, l.ax * x1 + l.bx);
            } else {
              nx0 = l.ax * x0 + l.bx;
              nx1 = l.ax * x1 + l.bx;
            }
            if (this->yAxis.scale == Graph::Axis::LOGSCALE) {
              ny0 = pow(10, (y1 - l.by) / l.ay);
              ny1 = pow(10, (y0 - l.by) / l.ay);
            } else {
              ny0 = (y1 - l.by) / l.ay;
              ny1 = (y0 - l.by) / l.ay;
            }
            this->setXRange(nx0, nx1, false);
            this->setYRange(ny0, ny1, false);
          }
          this->showRubberBand = false;
          this->setRenderHints(this->highQualityRenderHints);
          this->replot();
          ev->accept();
        }
      }
    }

    void Graph::replot() {
      this->plot(*(this->scene), this->width, this->height);
      emit updated();
    }

    void Graph::resizeEvent(QResizeEvent* ev) {
      const QSize s = ev->size();
      this->width = s.width();
      this->height = s.height();
      this->plot(*(this->scene), this->width, this->height);
    }

    std::vector<Graph::CurveHandler>::iterator Graph::findCurveHandler(
        Curve* const c) {
      return std::find_if(
          this->curves.begin(), this->curves.end(),
          [c](const CurveHandler& ch) { return ch.curve.get() == c; });
    }  // end of Graph::findCurveHandler

    void Graph::updatedCurve(Curve* c) {
      auto p = this->findCurveHandler(c);
      if (p == this->curves.end()) {
        return;
      }
      auto& h = *p;
      auto& a1 = h.axis & Graph::xaxis ? this->xAxis : this->x2Axis;
      auto& a2 = h.axis & Graph::yaxis ? this->yAxis : this->y2Axis;
      if (!a1.userDefinedRange) {
        if (h.curve->hasRange()) {
          qreal x0 = h.curve->minRange();
          qreal x1 = h.curve->maxRange();
          if (a1.scale == Graph::Axis::LOGSCALE) {
            x0 = Graph::log10(x0);
            x1 = Graph::log10(x1);
          }
          this->computeRange2(x0, x1, a1.id);
          this->updateRange(a1, x0, x1);
        }
      }
      if (!a2.userDefinedRange) {
        this->updateRange(a2);
      }
      this->replot();
    }  // end of Graph::updatedCurve

    void Graph::removeCurve(Curve* c, const bool b) {
      auto p = this->findCurveHandler(c);
      if (p == this->curves.end()) {
        return;
      }
      auto& h = *p;
      auto& a1 = h.axis & Graph::xaxis ? this->xAxis : this->x2Axis;
      auto& a2 = h.axis & Graph::yaxis ? this->yAxis : this->y2Axis;
      this->curves.erase(p);
      if (!a1.userDefinedRange) {
        qreal x0 = Graph::defaultLinearScaleMinValue;
        qreal x1 = Graph::defaultLinearScaleMaxValue;
        if (a1.scale == Graph::Axis::LOGSCALE) {
          x0 = Graph::log10(x0);
          x1 = Graph::log10(x1);
        }
        this->computeRange2(x0, x1, a1.id);
        this->updateRange(a1, x0, x1);
      }
      if (!a2.userDefinedRange) {
        this->updateRange(a2);
      }
      if (b) {
        this->replot();
      }
      emit curveRemoved(c);
    }  // end of Graph::removeCurve

    QGraphicsScene& Graph::getScene() { return *(this->scene); }

    qreal Graph::getGraphWidth() const { return this->width; }

    qreal Graph::getGraphHeight() const { return this->height; }

    const QString& Graph::getUpperTitle() const { return this->uTitle; }

    const QString& Graph::getDownTitle() const { return this->dTitle; }

    const QString& Graph::getLeftTitle() const { return this->lTitle; }

    const QString& Graph::getRightTitle() const { return this->rTitle; }

    const QString& Graph::getXLabel() const { return this->xAxis.label; }

    const QString& Graph::getX2Label() const { return this->x2Axis.label; }

    const QString& Graph::getYLabel() const { return this->yAxis.label; }

    const QString& Graph::getY2Label() const { return this->y2Axis.label; }

    void Graph::configureGraphRange(const GraphBorder&) {
      if (!this->gcd->isVisible()) {
        this->gcd->show();
      }
    }  // end of Graph::configureGraphRange

    void Graph::configureGraphRanges() {
      if (!this->gcd->isVisible()) {
        this->gcd->show();
      }
    }  // end of Graph::configureGraphRanges

    GraphConfigurationDialog& Graph::getConfigurationDialog() {
      return *(this->gcd);
    }  // end of Graph::getConfigurationDialog

  }  // end of namespace plot

}  // end of namespace tfel
