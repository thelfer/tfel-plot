/*! 
 * \file  CurveConfigurationDialog.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 mai 2012
 */

#ifndef LIB_TFEL_PLOT_CURVECONFIGURATIONDIALOG_H_
#define LIB_TFEL_PLOT_CURVECONFIGURATIONDIALOG_H_ 

#include<TFEL/Plot/Config.hxx>
#include"TFEL/Plot/CurveConfigurationDialogBase.hxx"

namespace tfel
{

  namespace plot
  {

    //! forward declaration
    class Curve;
    
    /*!
     * class handling curve configuration
     */
    class TFELPLOT_VISIBILITY_EXPORT CurveConfigurationDialog
      : public CurveConfigurationDialogBase
    {
      Q_OBJECT
    public:
      CurveConfigurationDialog(Curve *const,
			       QWidget *const);
    protected slots:
      virtual void curveKeyChanged(const QString&);
      virtual void keyChanged(const QString&);
      virtual void curveColorChanged(const QColor&);
      virtual void newColorSelected(const QColor&);
      virtual void chooseCurveColor() override;
      virtual void curveStyleChanged(int);
      virtual void styleChanged(int);
      virtual void widthChanged(int);
    protected:
      virtual void createCurveColorButton();
    private:
      bool   hadSpecifiedColor;
      QColor previousCurveColor;
      Curve *const curve;
    }; // end of CurveConfigurationDialog
    
  } // end of namespace plot
  
} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_CURVECONFIGURATIONDIALOG_H */

