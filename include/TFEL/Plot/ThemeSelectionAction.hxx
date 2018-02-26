/*!
 * \file  ThemeSelectionAction.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 29 juin 2012
 */

#ifndef LIB_TFEL_PLOT_THEMESELECTIONACTION_H_
#define LIB_TFEL_PLOT_THEMESELECTIONACTION_H_

#include <QtWidgets/QAction>
#include <TFEL/Plot/Config.hxx>

namespace tfel {

  namespace plot {

    struct Graph;

    /*!
     * class handling curve configuration
     */
    struct TFELPLOT_VISIBILITY_EXPORT ThemeSelectionAction : QAction {
      ThemeSelectionAction(const QString&, Graph&, QWidget* const);

     public slots:

      virtual void setTheme();

     private:
      Graph& g;

      const QString name;

      Q_OBJECT
    };  // end of CurveConfigurationAction

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_THEMESELECTIONACTION_H */
