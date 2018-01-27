/*! 
 * \file  CurveConfigurationDialogBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 juin 2012
 */

#ifndef LIB_TFEL_PLOT_CURVECONFIGURATIONDIALOGBASE_H_
#define LIB_TFEL_PLOT_CURVECONFIGURATIONDIALOGBASE_H_ 

#include<QtWidgets/QDialog>
#include<QtWidgets/QLineEdit>
#include<QtWidgets/QComboBox>
#include<QtWidgets/QGridLayout>
#include<QtWidgets/QColorDialog>
#include"TFEL/Plot/Config.hxx"

namespace tfel
{

  namespace plot
  {

    /*!
     * base class used to choose :
     * 
     */
    class TFELPLOT_VISIBILITY_EXPORT CurveConfigurationDialogBase
      : public QDialog
    {
      
      Q_OBJECT

    protected slots:

      virtual void
      chooseCurveColor();

    public:

      static QIcon
      createColorToolButtonIcon(const QColor&);
	      
    protected:
      
      CurveConfigurationDialogBase(QWidget *const);

      virtual void
      createColorButton(const QColor& = Qt::white);
      
      QComboBox    *scb;
      QSlider      *ws;
      QColorDialog *cd;
      QLineEdit    *ke;
      QGridLayout  *cl;

    }; // end of CurveConfigurationDialogBase
    
  } // end of namespace plot
  
} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_CURVECONFIGURATIONDIALOGBASE_H */
