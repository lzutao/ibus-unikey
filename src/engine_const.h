#ifndef __ENGINE_CONST_H__
#define __ENGINE_CONST_H__

#ifndef HAVE_CONFIG_H
    #define PACKAGE_NAME        "ibus-unikey"
    #define PACKAGE_VERSION     "0.6.2"
    #define GETTEXT_PACKAGE     PACKAGE_NAME
    #define PACKAGE_BUGREPORT   "https://github.com/vn-input/ibus-unikey"
    #define PACKAGE_STRING      PACKAGE_NAME " " PACKAGE_VERSION

    // For Ubuntu, LIBEXECDIR of ibus is NOT /usr/libexec
    #define LIBEXECDIR          "/usr/lib/ibus"
    #define LOCALEDIR           "/usr/share/locale"
    #define PKGDATADIR          "/usr/share/ibus-unikey"
    #define UI_DATA_DIR         "/usr/share/ibus-unikey/ui"
#endif

#define UNIKEY_DBUS_NAME         "org.freedesktop.IBus.Unikey"

//!< Ready for migrating from IBusConfig to GSettings
#define UNIKEY_SCHEMA_ID         "org.freedesktop.ibus.engine.unikey"

#define UNIKEY_MACRO_FILE        ".ibus/unikey/macro"

/**
 * settings is saved in ~/.config/dconf/user
 * or cache in ~/.cache/ibus/ibus/registry
 */
#define CONFIG_SECTION           "engine/Unikey"

#define CONFIG_INPUTMETHOD       "input_method"
#define CONFIG_OUTPUTCHARSET     "output_charset"
#define CONFIG_SPELLCHECK        "spell_check"
#define CONFIG_AUTORESTORENONVN  "auto_restore_non_vn"
#define CONFIG_MODERNSTYLE       "modern_style"
#define CONFIG_FREEMARKING       "free_marking"
#define CONFIG_MACROENABLED      "macro_enabled"

// DEFAULT options
#define DEFAULT_CONF_SPELLCHECK         TRUE
#define DEFAULT_CONF_AUTONONVNRESTORE   TRUE
#define DEFAULT_CONF_MODERNSTYLE        FALSE
#define DEFAULT_CONF_FREEMARKING        TRUE
#define DEFAULT_CONF_MACROENABLED       FALSE

#endif

