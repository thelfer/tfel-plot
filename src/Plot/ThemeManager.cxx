/*!
 * \file   ThemeManager.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 fév 2008
 */

#include<stdexcept>
#include<utility>

#include"TFEL/System/ExternalLibraryManager.hxx"

#include"TFEL/Plot/ThemeManager.hxx"
#include"TFEL/Plot/StandardGraphTheme.hxx"

namespace tfel
{

  namespace plot
  {

    ThemeProxy::~ThemeProxy() = default;
      
    void ThemeManager::loadLibrary(const QString& lib)
    {
      using namespace tfel::system;
      auto& lm = ExternalLibraryManager::getExternalLibraryManager();
      lm.loadLibrary(lib.toStdString());
    }
      
    const ThemeManager::ThemePtr
    ThemeManager::getTheme(const QString& n) const
    {
      auto p = this->proxies.find(n);
      if(p==this->proxies.end()){
	throw(std::runtime_error("ThemeManager::getTheme: "
				 "no theme named '"+n.toStdString()+
				 "' registred."));
      }
      return (*p)->getTheme();
    } // end of ThemeManager::getTheme

    void
    ThemeManager::addTheme(const ThemeProxy* const proxy)
    {
      using namespace std;
      const QString& name = proxy->getName();
      QMap<QString,shared_ptr<const ThemeProxy> >::const_iterator p;
      p = this->proxies.find(name);
      if(p!=this->proxies.end()){
	string msg("ThemeManager::getTheme : ");
	msg += "a theme named "+name.toStdString()+" has already been registred.";
	throw(runtime_error(msg));
      }
      this->proxies.insert(name,shared_ptr<const ThemeProxy>(proxy));
    } // end of ThemeManager::getTheme

    QList<QString>
    ThemeManager::getAvailableThemesNames() const
    {
      return this->proxies.keys();
    } // end of ThemeManager::getAvailableThemesNames() const

    ThemeManager&
    ThemeManager::getThemeManager()
    {
      static ThemeManager themeManager;
      return themeManager;
    } // end of ThemeManager::~ThemeManager()

    ThemeManager::ThemeManager()
    {
      this->addTheme(new StandardThemeProxy<StandardGraphTheme>("StandardGraphTheme",false));
    } // end of ThemeManager::ThemeManager()

    ThemeManager::~ThemeManager() = default;

  } // end of namespace plot

} // end of namespace tfel

