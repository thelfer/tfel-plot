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
	this->updateIcon(this->curve->getColor());
      } else  {
	this->updateIcon(Qt::white);
      }
      QObject::connect(this,&CurveConfigurationAction::triggered,
		       this,&CurveConfigurationAction::showCurveConfigurationDialog);
      QObject::connect(this->curve,&Curve::colorChanged,
		       this,&CurveConfigurationAction::updateIcon);
    } // end of CurveConfigurationAction::CurveConfigurationAction

    void CurveConfigurationAction::updateIcon(const QColor& c)
    {
      this->setIcon(CurveConfigurationDialogBase::createColorToolButtonIcon(c));
      this->setIconVisibleInMenu(true);
    } // CurveConfigurationAction::updateIcon

    void CurveConfigurationAction::showCurveConfigurationDialog()
    {
      auto w = qobject_cast<QWidget *>(this->parent());
      auto d = new CurveConfigurationDialog(this->curve,w);
      d->setModal(false);
      d->show();
    } 

  } // end of namespace plot
  
} // end of namespace tfel


