/*!
 * \file   ThemeManager.ixx
 * \brief
 *
 * \author Helfer Thomas
 * \date   12 fév 2008
 */

#ifndef LIB_TFEL_PLOT_THEMEMANAGER_IXX_
#define LIB_TFEL_PLOT_THEMEMANAGER_IXX_

namespace tfel {

  namespace plot {

    template <typename T>
    StandardThemeProxy<T>::StandardThemeProxy(const QString& n, const bool b)
        : name(n) {
      if (b) {
        ThemeManager::getThemeManager().addTheme(
            new StandardThemeProxy(n, false));
      }
    }  // end of StandardThemeProxy<T>::StandardThemeProxy

    template <typename T>
    QString StandardThemeProxy<T>::getName() const {
      return this->name;
    }  // end of StandardThemeProxy<T>::getName() const

    template <typename T>
    ThemeProxy::ThemePtr StandardThemeProxy<T>::getTheme() const {
      return ThemeProxy::ThemePtr(new T());
    }  // end of StandardThemeProxy<T>::getTheme

    template <typename T>
    StandardThemeProxy<T>::~StandardThemeProxy() {
    }  // end of StandardThemeProxy::~StandardThemeProxy

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_THEMEMANAGER_IXX */
