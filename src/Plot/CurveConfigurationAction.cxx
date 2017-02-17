/*! 
 * \file  CurveConfigurationAction.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 juin 2012
 */

#include<QtCore/QDebug>

#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/CurveConfigurationAction.hxx"
#include"TFEL/Plot/CurveConfigurationDialog.hxx"

namespace tfel
{

  namespace plot
  {
    
    CurveConfigurationAction::CurveConfigurationAction(Curve   *const c,
						       const QString& l,
						       QWidget *const p)
      : QAction(l,p),
	curve(c)
    {
      if(this->curve->hasSpecifiedColor()){
	QColor color = this->curve->getColor();
	this->updateIcon(color);
      } else  {
	this->updateIcon(Qt::white);
      }
      QObject::connect(this,SIGNAL(triggered()),
		       this,SLOT(showCurveConfigurationDialog()));
      QObject::connect(this->curve,SIGNAL(colorChanged(const QColor&)),
		       this,SLOT(updateIcon(const QColor&)));
    } // end of CurveConfigurationAction::CurveConfigurationAction

    void
    CurveConfigurationAction::updateIcon(const QColor& c)
    {
      typedef CurveConfigurationDialogBase CCDBase;
      this->setIcon(CCDBase::createColorToolButtonIcon(c));
      this->setIconVisibleInMenu(true);
    } // CurveConfigurationAction::updateIcon

    void
    CurveConfigurationAction::showCurveConfigurationDialog()
    {
      CurveConfigurationDialog *d;
      d = new CurveConfigurationDialog(this->curve,
				       qobject_cast<QWidget *>(this->parent()));
      d->setModal(false);
      d->show();
    } 

  } // end of namespace plot
  
} // end of namespace tfel


