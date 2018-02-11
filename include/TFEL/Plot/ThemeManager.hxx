/*!
 * \file   ThemeManager.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   12 fév 2008
 */

#ifndef LIB_TFEL_PLOT_THEMEMANAGER_HXX_
#define LIB_TFEL_PLOT_THEMEMANAGER_HXX_

#include <QtCore/QMap>
#include <QtCore/QString>

#include "TFEL/Plot/Config.hxx"
#include "TFEL/Plot/GraphTheme.hxx"

namespace tfel {

  namespace plot {

    struct TFELPLOT_VISIBILITY_EXPORT ThemeProxy {
      typedef std::shared_ptr<GraphTheme> ThemePtr;
      virtual QString getName() const = 0;
      virtual ThemePtr getTheme() const = 0;
      virtual ~ThemeProxy();
    };  // end of struct ThemeProxy

    template <typename T>
    struct StandardThemeProxy : public ThemeProxy {
      StandardThemeProxy(const QString&, const bool = true);
      QString getName() const override;
      ThemeProxy::ThemePtr getTheme() const override;
      ~StandardThemeProxy() override;

     private:
      const QString name;
    };  // end of struct StandardThemeProxy

    struct TFELPLOT_VISIBILITY_EXPORT ThemeManager {
      //! a simple alias
      using ThemePtr = std::shared_ptr<GraphTheme>;

      static ThemeManager& getThemeManager();

      void loadLibrary(const QString&);

      ThemePtr getTheme(const QString&) const;

      void addTheme(const ThemeProxy* const);

      QList<QString> getAvailableThemesNames() const;

     private:
      TFEL_VISIBILITY_LOCAL ThemeManager();
      TFEL_VISIBILITY_LOCAL ~ThemeManager();

      ThemeManager(ThemeManager&&) = delete;
      ThemeManager(const ThemeManager&) = delete;
      ThemeManager& operator=(ThemeManager&&) = delete;
      ThemeManager& operator=(const ThemeManager&) = delete;

      QMap<QString, std::shared_ptr<const ThemeProxy>> proxies;

    };  // end of struct ThemeManager

  }  // end of namespace plot

}  // end of namespace tfel

#include "TFEL/Plot/ThemeManager.ixx"

#endif /* LIB_TFEL_PLOT_THEMEMANAGER_HXX */
