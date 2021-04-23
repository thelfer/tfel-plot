/*!
 * \file   ThemeManager.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   12 fév 2008
 */

#include <stdexcept>
#include <utility>
#include "TFEL/Raise.hxx"
#include "TFEL/System/ExternalLibraryManager.hxx"
#include "TFEL/Plot/ThemeManager.hxx"
#include "TFEL/Plot/StandardGraphTheme.hxx"

namespace tfel {

  namespace plot {

    ThemeProxy::~ThemeProxy() = default;

    void ThemeManager::loadLibrary(const QString& lib) {
      using namespace tfel::system;
      auto& lm = ExternalLibraryManager::getExternalLibraryManager();
      lm.loadLibrary(lib.toStdString());
    }

    ThemeManager::ThemePtr ThemeManager::getTheme(const QString& n) const {
      const auto p = this->proxies.find(n);
      if (p == this->proxies.end()) {
        tfel::raise(
            "ThemeManager::getTheme: "
            "no theme named '" +
            n.toStdString() + "' registred.");
      }
      return p->second->getTheme();
    }  // end of ThemeManager::getTheme

    void ThemeManager::addTheme(const ThemeProxy* const proxy) {
      const auto& name = proxy->getName();
      const auto p = this->proxies.find(name);
      if (p != this->proxies.end()) {
        tfel::raise(
            "ThemeManager::getTheme: a theme named "
            "'" +
            name.toStdString() + "' has already been registred.");
      }
      this->proxies.insert({name, std::shared_ptr<const ThemeProxy>(proxy)});
    }  // end of ThemeManager::getTheme

    QStringList ThemeManager::getAvailableThemesNames() const {
      QStringList names;
      for (const auto& p : this->proxies) {
        names.append(p.first);
      }
      return names;
    }  // end of ThemeManager::getAvailableThemesNames() const

    ThemeManager& ThemeManager::getThemeManager() {
      static ThemeManager themeManager;
      return themeManager;
    }  // end of ThemeManager::~ThemeManager()

    ThemeManager::ThemeManager() {
      this->addTheme(new StandardThemeProxy<StandardGraphTheme>(
          "StandardGraphTheme", false));
    }  // end of ThemeManager::ThemeManager()

    ThemeManager::~ThemeManager() = default;

  }  // end of namespace plot

}  // end of namespace tfel
