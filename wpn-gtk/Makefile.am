SUBDIRS = .

ACLOCAL_AMFLAGS = -I m4

#	Binaries
bin_PROGRAMS = \
	wpn-gtk

commoncppflags = -O2
commoncppflags += -I/usr/local/include -I/usr/local/opt/openssl@1.1/include -I ../../wpn

commonlibs = -L. -L/usr/local/lib -L/usr/local/opt/openssl@1.1/lib -lssl -lcrypto -lcurl -lece

AM_CXXFLAGS = -std=c++11

nobase_dist_include_HEADERS = \
	top-window.h wpn-app.h client-env.h

common_src =

#
#	wpn-gtk
#
wpn_gtk_SOURCES = \
	wpn-gtk.cpp wpn-app.cpp \
	top-window.cpp client-env.cpp \
	$(common_src)

wpn_gtk_LDADD = -lgtkmm-3.0 -latkmm-1.6 -lgdkmm-3.0 -lgiomm-2.4 -lpangomm-1.4 -lglibmm-2.4 -lgtk-3 -lgdk-3 \
	-lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lgio-2.0 -lcairomm-1.0 -lcairo -lsigc-2.0 -lgdk_pixbuf-2.0 -lgobject-2.0 -lglib-2.0 \
	-L../../wpn -lprotobuf-lite \
	$(commonlibs)

wpn_gtk_CPPFLAGS = $(commoncppflags) `pkg-config gtkmm-3.0 --cflags`

#
#	Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
	LICENSE COPYING CODE_OF_CONDUCT.md CONTRIBUTING.md HISTORY README.md TODO \
	autogen.sh
