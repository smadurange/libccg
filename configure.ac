AC_PREREQ([2.71])
AC_INIT([libccg],[0.0.1],[smadurange@users.noreply.github.com])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SRCDIR([src/mem.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AM_PROG_AR

PKG_CHECK_MODULES([CHECK], [check >= 0.15.2])

LT_INIT

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CONFIG_FILES([Makefile])
AC_OUTPUT