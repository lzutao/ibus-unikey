# using non-recursive Make
SUBDIRS = . po

ACLOCAL_AMFLAGS = -I m4 --install

MAKEFLAGS = --no-print-directory

noinst_LIBRARIES = libunikey-ibus.a

libexec_PROGRAMS = ibus-engine-unikey ibus-setup-unikey

############
## ukengine/
############

libunikey_ibus_a_SOURCES = \
	ukengine/byteio.cpp \
	ukengine/byteio.h \
	ukengine/charset.cpp \
	ukengine/charset.h \
	ukengine/convert.cpp \
	ukengine/data.cpp \
	ukengine/data.h \
	ukengine/error.cpp \
	ukengine/inputproc.cpp \
	ukengine/inputproc.h \
	ukengine/keycons.h \
	ukengine/mactab.cpp \
	ukengine/mactab.h \
	ukengine/pattern.cpp \
	ukengine/pattern.h \
	ukengine/ukengine.cpp \
	ukengine/ukengine.h \
	ukengine/unikey.cpp \
	ukengine/unikey.h \
	ukengine/usrkeymap.cpp \
	ukengine/usrkeymap.h \
	ukengine/vnconv.h \
	ukengine/vnlexi.h

libunikey_ibus_a_CXXFLAGS = -Wall -funsigned-char

#######
## src/
#######

ibus_engine_unikey_SOURCES = \
	src/main.cpp \
	src/engine.cpp \
	src/engine.h \
	src/engine_private.h \
	src/utils.cpp \
	src/utils.h

ibus_engine_unikey_CPPFLAGS = \
	-I$(top_srcdir)/ukengine \
	-DPKGDATADIR=\"$(pkgdatadir)\" \
	-DLOCALEDIR=\"$(localedir)\" \
	-DLIBEXECDIR=\"$(libexecdir)\" \
	-Wall

ibus_engine_unikey_CXXFLAGS = @IBUS_CFLAGS@
ibus_engine_unikey_LDADD = libunikey-ibus.a @IBUS_LIBS@

component_DATA = src/unikey.xml

componentdir = $(datadir)/ibus/component

src/unikey.xml: src/unikey.xml.in
	$(AM_V_GEN)
	$(AM_V_at)( \
		libexecdir=${libexecdir}; \
		pkgdatadir=${pkgdatadir}; \
		s=`cat $<`; \
		eval "echo \"$${s}\""; \
	) > $@

test: ibus-engine-unikey
	$(builddir)/ibus-engine-unikey

########
## setup
########
ui_DATA = setup/setup-main.ui setup/setup-macro.ui

uidir = $(pkgdatadir)/ui

ibus_setup_unikey_SOURCES = \
	setup/setup.cpp \
	setup/dlg_main_setup.cpp \
	setup/dlg_macro_table.cpp

ibus_setup_unikey_CPPFLAGS = \
	-I$(top_srcdir)/ukengine \
	-I$(top_srcdir)/src \
	-DPKGDATADIR=\"$(pkgdatadir)\" \
	-DUI_DATA_DIR=\"$(uidir)\" \
	-DLOCALEDIR=\"$(localedir)\" \
	-Wall

ibus_setup_unikey_CXXFLAGS = @IBUS_CFLAGS@ @GTK_CFLAGS@
ibus_setup_unikey_LDADD = libunikey-ibus.a @IBUS_LIBS@ @GTK_LIBS@
ibus_setup_unikey_LDFLAGS = $(top_builddir)/src/ibus_engine_unikey-utils.o

########
## icons
########
icons_DATA = icons/ibus-unikey.png

iconsdir = $(pkgdatadir)/icons

######
## end
######

noinst_HEADERS = \
	src/engine.h \
	src/engine_private.h \
	src/engine_const.h \
	src/utils.h \
	setup/dlg_main_setup.h \
	setup/dlg_macro_table.h

EXTRA_DIST = \
	ibus-unikey.spec.in \
	ukengine/README ukengine/COPYING \
	src/unikey.xml.in \
	$(ui_DATA) \
	$(icons_DATA) \
	m4/as-version.m4

CLEANFILES = src/unikey.xml

DISTCLEANFILES = po/stamp-it

rpm: dist @PACKAGE_NAME@.spec
	rpmbuild -bb \
			--define "_sourcedir `pwd`" \
			--define "_builddir `pwd`" \
			--define "_srcrpmdir `pwd`" \
			--define "_rpmdir `pwd`" \
			--define "_specdir `pwd`" \
			@PACKAGE_NAME@.spec

srpm: dist @PACKAGE_NAME@.spec
	rpmbuild -bs \
			--define "_sourcedir `pwd`" \
			--define "_builddir `pwd`" \
			--define "_srcrpmdir `pwd`" \
			--define "_rpmdir `pwd`" \
			--define "_specdir `pwd`" \
			@PACKAGE_NAME@.spec

clean-rpm:
	$(RM) -r "`uname -i`"
	$(RM) -r @PACKAGE_NAME@-@PACKAGE_VERSION@*.src.rpm

clean-local: clean-rpm
	$(RM) -r $(distdir)
