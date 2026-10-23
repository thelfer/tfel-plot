/*!
 * \file   GetInstallPath.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   01 mars 2017
 */

#include <stdexcept>
#include <algorithm>
#include <cstdlib>

#if defined _WIN32 || defined _WIN64
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "GetInstallPath-Prefix.hxx"
#include "TFEL/Raise.hxx"
#include "TFEL/Plot/Config/GetInstallPath.hxx"

namespace tfel {

  namespace plot {

#if defined _WIN32 || defined _WIN64
    static bool getValueInRegistry(std::string& value) {
      using namespace std;
      HKEY hKey;
      char szBuffer[512];
      DWORD dwBufferSize = sizeof(szBuffer);
      DWORD dwType = REG_NONE;
      LONG nError;
      LONG lRes = RegOpenKeyEx(HKEY_CLASSES_ROOT, L"TFELPLOTHOME-" VERSION, 0,
                               KEY_READ, &hKey);
      if (ERROR_SUCCESS != lRes) {
        return false;
      }
      nError =
          RegQueryValueEx(hKey, L"", nullptr, &dwType,
                          reinterpret_cast<LPBYTE>(szBuffer), &dwBufferSize);
      RegCloseKey(hKey);
      if (ERROR_SUCCESS == nError) {
        if (dwType != REG_SZ) {
          return false;
        }
        // RegQueryValueEx does not guarantee the value to be
        // NUL terminated (for instance if it exactly fills the
        // buffer): terminate it explicitly before use.
        const DWORD last = static_cast<DWORD>(sizeof(szBuffer) - 1u);
        szBuffer[std::min(dwBufferSize, last)] = '\0';
        value = szBuffer;
        return true;
      }
      return false;
    }
#endif

    static std::string handleSpace(const std::string& p) {
      if (std::find(p.begin(), p.end(), ' ') != p.end()) {
#if defined _WIN32 || defined _WIN64
        tfel::raise(
            "tfel-plot-config handleSpace: "
            "path to TFEL shall not contain space as "
            "MinGW can't handle it (Found '" +
            p +
            "'). "
            "Please change TFEL installation directory");
#else
        return '"' + p + '"';
#endif
      }
      return p;
    }

    std::string getInstallPath() {
#if defined _WIN32 || defined _WIN64
      // check in the registry (installation through NSIS)
      std::string rpath;
      if (getValueInRegistry(rpath)) {
        auto path = handleSpace(rpath);
        if (!path.empty()) {
          return path;
        }
      }
#endif
      const auto path = ::getenv("TFELPLOTHOME");
      if (path != nullptr) {
        return handleSpace(path);
      }
#if defined _WIN32 || defined _WIN64
      return "";
#else
      return PREFIXDIR;
#endif
    }  // end of getInstallPath

  }  // end of namespace plot

}  // end of namespace tfel
