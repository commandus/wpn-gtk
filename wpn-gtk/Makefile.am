SUBDIRS = .

ACLOCAL_AMFLAGS = -I m4

#	Binaries
bin_PROGRAMS = \
	wpn-gtk

commoncppflags = -O2 -I/usr/local/include
commoncppflags += -I/usr/local/opt/openssl@1.1/include

commonlibs = -L. -L/usr/local/opt/openssl@1.1/lib

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
wpn_gtk_CPPFLAGS = $(commoncppflags)

#
#	Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
	LICENSE COPYING CODE_OF_CONDUCT.md CONTRIBUTING.md HISTORY README.md TODO \
	autogen.sh
