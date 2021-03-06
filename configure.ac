AC_PREREQ([2.69])

dnl initialize autoconf
dnl releases only do -Wall, git and prerelease does -Werror too
dnl use a three digit version number for releases, and four for git/pre
AC_INIT([ibus-unikey],[0.6.2],[https://github.com/vn-input/ibus-unikey],[ibus-unikey])
AC_CONFIG_AUX_DIR([build-aux])
dnl AC_GNU_SOURCE # Obsolete, use AC_USE_SYSTEM_EXTENSIONS, but neccessary ?
AC_USE_SYSTEM_EXTENSIONS

dnl initialize automake
dnl subdir-objects keep the object files in the same structure as the source files
dnl but the main program to be built in the top-level build directory
dnl silent-rules doing nothing at all after automake 1.13
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])
AM_PROG_AR
dnl using silent rules without forcing automake 1.11
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

dnl can autoconf find the source ?
AC_CONFIG_SRCDIR([src/main.cpp])

dnl define the output header for config
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

dnl define PACKAGE_VERSION_* variables
AS_VERSION
dnl check if this is a release version
AS_NANO

dnl AM_MAINTAINER_MODE only provides the option to configure to enable it
AM_MAINTAINER_MODE
AC_SEARCH_LIBS([strerror],[cposix])

dnl *** checks for programs ***

dnl find a compiler
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP

AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_PROG_RANLIB

dnl Turn off shared libraries during beta-testing, since they
dnl make the build process take too long.
dnl LT_PREREQ([2.4])
dnl LT_INIT([disable-shared])
dnl LT_INIT()

dnl ** Checks for header files **
AC_CHECK_HEADERS([fcntl.h libintl.h locale.h memory.h stddef.h stdint.h stdlib.h string.h])

dnl check if we have ANSI C header files
AC_HEADER_STDC

dnl Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

dnl ** Checks for library functions **

AC_CHECK_FUNCS([memmove memset setlocale strcasecmp strchr strrchr strstr])

dnl More https://developer.gnome.org/gtkmm-tutorial/stable/sec-internationalization-intro.html.en
dnl But intltool is obsolete
dnl IT_PROG_INTLTOOL([0.50.2])

GETTEXT_PACKAGE="$PACKAGE_NAME"
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, ["$GETTEXT_PACKAGE"], [Gettext package])

dnl set up gettext
dnl the version check needs to stay here because autopoint greps for it
AM_GNU_GETTEXT_VERSION([0.19.7])
AM_GNU_GETTEXT([external])

AC_ARG_WITH(gtk-version,
    AS_HELP_STRING([--with-gtk-version[=VERSION]],
                   [Choose GTK version to build with. VERSION=2 or 3 [default=2]]),
    gtk_version=$with_gtk_version
)
AS_IF([test "x$gtk_version" = "x3"],
    gtk_version="gtk+-3.0",
    gtk_version="gtk+-2.0 >= 2.24",
)

PKG_CHECK_MODULES([IBUS], [ibus-1.0 >= 1.5.4])

PKG_CHECK_MODULES([GTK],$gtk_version)
AC_SUBST(GTK)

dnl OUTPUT files
AC_CONFIG_FILES([Makefile src/unikey.xml.in po/Makefile.in ibus-unikey.spec])

AC_OUTPUT

AC_MSG_RESULT([
Build options:
    Version         $VERSION
    Install prefix  $prefix
    GTK Version     $gtk_version
])

