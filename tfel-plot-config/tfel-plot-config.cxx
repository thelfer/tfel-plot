/*!
 * \file   src/TFELPlotConfig/tfel-config.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   27 août 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<map>
#include<string>
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<stdexcept>

#include"TFEL/Config/TFELConfig.hxx"
#include"tfel-plot-config.hxx"

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#endif

using FuncPtr = void (*)();
using CallBacksContainer = std::map<std::string,std::pair<FuncPtr,std::string>>;

static std::string
handleSpace(const std::string& p)
{
  if(find(p.begin(),p.end(),' ')!=p.end()){
#if defined _WIN32 || defined _WIN64
    throw(std::runtime_error("tfel-config handleSpace: "
			     "path to tfel-plot shall not contain space as "
			     "MinGW can't handle it (Found '"+p+"'). "
			     "Please change tfel-plot installation directory"));
#else
    return '"'+p+'"';
#endif
  }
  return p;
}

static std::string
getTFELPLOTHOME();

static std::string libDir();

static std::string includeDir();

static void
registerCallBack(const std::string&,
		 const FuncPtr&,
		 const std::string&);

static void treatIncludes();

static void treatLibs();

TFEL_NORETURN static void
treatUnknownOption(const std::string&);

TFEL_NORETURN static void
treatHelp();

TFEL_NORETURN static void treatLicences();

static CallBacksContainer callBacks;
static bool incs            = false;
static bool libs            = false;

#if defined _WIN32 || defined _WIN64
static bool
getValueInRegistry(std::string &value)
{
  using namespace std;
  HKEY  hKey;
  char  szBuffer[512];
  DWORD dwBufferSize = sizeof(szBuffer);
  LONG  nError;
  LONG  lRes = RegOpenKeyEx(HKEY_CLASSES_ROOT,"TFELPLOTHOME-" VERSION,0,KEY_READ,&hKey);
  if(ERROR_SUCCESS != lRes){
    return false;
  }
  nError = RegQueryValueEx(hKey,"",nullptr,nullptr,
			   reinterpret_cast<LPBYTE>(szBuffer),
			   &dwBufferSize);
  RegCloseKey(hKey);
  if (ERROR_SUCCESS == nError){
    value = szBuffer;
    return true;
  }
  return false;
}
#endif

static std::string getTFELPLOTHOME()
{
#if defined _WIN32 || defined _WIN64
  // check in the registry (installation through NSIS)
  std::string rpath;
  if(getValueInRegistry(rpath)){
    return handleSpace(rpath);
  }
#endif

  const char * const path = getenv("TFELPLOTHOME");
  if(path!=nullptr){
    return handleSpace(path);
  }

#if defined _WIN32 || defined _WIN64
  throw(std::runtime_error("tfel-config getTFELPLOTHOME: "
			   "no TFELPLOTHOME registry key "
			   "defined and no TFEHOME "
			   "environment variable defined"));
#else
  return "";
#endif
}

static std::string libDir()
{
  using namespace std;
  const string prefix(PREFIXDIR);
  const string execPrefix(EXECPREFIXDIR);
  string lib(LIBDIR);
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
  const string ldir("/bin");
#else 
#ifdef LIB_SUFFIX
  const string ldir("/lib"LIB_SUFFIX);
#else
  const string ldir("/lib");
#endif
#endif
  const auto& th = getTFELPLOTHOME();
  if(!th.empty()){
    return th+ldir;
  }
  if(lib.substr(0,14)=="${exec_prefix}"){
    if(execPrefix=="${prefix}"){
      lib = prefix+ldir;
    } else {
      lib = execPrefix+ldir;
    }
  }
  return handleSpace(lib);
} // end of libDir

static std::string includeDir()
{
  const std::string prefix(PREFIXDIR);
  std::string inc(INCLUDEDIR);
  const auto& th = getTFELPLOTHOME();
  if(!th.empty()){
    return th+"/include";
  } else {
    if(inc.substr(0,9)=="${prefix}"){
      inc = handleSpace(prefix + "/include");
    }
  }
  return inc;
} // end of libDir

static void
registerCallBack(const std::string& key,
		 const FuncPtr& f,
		 const std::string& description)
{
  callBacks.insert({key,{f,description}});
} // end of registerNewCallBack


static void treatIncludes()
{
  incs = true;
} // end of treatIncludes

static void treatLibs()
{
  libs = true;
} // end of treatLibs

static void listOptions(std::ostream& os)
{
  os << "Valid options are:\n";
  for(const auto& c : callBacks){ 
    std::string options(c.first);
    if(options.size()<=18){
      options.insert(options.size(),18-options.size(),' ');
    }
    os << options << " : " <<  c.second.second << '\n';
  }
} // end of listOptions

TFEL_NORETURN static void treatVersion()
{
  std::cout << "tfel-plot-config " << VERSION << '\n';
  exit(EXIT_SUCCESS);
} // end of treatHelp

TFEL_NORETURN static void treatHelp()
{
  std::cout << "usage : tfel-config [options]\n";
  listOptions(std::cout);
  std::exit(EXIT_SUCCESS);
} // end of treatHelp

TFEL_NORETURN static void treatUnknownOption(const std::string& o)
{
  std::cerr << "unknown option " << o << '\n';
  listOptions(std::cerr);
  std::exit(EXIT_FAILURE);
} // end of treatUnknownOption

TFEL_NORETURN static void treatLicences()
{
  using namespace std;
  cout << "Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights reserved." << endl;
  cout << "This project is publicly released under either the GNU GPL Licence or the" << endl;
  cout << "CECILL-A licence. A copy of thoses licences are delivered with the sources of" << endl;
  cout << "TFEL. CEA or EDF may also distribute this project under specific licensing" << endl;
  cout << "conditions." << endl;
  cout << endl;
  cout << "For further information, please read the licences files or visit the following" << endl;
  cout << "websites:" << endl;
  cout << "    http://www.gnu.org/licenses" << endl;
  cout << "    http://www.cecill.info/licences.en.html" << endl;
  cout << "    http://www.cecill.info/licences.fr.html" << endl;
  exit(EXIT_SUCCESS);
} // end of treatLicences

/* coverity [UNCAUGHT_EXCEPT]*/
int main(const int argc,
	 const char *const *const argv)
{
  using namespace std;

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
  try{
#endif /* __CYGWIN__ */

  registerCallBack("--includes",&treatIncludes,"return tfel include path.");
  registerCallBack("--libs",&treatLibs,"return linking flags.");
  registerCallBack("--help",&treatHelp,"print this help message.");
  registerCallBack("--version",&treatVersion,"print tfel version and svn revision.");
  registerCallBack("--licence",&treatLicences,"print tfel licences.");
  for(auto p2=argv+1;p2!=argv+argc;++p2){
    auto p = callBacks.find(*p2);
    if(p==callBacks.end()){
      treatUnknownOption(*p2);
    }
    (*(p->second.first))();
  }

  if(incs){
    std::cout << "-I" << includeDir() << " ";
  }

  if(libs){
    std::cout << "-L" << libDir() << " ";
  }

  if(incs||libs){
    std::cout << '\n';
  }
  
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
  }
  catch(exception& e){
    MessageBox(nullptr,e.what(),
	       "mfront",0);
    return EXIT_FAILURE;
  }
  catch(...){
    return EXIT_FAILURE;
  }
#endif /* __CYGWIN__ */

  return EXIT_SUCCESS;
}
