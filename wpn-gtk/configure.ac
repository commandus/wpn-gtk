AC_PREREQ([2.63])
AC_INIT([wpn-gtk], [0.2], [andrei.i.ivanov@commandus.com])
AM_INIT_AUTOMAKE([1.11 foreign no-define subdir-objects])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_SRCDIR([wpn-gtk.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_ENABLE_SHARED
AC_PROG_LIBTOOL

AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])

#PKG_CHECK_MODULES(PROTOBUF, protobuf >= 3.1.0)
#AC_SUBST(PROTOBUF_LIBS)
#AC_SUBST(PROTOBUF_CFLAGS)
#AC_SUBST(PROTOBUF_VERSION)

# MacOS: 
#   brew install openssl@1.1
# install openssl into /usr/local/lib/opt/openssl@1.1
LDFLAGS="$LDFLAGS -L/usr/local/lib -L/usr/local/opt/openssl@1.1/lib"

# Checks for header files. Add /usr/local/include for IS X.
CFLAGS="$CFLAGS -I/usr/local/include"
AC_CHECK_HEADERS([fcntl.h limits.h stdint.h string.h sys/time.h termios.h unistd.h])

# Checks for libraries. Add /usr/local/lib for OS X
AC_CHECK_LIB([crypto], [EVP_CIPHER_CTX_reset], [], AC_MSG_ERROR([Could not find openssl >= 1.1.0. Try $ ./configure LDFLAGS='-Lyour-openssl2-lib-path']))

# Checks for typedefs, structures, and compiler characteristics.
# AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([memmove memset])

AC_OUTPUT
