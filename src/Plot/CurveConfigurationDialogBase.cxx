/*!
 * \file  CurveConfigurationDialogBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 juin 2012
 */

#include <QtCore/QtDebug>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QColorDialog>

#include "TFEL/Plot/Graph.hxx"
#include "TFEL/Plot/CurveConfigurationDialogBase.hxx"

namespace tfel {

  namespace plot {

    QIcon CurveConfigurationDialogBase::createColorToolButtonIcon(
        const QColor& color) {
      QPixmap pixmap(50, 80);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      painter.setRenderHints(QPainter::Antialiasing |
                             QPainter::SmoothPixmapTransform);
      painter.fillRect(QRect(0, 20, 50, 50), color);
      return QIcon(pixmap);
    }

    CurveConfigurationDialogBase::CurveConfigurationDialogBase(QWidget* const p)
        : QDialog(p), cd(new QColorDialog(this)) {
      this->cl = new QGridLayout();
      // key editing
      this->cl->addWidget(new QLabel(tr("Curve key")), 0, 0);
      this->ke = new QLineEdit;
      this->cl->addWidget(ke, 0, 1);
      // color editing
      this->cl->addWidget(new QLabel(tr("Curve color")), 1, 0);
      this->createColorButton();
      // style editing
      this->cl->addWidget(new QLabel(tr("Pen style")), 2, 0);
      this->scb = Curve::getStyleComboBox();
      // width editing
      this->cl->addWidget(new QLabel(tr("Pen width")), 3, 0);
      this->ws = new QSlider(Qt::Horizontal);
      this->ws->setRange(1, 30);
      this->cl->addWidget(this->ws, 3, 1);
      // layout
      this->cl->addWidget(this->scb, 2, 1);
      // this->cl->addWidget(new QPushButton(QIcon::fromTheme("edit-delete"),
      // 					  QObject::tr("Remove"),this),4,1);
    }  // end of CurveConfigurationDialogBase::CurveConfigurationDialogBase()

    void CurveConfigurationDialogBase::createColorButton(const QColor& c) {
      auto* cb = new QPushButton("Choose color");
      QObject::connect(cb, &QPushButton::clicked, this,
                       &CurveConfigurationDialogBase::chooseCurveColor);
      cb->setIcon(createColorToolButtonIcon(c));
      this->cl->addWidget(cb, 1, 1);
    }

    void CurveConfigurationDialogBase::chooseCurveColor() {
      if (this->cd->exec() == QDialog::Accepted) {
        this->createColorButton(cd->selectedColor());
      }
    }

  }  // end of namespace plot

}  // end of namespace tfel
