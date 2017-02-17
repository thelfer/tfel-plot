/*! 
 * \file  CurveConfigurationDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 mai 2012
 */

#ifdef TFEL_QT4
#include<QtGui/QPushButton>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QPushButton>
#endif /* TFEL_QT5 */

#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/CurveConfigurationDialog.hxx"

namespace tfel
{

  namespace plot
  {
    
    CurveConfigurationDialog::CurveConfigurationDialog(Curve   *const c,
						       QWidget *const p)
      : CurveConfigurationDialogBase(p),
	curve(c)
    {
      this->createCurveColorButton();
      this->setLayout(this->cl);
      this->setAttribute(Qt::WA_DeleteOnClose);
      QObject::connect(this->curve,SIGNAL(destroyed()),
		       this,SLOT(close()));
      // signals and default values
      if(this->curve->hasKey()){
	this->ke->setText(this->curve->getKey());
      }
      QObject::connect(this->ke,SIGNAL(textChanged(const QString&)),
		       this,SLOT(keyChanged(const QString&)));
      QObject::connect(this->curve,SIGNAL(keyChanged(const QString&)),
		       this,SLOT(curveKeyChanged(const QString&)));
      QObject::connect(this->cd,SIGNAL(currentColorChanged(const QColor&)),
		       this,SLOT(newColorSelected(const QColor&)));
      QObject::connect(this->curve,SIGNAL(colorChanged(const QColor&)),
		       this,SLOT(curveColorChanged(const QColor&)));
      if(this->curve->hasSpecifiedColor()){
	this->hadSpecifiedColor = true;
	this->previousCurveColor = this->curve->getColor();
      } else {
	this->hadSpecifiedColor = false;
      }
      this->scb->setCurrentIndex(this->curve->getStyle());
      this->ws->setValue(this->curve->getWidth());
      QObject::connect(this->curve,SIGNAL(styleChanged(int)),
		       this,SLOT(curveStyleChanged(int)));
      QObject::connect(this->scb,SIGNAL(currentIndexChanged(int)),
		       this,SLOT(styleChanged(int)));
      QObject::connect(this->ws,SIGNAL(valueChanged(int)),
		       this,SLOT(widthChanged(int)));
      QObject::connect(this->curve,SIGNAL(widthChanged(int)),
		       this->ws,SLOT(setValue(int)));
    }

    void CurveConfigurationDialog::keyChanged(const QString& k)
    {
      QObject::disconnect(this->curve,SIGNAL(keyChanged(const QString&)),
			  this,SLOT(curveKeyChanged(const QString&)));
      this->curve->setKey(k,true);
      QObject::connect(this->curve,SIGNAL(keyChanged(const QString&)),
		       this,SLOT(curveKeyChanged(const QString&)));
    }

    void
    CurveConfigurationDialog::curveKeyChanged(const QString& k)
    {
      this->ke->setText(k);
    } // end of CurveConfigurationDialog::curveKeyChanged

    void
    CurveConfigurationDialog::styleChanged(int i)
    {
      QObject::disconnect(this->curve,SIGNAL(styleChanged(int)),
			  this,SLOT(curveStyleChanged(int)));
      this->curve->setStyle(Curve::intToCurveStyle(i),true);
      QObject::connect(this->curve,SIGNAL(styleChanged(int)),
		       this,SLOT(curveStyleChanged(int)));
    }

    void CurveConfigurationDialog::curveStyleChanged(int)
    {
      this->scb->setCurrentIndex(this->curve->getStyle());
    } // end of CurveConfigurationDialog::curveStyleChanged

    void CurveConfigurationDialog::widthChanged(int w)
    {
      QObject::disconnect(this->curve,SIGNAL(widthChanged(int)),
			  this->ws,SLOT(setValue(int)));
      this->curve->setWidth(w,true);
      QObject::connect(this->curve,SIGNAL(widthChanged(int)),
		       this->ws,SLOT(setValue(int)));
    } // end of CurveConfigurationDialog::widthChanged

    void CurveConfigurationDialog::chooseCurveColor()
    {
      if(this->hadSpecifiedColor){
	this->cd->setCurrentColor(this->previousCurveColor);
      }
      QObject::connect(this->cd,SIGNAL(currentColorChanged(const QColor&)),
		       this,SLOT(newColorSelected(const QColor&)));
      if(cd->exec()==QDialog::Accepted){
	QObject::disconnect(this->curve,SIGNAL(colorChanged(const QColor&)),
			    this,SLOT(curveColorChanged(const QColor&)));
	QColor nc = cd->selectedColor();
	this->curve->setColor(nc,true);
	this->hadSpecifiedColor  = true;
	this->previousCurveColor = nc;
	QObject::connect(this->curve,SIGNAL(colorChanged(const QColor&)),
			 this,SLOT(curveColorChanged(const QColor&)));
      } else {
	QObject::disconnect(this->curve,SIGNAL(colorChanged(const QColor&)),
			    this,SLOT(curveColorChanged(const QColor&)));
	if(this->hadSpecifiedColor){
	  this->curve->setColor(this->previousCurveColor,true);
	} else {
	  this->curve->unsetColor(true);
	}
	this->createCurveColorButton();
	QObject::connect(this->curve,SIGNAL(colorChanged(const QColor&)),
			 this,SLOT(curveColorChanged(const QColor&)));
      }
      QObject::disconnect(this->cd,SIGNAL(currentColorChanged(const QColor&)),
			  this,SLOT(newColorSelected(const QColor&)));
    }

    void
    CurveConfigurationDialog::curveColorChanged(const QColor& c)
    {
      if(this->curve->hasSpecifiedColor()){
	this->hadSpecifiedColor = true;
	this->previousCurveColor = this->curve->getColor();
      } else {
	this->hadSpecifiedColor = false;
      }
      this->cd->setCurrentColor(c);
      this->createCurveColorButton();
    }

    void
    CurveConfigurationDialog::newColorSelected(const QColor& c)
    {
      QObject::disconnect(this->curve,SIGNAL(colorChanged(const QColor&)),
			  this,SLOT(curveColorChanged(const QColor&)));
      this->curve->setColor(c,true);
      this->createCurveColorButton();
      QObject::connect(this->curve,SIGNAL(colorChanged(const QColor&)),
		       this,SLOT(curveColorChanged(const QColor&)));
    }
    
    void
    CurveConfigurationDialog::createCurveColorButton()
    {
      auto *cb = new QPushButton("Choose color");
      QObject::connect(cb,SIGNAL(clicked()),
		       this,SLOT(chooseCurveColor()));
      if(this->curve->hasSpecifiedColor()){
	cb->setIcon(createColorToolButtonIcon(this->curve->getColor()));
      } else {
	cb->setIcon(createColorToolButtonIcon(Qt::white));
      }
      this->cl->addWidget(cb,1,1);
    }

  } // end of namespace plot
  
} // end of namespace tfel

