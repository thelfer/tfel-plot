/*! 
 * \file  ThemeSelectionAction.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 29 juin 2012
 */

#ifndef _LIB_TFEL_PLOT_THEMESELECTIONACTION_H_
#define _LIB_TFEL_PLOT_THEMESELECTIONACTION_H_ 

#ifdef TFEL_QT4
#include<QtGui/QAction>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QAction>
#endif /* TFEL_QT5 */
#include<TFEL/Plot/Config.hxx>

namespace tfel
{

  namespace plot
  {

    class Graph;

    /*!
     * class handling curve configuration
     */
    class TFELPLOT_VISIBILITY_EXPORT ThemeSelectionAction
      : public QAction
    {
      Q_OBJECT

    public:

      ThemeSelectionAction(const QString&,
			   Graph&,
			   QWidget *const);
      
    public slots:

      virtual void
      setTheme();

    private:

      Graph& g;

      const QString name;

    }; // end of CurveConfigurationAction

  } // end of namespace plot
  
} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_THEMESELECTIONACTION_H */

