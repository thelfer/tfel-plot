/*! 
 * \file  CurveConfigurationAction.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 juin 2012
 */

#ifndef LIB_TFEL_PLOT_CURVECONFIGURATIONACTION_H_
#define LIB_TFEL_PLOT_CURVECONFIGURATIONACTION_H_ 

#include<QtWidgets/QAction>
#include<TFEL/Plot/Config.hxx>

namespace tfel
{

  namespace plot
  {

    //! forward declaration
    class Curve;

    /*!
     * class handling curve configuration
     */
    class TFELPLOT_VISIBILITY_EXPORT CurveConfigurationAction
      : public QAction
    {
      Q_OBJECT

    public:

      CurveConfigurationAction(Curve *const,
			       const QString&,
			       QWidget *const);
      
    public slots:

      virtual void
      showCurveConfigurationDialog();

    protected slots:

      virtual void
      updateIcon(const QColor&);

    private:

      Curve *const curve;

    }; // end of CurveConfigurationAction

  } // end of namespace plot
  
} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_CURVECONFIGURATIONACTION_H */

