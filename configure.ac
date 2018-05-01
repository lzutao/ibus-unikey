m4_define([package_name], [ibus-unikey])
m4_define([ibus_unikey_version], [0.6.1])

AC_PREREQ([2.69])
AC_INIT([package_name], [ibus_unikey_version], [http://code.google.com/p/ibus-unikey], [package_name])
AC_CONFIG_AUX_DIR([build-aux])
dnl AC_GNU_SOURCE # Obsolete, use AC_USE_SYSTEM_EXTENSIONS, but neccessary ?
AC_USE_SYSTEM_EXTENSIONS

AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])
AM_PROG_AR
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])


AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AS_VERSION
AS_NANO

AM_SANITY_CHECK
AM_MAINTAINER_MODE
AC_ISC_POSIX

# Turn off shared libraries during beta-testing, since they
# make the build process take too long.
LT_PREREQ([2.4])
LT_INIT([disable-shared])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([fcntl.h libintl.h locale.h memory.h stddef.h stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS([memmove memset setlocale strcasecmp strchr strrchr strstr])

# More https://developer.gnome.org/gtkmm-tutorial/stable/sec-internationalization-intro.html.en
IT_PROG_INTLTOOL([0.50.2])

GETTEXT_PACKAGE="$PACKAGE_NAME"
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",[Gettext package])

AM_GNU_GETTEXT_VERSION([0.19.8])
AM_GNU_GETTEXT([external])


AC_ARG_WITH(gtk-version,
    AS_HELP_STRING([--with-gtk-version[=VERSION]],
                   [Choose GTK version to build with. VERSION=2 or 3 [default=2]]),
    gtk_version=$with_gtk_version
)
AS_IF([test "x$gtk_version" = "x3" || test "x$gtk_version" = "xgtk-3" || test "x$gtk_version" = "xgtk3"],
    gtk_version="gtk+-3.0",
    gtk_version="gtk+-2.0 >= 2.24",
)

PKG_CHECK_MODULES(IBUS, [
    ibus-1.0 >= 1.4.0
])

PKG_CHECK_MODULES(X11, [
    x11
])
AC_SUBST(X11)

PKG_CHECK_MODULES(GTK,
    $gtk_version
)
AC_SUBST(GTK)

# OUTPUT files
AC_CONFIG_FILES([Makefile src/unikey.xml:src/unikey.xml.in po/Makefile.in ibus-unikey.spec])

AC_OUTPUT

AC_MSG_RESULT([
Build options:
    Version         $VERSION
    Install prefix  $prefix
    GTK Version     $gtk_version
])

