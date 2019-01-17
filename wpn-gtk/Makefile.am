SUBDIRS = .

ACLOCAL_AMFLAGS = -I m4

#	Binaries
bin_PROGRAMS = \
	wpn-gtk

commoncppflags = -O2 -I/usr/local/include
commoncppflags += -I/usr/local/opt/openssl@1.1/include

commonlibs = -L. -L/usr/local/opt/openssl@1.1/lib
commonlibs += -lgtkmm-2.4 -latkmm-1.6 -lgtk-x11-2.0 -lgdkmm-2.4 -lgiomm-2.4 -lpangomm-1.4 -lglibmm-2.4 -lcairomm-1.0 -lsigc-2.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype

AM_CXXFLAGS = -std=c++11

nobase_dist_include_HEADERS = \
	wpn-gtk.h \
	top-window.h

common_src =

#
#	wpn
#
wpn_gtk_SOURCES= \
	wpn-gtk.cpp \
	top-window.cpp \
	$(common_src)

wpn_gtk_LDADD = $(commonlibs) 
wpn_gtk_CPPFLAGS = $(commoncppflags) `pkg-config gtkmm-2.4 --cflags`

#
#	Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
	LICENSE COPYING CODE_OF_CONDUCT.md CONTRIBUTING.md HISTORY README.md TODO \
	autogen.sh
