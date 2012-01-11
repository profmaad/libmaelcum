AC_PREREQ([2.68])
AC_INIT([libmaelcum], [0.0], [himself@prof-maad.org])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP

# Libtool
LT_INIT

# Checks for libraries.
PKG_CHECK_MODULES([NETTTLE], [nettle >= 2.4])
PKG_CHECK_MODULES([HOGWEED], [hogweed >= 2.4])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile
		 maelcum/Makefile])

AC_OUTPUT
