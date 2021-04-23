/*!
 * \file   Curve.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   04 jan 2008
 */

#include <QtCore/QtDebug>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

#include "TFEL/Plot/Curve.hxx"

namespace tfel {

  namespace plot {

    static QIcon createLineIcon(const Qt::PenStyle s) {
      const unsigned short length = 125u;
      const unsigned short hwidth = 10u;
      QPixmap pixmap(length, 2 * hwidth);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      QPainterPath path;
      painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform);
      path.moveTo(0., hwidth);
      path.lineTo(length, hwidth);
      painter.setPen(QPen(QColor(Qt::black), 3, s));
      painter.drawPath(path);
      return QIcon(pixmap);
    }  // end of createLineIcon

    static QIcon createDotIcon() {
      const unsigned short length = 100u;
      const unsigned short hwidth = 10u;
      QPixmap pixmap(length, 2 * hwidth);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      QPainterPath path;
      path.addEllipse(0.5 * length - 0.5 * hwidth, 0.5 * hwidth, hwidth,
                      hwidth);
      // paint
      painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform);
      painter.setPen(QPen(QColor(Qt::black), 3));
      painter.setBrush(QBrush(QColor(Qt::black)));
      painter.drawPath(path);
      return QIcon(pixmap);
    }

    static QIcon createSquareIcon() {
      const unsigned short length = 100u;
      const unsigned short hwidth = 10u;
      QPixmap pixmap(length, 2 * hwidth);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      QPainterPath path;
      path.addRect(0.5 * length - 0.5 * hwidth, 0.5 * hwidth, hwidth, hwidth);
      // paint
      painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform);
      painter.setPen(QPen(QColor(Qt::black), 3));
      painter.setBrush(QBrush(QColor(Qt::black)));
      painter.drawPath(path);
      return QIcon(pixmap);
    }

    static QIcon createTriangleIcon() {
      const unsigned short length = 100u;
      const unsigned short hwidth = 10u;
      QPixmap pixmap(length, 2 * hwidth);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      QPainterPath path;
      path.moveTo(0.5 * length, 0.3 * hwidth);
      path.lineTo(0.5 * length + 0.6 * hwidth, 1.5 * hwidth);
      path.lineTo(0.5 * length - 0.6 * hwidth, 1.5 * hwidth);
      path.lineTo(0.5 * length, 0.3 * hwidth);
      // paint
      painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform);
      painter.setPen(QPen(QColor(Qt::black), 3));
      painter.setBrush(QBrush(QColor(Qt::black)));
      painter.drawPath(path);
      return QIcon(pixmap);
    }

    static QIcon createCrossIcon() {
      const unsigned short length = 100u;
      const unsigned short hwidth = 10u;
      QPixmap pixmap(length, 2 * hwidth);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      QPainterPath path;
      path.moveTo(0.5 * length - 0.6 * hwidth, 0.3 * hwidth);
      path.lineTo(0.5 * length + 0.6 * hwidth, 1.5 * hwidth);
      path.moveTo(0.5 * length - 0.6 * hwidth, 1.5 * hwidth);
      path.lineTo(0.5 * length + 0.6 * hwidth, 0.3 * hwidth);
      // paint
      painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform);
      painter.setPen(QPen(QColor(Qt::black), 3));
      painter.setBrush(QBrush(QColor(Qt::black)));
      painter.drawPath(path);
      return QIcon(pixmap);
    }

    static QIcon createDiamondIcon() {
      const unsigned short length = 100u;
      const unsigned short hwidth = 10u;
      QPixmap pixmap(length, 2 * hwidth);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      QPainterPath path;
      path.moveTo(0.5 * length, 0.3 * hwidth);
      path.lineTo(0.5 * length + 0.6 * hwidth, hwidth);
      path.lineTo(0.5 * length, 1.5 * hwidth);
      path.lineTo(0.5 * length - 0.6 * hwidth, hwidth);
      path.lineTo(0.5 * length, 0.3 * hwidth);
      // paint
      painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform);
      painter.setPen(QPen(QColor(Qt::black), 3));
      painter.setBrush(QBrush(QColor(Qt::black)));
      painter.drawPath(path);
      return QIcon(pixmap);
    }

    static QIcon createLinePlusIcon() {
      const unsigned short length = 100u;
      const unsigned short hwidth = 10u;
      QPixmap pixmap(length, 2 * hwidth);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      QPainterPath path;
      painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform);
      path.moveTo(0., hwidth);
      path.lineTo(length, hwidth);
      path.moveTo(0.5 * length, 0.3 * hwidth);
      path.lineTo(0.5 * length, 1.5 * hwidth);
      painter.setPen(QPen(QColor(Qt::black), 3));
      painter.drawPath(path);
      return QIcon(pixmap);
    }  // end of createLinePlusIcon

    static QIcon createPlusIcon() {
      const unsigned short length = 100u;
      const unsigned short hwidth = 10u;
      QPixmap pixmap(length, 2 * hwidth);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      QPainterPath path;
      painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform);
      path.moveTo(0.5 * length - 0.5 * hwidth, hwidth);
      path.lineTo(0.5 * length + 0.5 * hwidth, hwidth);
      path.moveTo(0.5 * length, 0.5 * hwidth);
      path.lineTo(0.5 * length, 1.5 * hwidth);
      painter.setPen(QPen(QColor(Qt::black), 3));
      painter.drawPath(path);
      return QIcon(pixmap);
    }  // end of createPlusIcon

    QComboBox* Curve::getStyleComboBox() {
      auto* scb = new QComboBox();
      scb->setIconSize(QSize(100, 20));
      scb->addItem(createLineIcon(Qt::SolidLine), "SolidLine");
      scb->addItem(createLineIcon(Qt::DotLine), "DotLine");
      scb->addItem(createLineIcon(Qt::DashLine), "DashLine");
      scb->addItem(createLineIcon(Qt::DashDotLine), "DashDotLine");
      scb->addItem(createLineIcon(Qt::DashDotDotLine), "DashDotDotLine");
      scb->addItem(createLinePlusIcon(), "LinePlus");
      scb->addItem(createDotIcon(), "Dot");
      scb->addItem(createSquareIcon(), "Square");
      scb->addItem(createTriangleIcon(), "Triangle");
      scb->addItem(createCrossIcon(), "Cross");
      scb->addItem(createDiamondIcon(), "Diamond");
      scb->addItem(createPlusIcon(), "Plus");
      return scb;
    }  // end of Curve::getStyleComboBox()

    int Curve::curveStyleToInt(const Curve::Style s) { return s; }

    Curve::Style Curve::intToCurveStyle(const int i) {
      Curve::Style s;
      switch (i) {
        case 0u:
          s = Curve::SOLIDLINE;
          break;
        case 1u:
          s = Curve::DOTLINE;
          break;
        case 2u:
          s = Curve::DASHLINE;
          break;
        case 3u:
          s = Curve::DASHDOTLINE;
          break;
        case 4u:
          s = Curve::DASHDOTDOTLINE;
          break;
        case 5u:
          s = Curve::LINEPLUS;
          break;
        case 6u:
          s = Curve::DOT;
          break;
        case 7u:
          s = Curve::SQUARE;
          break;
        case 8u:
          s = Curve::TRIANGLE;
          break;
        case 9u:
          s = Curve::CROSS;
          break;
        case 10u:
          s = Curve::DIAMOND;
          break;
        case 11u:
          s = Curve::PLUS;
          break;
        default:
          s = Curve::SOLIDLINE;
          qDebug() << "CurveConfigurationDialog::styleChanged : "
                   << tr("unsupported line style '%1'").arg(i);
      }
      return s;
    }  // end of Curve::intToCurveStyle

    Curve::~Curve() {}  // end of Curve::Curve()

  }  // end of namespace plot

}  // end of namespace tfel
