/*!
 * \file  ThemeSelectionAction.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 29 juin 2012
 */

#include "TFEL/Plot/Graph.hxx"
#include "TFEL/Plot/ThemeManager.hxx"
#include "TFEL/Plot/ThemeSelectionAction.hxx"

namespace tfel {

  namespace plot {

    ThemeSelectionAction::ThemeSelectionAction(const QString& n,
                                               Graph& g_,
                                               QWidget* const p)
        : QAction(n, p), g(g_), name(n) {
      QObject::connect(this, &ThemeSelectionAction::triggered, this,
                       &ThemeSelectionAction::setTheme);
    }  // end of ThemeSelectionAction:ThemeSelectionAction

    void ThemeSelectionAction::setTheme() {
      try {
        auto& tm = ThemeManager::getThemeManager();
        this->g.setTheme(tm.getTheme(this->name), true);
      } catch (...) {
      }
    }  // end of ThemeSelectionAction::setTheme

  }  // end of namespace plot

}  // end of namespace tfel
