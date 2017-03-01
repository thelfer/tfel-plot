# Process this file with autoconf to produce a configure script.
# Process this file with automake to produce Makefile.in files.

AC_INIT(tfel-plot,1.1,thomas.helfer@cea.fr)
AC_CONFIG_SRCDIR([include/TFEL/Plot/Curve.hxx])
AC_CONFIG_COMMANDS
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])
AC_CANONICAL_TARGET

# Automake specific stuff
AM_INIT_AUTOMAKE([foreign dist-bzip2])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

#disable static library generation by default
AC_ENABLE_STATIC(no)

# disable automatic declaration of
# CXXFLAGS by AC_PROG_CXX
if test "x$CXXFLAGS" = "x";
    then
    CXXFLAGS=""
fi

AC_PROG_CXX

# Libtool specific stuff
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)
AC_SUBST(LIBTOOL_DEPS)

# Compiler specific stuff
AC_ARG_ENABLE([optimizations],
	       AC_HELP_STRING([--disable-optimizations],
		              [Do not try to guess what optimization to enable]))

# Enable debug
AC_ARG_ENABLE([debug],
	AC_HELP_STRING([--enable-debug], [Compile with debug options]))

#disable default flags
CFLAGS="";

#choose between intel icpc and g++ and ekopath
INTEL="no"
PATHSCALE="no"
if test "x$GXX" = "xyes" -o "${CXX##*/}x" = "icpcx" ; then
    if test "${CXX##*/}x" = "icpcx"; then
	GXX="no"
	INTEL="yes"
    fi
fi
if test "x$GXX" = "xyes" -o "${CXX##*/}x" = "pathCCx" ; then
    if test "${CXX##*/}x" = "pathCCx"; then
	GXX="no"
	PATHSCALE="yes"
    fi
fi

# check for compiler
COMPILER_SPECIFIC_OPTIONS="";
COMPILER_WARNINGS="";
OPTIMISATION_FLAGS="-DNO_RUNTIME_CHECK_BOUNDS -DNDEBUG"

# g++ specific treatement
if test "x$GXX" = "xyes"; then
    AC_CHECK_GXX
fi

# intel icc specific treatement
if test "x$INTEL" = "xyes"; then
    AC_CHECK_INTEL
fi

# check for compiler
COMPILER_FLAGS="";
COMPILER_CXXFLAGS="";
COMPILER_WARNINGS="";
OPTIMISATION_FLAGS0=""
OPTIMISATION_FLAGS=""
OPTIMISATION_FLAGS2=""

# g++ specific treatement
if test "x$GXX" == "xyes"; then
    COMPILER_CXXFLAGS="-std=c++11";
    COMPILER_WARNINGS="";
    OPTIMISATION_FLAGS0="-DNO_RUNTIME_CHECK_BOUNDS -DNDEBUG"
    OPTIMISATION_FLAGS=""
    OPTIMISATION_FLAGS2=""
    AC_CHECK_GXX
fi

# intel icc specific treatement
if test "x$INTEL" == "xyes";
then
    COMPILER_FLAGS="";
    COMPILER_CXXFLAGS="-std=c++11";
    COMPILER_WARNINGS="";
    OPTIMISATION_FLAGS0="-DNO_RUNTIME_CHECK_BOUNDS -DNDEBUG"
    OPTIMISATION_FLAGS=""
    OPTIMISATION_FLAGS2=""
    AC_CHECK_INTEL
fi

CFLAGS="$CFLAGS $COMPILER_FLAGS"
CXXFLAGS="$CXXFLAGS $COMPILER_FLAGS"
CXXFLAGS="$CXXFLAGS $COMPILER_CXXFLAGS"
CXXFLAGS="$CXXFLAGS $COMPILER_WARNINGS"
CXXFLAGS="$CXXFLAGS $OPTIMISATION_FLAGS0"

AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_MSG_CHECKING([C++11 noreturn attribute])
noreturn=no
AC_TRY_COMPILE([],
	       [[[noreturn]] void f();],
	       [noreturn=yes],
	       [noreturn=no])
AC_MSG_RESULT($noreturn)
if test "x$noreturn" == "xyes";
then
   COMPILER_CXXFLAGS="$COMPILER_CXXFLAGS -DTFEL_HAVE_NORETURN_ATTRIBUTE";
   CXXFLAGS="$CXXFLAGS -DTFEL_HAVE_NORETURN_ATTRIBUTE";
fi
AC_LANG_RESTORE

echo
echo ---------------------------------------------
echo testing tfel
echo ---------------------------------------------
echo

CHECK_TFEL

## QT

AC_ARG_ENABLE([qt4],
	       AC_HELP_STRING([--enable-qt4],
		              [use the qt4 version of the library]))
AC_ARG_ENABLE([qt5],
	       AC_HELP_STRING([--enable-qt5],
		              [use the qt5 version of the library]))

if test "x$enable_qt4" == "xyes" -a "x$enable_qt5" == "xyes";
then
  AC_MSG_ERROR([one must enable only one Qt version])
fi

if test ! "x$enable_qt4" == "xyes" -a ! "x$enable_qt5" == "xyes";
then
  AC_MSG_ERROR([one must choose one Qt version])
fi

if test "x$enable_qt4" == "xyes";
then
    echo
    echo ---------------------------------------------
    echo testing Qt4
    echo ---------------------------------------------
    echo
    AC_DEFINE(TFEL_QT4,"","")
    AM_CONDITIONAL([TFEL_QT4],[true])
    AM_CONDITIONAL([TFEL_QT5],[false])
    PKG_CHECK_MODULES(QT4, [QtCore QtGui QtSvg QtXml QtNetwork >= 4.4.0], [
			     AC_PATH_PROGS(MOC, [moc-qt4 moc], moc,`eval $PKG_CONFIG --variable=exec_prefix QtCore`/bin)
			     AC_PATH_PROG(RCC, rcc, rcc,`eval $PKG_CONFIG --variable=exec_prefix QtCore`/bin)
			     AC_PATH_PROGS(UIC, [uic-qt4 uic], uic,`eval $PKG_CONFIG --variable=exec_prefix QtCore`/bin)
			 ], [
			     AC_MSG_ERROR([Qt4 library not found])
			 ])
fi

if test "x$enable_qt5" == "xyes";
then
    echo
    echo ---------------------------------------------
    echo testing Qt5
    echo ---------------------------------------------
    echo
    m4_include([m4/m4-ax_have_qt.m4])
    AX_HAVE_QT
    if test "x$have_qt" == "xno"; then
      AC_MSG_ERROR([Qt5 not detected])
    fi
    echo "qt5 C++ flags: $QT_CXXFLAGS"
    echo "qt5 libraries: $QT_LIBS"
    echo "qt5 moc      : $QT_MOC"
    AC_DEFINE(TFEL_QT5,"","")
    AM_CONDITIONAL([TFEL_QT4],[false])
    AM_CONDITIONAL([TFEL_QT5],[true])
fi

AC_CONFIG_FILES([Makefile
		 cmake/Makefile
		 cmake/modules/Makefile
		 include/Makefile
		 src/Makefile
		 src/Plot/Makefile
 		 src/Plot/GetInstallPath-Prefix.hxx
		 tplot/Makefile
		 tplot/src/Makefile
		 tplot/include/Makefile
		 tests/Makefile
		 docs/Makefile
		 docs/tplot/Makefile
		 docs/tplot/gp/Makefile		 
		 ])

AC_OUTPUT

