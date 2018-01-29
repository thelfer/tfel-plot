/*! 
 * \file  CurveConfigurationDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 mai 2012
 */

#include<QtWidgets/QPushButton>

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
      QObject::connect(this->curve,&Curve::destroyed,
		       this,&CurveConfigurationDialog::close);
      // signals and default values
      if(this->curve->hasKey()){
	this->ke->setText(this->curve->getKey());
      }
      QObject::connect(this->ke,&QLineEdit::textChanged,
		       this,&CurveConfigurationDialog::keyChanged);
      QObject::connect(this->curve,&Curve::keyChanged,
		       this,&CurveConfigurationDialog::curveKeyChanged);
      QObject::connect(this->cd,&QColorDialog::currentColorChanged,
		       this,&CurveConfigurationDialog::newColorSelected);
      QObject::connect(this->curve,&Curve::colorChanged,
		       this,&CurveConfigurationDialog::curveColorChanged);
      if(this->curve->hasSpecifiedColor()){
	this->hadSpecifiedColor = true;
	this->previousCurveColor = this->curve->getColor();
      } else {
	this->hadSpecifiedColor = false;
      }
      this->scb->setCurrentIndex(this->curve->getStyle());
      this->ws->setValue(this->curve->getWidth());
      QObject::connect(this->curve,&Curve::styleChanged,
		       this,&CurveConfigurationDialog::curveStyleChanged);
      QObject::connect(this->scb,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		       this,&CurveConfigurationDialog::styleChanged);
      QObject::connect(this->ws,&QSlider::valueChanged,
		       this,&CurveConfigurationDialog::widthChanged);
      QObject::connect(this->curve,&Curve::widthChanged,
		       this->ws,&QSlider::setValue);
    }

    void CurveConfigurationDialog::keyChanged(const QString& k)
    {
      QObject::disconnect(this->curve,&Curve::keyChanged,
			  this,&CurveConfigurationDialog::curveKeyChanged);
      this->curve->setKey(k,true);
      QObject::connect(this->curve,&Curve::keyChanged,
		       this,&CurveConfigurationDialog::curveKeyChanged);
    }

    void CurveConfigurationDialog::curveKeyChanged(const QString& k)
    {
      this->ke->setText(k);
    } // end of CurveConfigurationDialog::curveKeyChanged

    void CurveConfigurationDialog::styleChanged(int i)
    {
      QObject::disconnect(this->curve,&Curve::styleChanged,
			  this,&CurveConfigurationDialog::curveStyleChanged);
      this->curve->setStyle(Curve::intToCurveStyle(i),true);
      QObject::connect(this->curve,&Curve::styleChanged,
		       this,&CurveConfigurationDialog::curveStyleChanged);
    }

    void CurveConfigurationDialog::curveStyleChanged(int)
    {
      this->scb->setCurrentIndex(this->curve->getStyle());
    } // end of CurveConfigurationDialog::curveStyleChanged

    void CurveConfigurationDialog::widthChanged(int w)
    {
      QObject::disconnect(this->curve,&Curve::widthChanged,
			  this->ws,&QSlider::setValue);
      this->curve->setWidth(w,true);
      QObject::connect(this->curve,&Curve::widthChanged,
		       this->ws,&QSlider::setValue);
    } // end of CurveConfigurationDialog::widthChanged

    void CurveConfigurationDialog::chooseCurveColor()
    {
      if(this->hadSpecifiedColor){
	this->cd->setCurrentColor(this->previousCurveColor);
      }
      QObject::connect(this->cd,&QColorDialog::currentColorChanged,
		       this,&CurveConfigurationDialog::newColorSelected);
      if(cd->exec()==QDialog::Accepted){
	QObject::disconnect(this->curve,&Curve::colorChanged,
			    this,&CurveConfigurationDialog::curveColorChanged);
	QColor nc = cd->selectedColor();
	this->curve->setColor(nc,true);
	this->hadSpecifiedColor  = true;
	this->previousCurveColor = nc;
	QObject::connect(this->curve,&Curve::colorChanged,
			    this,&CurveConfigurationDialog::curveColorChanged);
      } else {
	QObject::disconnect(this->curve,&Curve::colorChanged,
			    this,&CurveConfigurationDialog::curveColorChanged);
	if(this->hadSpecifiedColor){
	  this->curve->setColor(this->previousCurveColor,true);
	} else {
	  this->curve->unsetColor(true);
	}
	this->createCurveColorButton();
	QObject::connect(this->curve,&Curve::colorChanged,
			 this,&CurveConfigurationDialog::curveColorChanged);
      }
      QObject::disconnect(this->cd,&QColorDialog::currentColorChanged,
			  this,&CurveConfigurationDialog::newColorSelected);
    }

    void CurveConfigurationDialog::curveColorChanged(const QColor& c)
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

    void CurveConfigurationDialog::newColorSelected(const QColor& c)
    {
      QObject::disconnect(this->curve,&Curve::colorChanged,
			  this,&CurveConfigurationDialog::curveColorChanged);
      this->curve->setColor(c,true);
      this->createCurveColorButton();
      QObject::connect(this->curve,&Curve::colorChanged,
		       this,&CurveConfigurationDialog::curveColorChanged);
    }
    
    void CurveConfigurationDialog::createCurveColorButton()
    {
      auto *cb = new QPushButton("Choose color");
      QObject::connect(cb,&QPushButton::clicked,
		       this,&CurveConfigurationDialog::chooseCurveColor);
      if(this->curve->hasSpecifiedColor()){
	cb->setIcon(createColorToolButtonIcon(this->curve->getColor()));
      } else {
	cb->setIcon(createColorToolButtonIcon(Qt::white));
      }
      this->cl->addWidget(cb,1,1);
    }

  } // end of namespace plot
  
} // end of namespace tfel

