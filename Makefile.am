ACLOCAL_AMFLAGS = -I m4
SUBDIRS = cmake include src tplot tests

DISTCHECK_CONFIGURE_FLAGS = --with-tfel=$(TFELHOME)
if TFEL_QT4
DISTCHECK_CONFIGURE_FLAGS += --enable-qt4
endif
if TFEL_QT5
DISTCHECK_CONFIGURE_FLAGS += --enable-qt5
endif

EXTRA_DIST=CMakeLists.txt
