/* vim:set et ts=4 sts=4:
 *
 * ibus-unikey
 *
 * Copyright (c) 2009-2012 Le Quoc Tuan <mr.lequoctuan@gmail.com>
 *                    2018 Lzu Tao <1stpangu@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <libintl.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <unistd.h>

#include "engine_const.h"
#include "engine_private.h"
#include "utils.h"
#include "unikey.h"
#include "vnconv.h"

#define _(string) gettext(string)

#define CONVERT_BUF_SIZE 1024

static unsigned char WordBreakSyms[] = {
    ',', ';', ':', '.', '\"', '\'', '!', '?', ' ',
    '<', '>', '=', '+', '-', '*', '/', '\\',
    '_', '~', '`', '@', '#', '$', '%', '^', '&', '(', ')', '{', '}', '[', ']',
    '|'
};

static IBusConfig       *kConfig      = NULL;
static IBusEngineClass  *kParentClass = NULL;
static IBusUnikeyEngine *kUnikey      = NULL; //!< current (focus) unikey engine

/**
 * Define
 *      GType           ibus_unikey_engine_get_type(void);
 *      gpointer        ibus_unikey_engine_parent_class = gtk_gadget_class_intern_init()
 * and declare:
 *      static void     ibus_unikey_engine_init       (IBusUnikeyEngine      *self);
 *      static void     ibus_unikey_engine_class_init (IBusUnikeyEngineClass *klass);
 *
 * See https://developer.gnome.org/gobject/stable/gobject-Type-Information.html
 */
G_DEFINE_TYPE(IBusUnikeyEngine, ibus_unikey_engine, IBUS_TYPE_ENGINE)

void ibus_unikey_init(IBusBus *bus)
{
    UnikeySetup();
    kConfig = ibus_bus_get_config(bus);
}

void ibus_unikey_exit()
{
    UnikeyCleanup();
}

static void ibus_unikey_engine_class_init(IBusUnikeyEngineClass *klass)
{
    GObjectClass *object_class         = G_OBJECT_CLASS(klass);
    object_class->constructor          = ibus_unikey_engine_constructor;

    IBusObjectClass *ibus_object_class = IBUS_OBJECT_CLASS(klass);
    ibus_object_class->destroy         = (IBusObjectDestroyFunc)ibus_unikey_engine_destroy;

    IBusEngineClass *engine_class      = IBUS_ENGINE_CLASS(klass);
    engine_class->process_key_event    = ibus_unikey_engine_process_key_event;
    engine_class->reset                = ibus_unikey_engine_reset;
    engine_class->enable               = ibus_unikey_engine_enable;
    engine_class->disable              = ibus_unikey_engine_disable;
    engine_class->focus_in             = ibus_unikey_engine_focus_in;
    engine_class->focus_out            = ibus_unikey_engine_focus_out;
    engine_class->property_activate    = ibus_unikey_engine_property_activate;

    kParentClass = (IBusEngineClass* )ibus_unikey_engine_parent_class;
}

static void ibus_unikey_engine_init(IBusUnikeyEngine *unikey)
{
    if (g_object_is_floating (unikey))
        g_object_ref_sink (unikey); // make engine sink
    ibus_unikey_engine_load_config(unikey);

    unikey->preeditstr = new std::string();
    ibus_unikey_engine_create_property_list(unikey);
}

static GObject *ibus_unikey_engine_constructor(GType type,
                                               guint n_construct_params,
                                               GObjectConstructParam *construct_params)
{
    IBusUnikeyEngine *unikey = (IBusUnikeyEngine *)
                                   G_OBJECT_CLASS(kParentClass)
                                       ->constructor(type,
                                                     n_construct_params,
                                                     construct_params);

    return (GObject *)unikey;
}

static void ibus_unikey_engine_destroy(IBusUnikeyEngine *unikey)
{
    delete unikey->preeditstr;
    unikey->preeditstr = NULL;

    g_object_unref(unikey->prop_list);
    unikey->prop_list = NULL;

    ((IBusObjectClass *)kParentClass)->destroy((IBusObject *)unikey);
}

static void ibus_unikey_engine_focus_in(IBusEngine *engine)
{
    kUnikey = (IBusUnikeyEngine *)engine;

    ibus_unikey_engine_load_config(kUnikey);
    kParentClass->focus_in(engine);

    //update menu
    ibus_engine_register_properties(engine, kUnikey->prop_list);
}

static void ibus_unikey_engine_focus_out(IBusEngine *engine)
{
    // remove buffer since we lost focus
    // cannot commit here because the text is out of focus
    ibus_unikey_engine_clean_buffer(engine);

    kParentClass->focus_out(engine);
}

static void ibus_unikey_engine_reset(IBusEngine *engine)
{
    ibus_unikey_engine_commit(engine);

    kParentClass->reset(engine);
}

static void ibus_unikey_engine_enable(IBusEngine *engine)
{
    kParentClass->enable(engine);
}

static void ibus_unikey_engine_disable(IBusEngine *engine)
{
    kParentClass->disable(engine);
}

static void ibus_unikey_engine_load_config(IBusUnikeyEngine *unikey)
{
    gchar *str;
    gboolean b;
    guint i;

    // Set default options
    unikey->im = Unikey_IM[0];
    unikey->oc = Unikey_OC[0];
    unikey->ukopt.spellCheckEnabled     = DEFAULT_CONF_SPELLCHECK;
    unikey->ukopt.autoNonVnRestore      = DEFAULT_CONF_AUTONONVNRESTORE;
    unikey->ukopt.modernStyle           = DEFAULT_CONF_MODERNSTYLE;
    unikey->ukopt.freeMarking           = DEFAULT_CONF_FREEMARKING;
    unikey->ukopt.macroEnabled          = DEFAULT_CONF_MACROENABLED;

    if (ibus_unikey_config_get_string(kConfig, CONFIG_SECTION, CONFIG_INPUTMETHOD, &str))
    {
        for (i = 0; i < NUM_INPUTMETHOD; i++)
        {
            if (!strncasecmp(str, Unikey_IMNames[i], 16))
            {
                unikey->im = Unikey_IM[i];
                break;
            }
        }
    } else {
        // set default value
        ibus_unikey_config_set_string(kConfig, CONFIG_SECTION, CONFIG_INPUTMETHOD, Unikey_IMNames[0]);
    }
    g_free(str);
    str = NULL;

    if (ibus_unikey_config_get_string(kConfig, CONFIG_SECTION, CONFIG_OUTPUTCHARSET, &str))
    {
        for (i = 0; i < NUM_OUTPUTCHARSET; i++)
        {
            if (!strncasecmp(str, Unikey_OCNames[i], 16))
            {
                unikey->oc = Unikey_OC[i];
                break;
            }
        }
    } else {
        ibus_unikey_config_set_string(kConfig, CONFIG_SECTION, CONFIG_INPUTMETHOD, Unikey_OCNames[0]);
    }
    g_free(str);
    str = NULL;

    if (ibus_unikey_config_get_boolean(kConfig, CONFIG_SECTION, CONFIG_FREEMARKING, &b))
        unikey->ukopt.freeMarking = b;
    else {
        ibus_unikey_config_set_boolean(kConfig, CONFIG_SECTION, CONFIG_FREEMARKING, DEFAULT_CONF_FREEMARKING);
    }

    if (ibus_unikey_config_get_boolean(kConfig, CONFIG_SECTION, CONFIG_MODERNSTYLE, &b))
        unikey->ukopt.modernStyle = b;
    else {
        ibus_unikey_config_set_boolean(kConfig, CONFIG_SECTION, CONFIG_MODERNSTYLE, DEFAULT_CONF_MODERNSTYLE);
    }

    if (ibus_unikey_config_get_boolean(kConfig, CONFIG_SECTION, CONFIG_MACROENABLED, &b))
        unikey->ukopt.macroEnabled = b;
    else {
        ibus_unikey_config_set_boolean(kConfig, CONFIG_SECTION, CONFIG_MACROENABLED, DEFAULT_CONF_MACROENABLED);
    }

    if (ibus_unikey_config_get_boolean(kConfig, CONFIG_SECTION, CONFIG_SPELLCHECK, &b))
        unikey->ukopt.spellCheckEnabled = b;
    else {
        ibus_unikey_config_set_boolean(kConfig, CONFIG_SECTION, CONFIG_SPELLCHECK, DEFAULT_CONF_SPELLCHECK);
    }

    if (ibus_unikey_config_get_boolean(kConfig, CONFIG_SECTION, CONFIG_AUTORESTORENONVN, &b))
        unikey->ukopt.autoNonVnRestore = b;
    else {
        ibus_unikey_config_set_boolean(kConfig, CONFIG_SECTION, CONFIG_AUTORESTORENONVN, DEFAULT_CONF_AUTONONVNRESTORE);
    }

    // load macro
    gchar *fn = get_macro_file();
    UnikeyLoadMacroTable(fn);
    g_free(fn);

    //update configures to UKEngine
    UnikeySetInputMethod(unikey->im);
    UnikeySetOutputCharset(unikey->oc);
    UnikeySetOptions(&unikey->ukopt);
}

static void ibus_unikey_engine_property_activate(IBusEngine *engine,
                                                 const gchar *prop_name,
                                                 guint prop_state)
{
    IBusProperty *prop;
    IBusText *label;
    guint i, j;

    kUnikey = (IBusUnikeyEngine *)engine;

    // input method active
    if (!strncmp(prop_name, CONFIG_INPUTMETHOD, strlen(CONFIG_INPUTMETHOD)))
    {
        for (i = 0; i < NUM_INPUTMETHOD; i++)
        {
            if (!strncmp(prop_name + strlen(CONFIG_INPUTMETHOD) + 1, Unikey_IMNames[i], 8))
            {
                kUnikey->im = Unikey_IM[i];

                // update label
                for (j = 0; j < kUnikey->prop_list->properties->len; j++)
                {
                    prop = ibus_prop_list_get(kUnikey->prop_list, j);
                    if (!prop)
                        return;

                    if (!strncmp(ibus_property_get_key(prop), CONFIG_INPUTMETHOD, strlen(CONFIG_INPUTMETHOD)))
                    {
                        label = ibus_text_new_from_static_string(Unikey_IMNames[i]);
                        ibus_property_set_label(prop, label);
                        break;
                    }
                } // end update label

                // update property state
                for (j = 0; j < kUnikey->menu_im->properties->len; j++)
                {
                    prop = ibus_prop_list_get(kUnikey->menu_im, j);
                    if (!prop)
                        return;

                    if (!strncmp(ibus_property_get_key(prop), prop_name, 32))
                        ibus_property_set_state(prop, PROP_STATE_CHECKED);
                    else
                        ibus_property_set_state(prop, PROP_STATE_UNCHECKED);
                } // end update property state

                // update menu && save changes
                ibus_engine_register_properties(engine, kUnikey->prop_list);
                ibus_unikey_config_set_string(kConfig, CONFIG_SECTION, CONFIG_INPUTMETHOD, Unikey_IMNames[i]);
                break;
            }
        }
    } // end input method active

    // output charset active
    else if (!strncmp(prop_name, CONFIG_OUTPUTCHARSET, strlen(CONFIG_OUTPUTCHARSET)))
    {
        for (i = 0; i < NUM_OUTPUTCHARSET; i++)
        {
            if (!strncmp(prop_name + strlen(CONFIG_OUTPUTCHARSET) + 1, Unikey_OCNames[i], 16))
            {
                kUnikey->oc = Unikey_OC[i];

                // update label
                for (j = 0; j < kUnikey->prop_list->properties->len; j++)
                {
                    prop = ibus_prop_list_get(kUnikey->prop_list, j);
                    if (!prop)
                        return;

                    if (!strncmp(ibus_property_get_key(prop), CONFIG_OUTPUTCHARSET, strlen(CONFIG_OUTPUTCHARSET)))
                    {
                        label = ibus_text_new_from_static_string(Unikey_OCNames[i]);
                        ibus_property_set_label(prop, label);
                        break;
                    }
                } // end update label

                // update property state
                for (j = 0; j < kUnikey->menu_oc->properties->len; j++)
                {
                    prop = ibus_prop_list_get(kUnikey->menu_oc, j);
                    if (!prop)
                        return;

                    if (!strncmp(ibus_property_get_key(prop), prop_name, 32) == 0)
                        ibus_property_set_state(prop, PROP_STATE_CHECKED);
                    else
                        ibus_property_set_state(prop, PROP_STATE_UNCHECKED);
                } // end update property state

                // update menu && save changes
                ibus_engine_register_properties(engine, kUnikey->prop_list);
                ibus_unikey_config_set_string(kConfig, CONFIG_SECTION, CONFIG_OUTPUTCHARSET, Unikey_OCNames[i]);
                break;
            }
        }
    } // end output charset active

    // if Run setup {{{
    else if (!strncmp(prop_name, "RunSetupGUI", 12))
    {
        pid_t pid = fork();
        int errsv = errno;
        int ret = 0;
        switch (pid)
        {
        case -1:
            g_warning("ibus_unikey_engine_property_activate: Cannot fork with errno %d\n", errsv);
            exit(errsv);
            return;
        case 0:
            ret = execl(LIBEXECDIR "/ibus-setup-unikey", "ibus-setup-unikey", NULL);
            errsv = errno;

            if (ret == -1)
            {
                g_warning("ibus_unikey_engine_property_activate: Cannot exec ibus-setup-unikey with errno %d\n", errsv);
            }
            exit(errsv);
        default:
            break;
        }
    } // }}} Run setup

    UnikeySetInputMethod(kUnikey->im);
    UnikeySetOutputCharset(kUnikey->oc);
    UnikeySetOptions(&kUnikey->ukopt);

    kParentClass->property_activate(engine, prop_name, prop_state);
}

static void ibus_unikey_engine_create_property_list(IBusUnikeyEngine *unikey)
{
    if (!unikey->prop_list)
    {
        unikey->prop_list = ibus_prop_list_new();
        unikey->menu_im   = ibus_prop_list_new();
        unikey->menu_oc   = ibus_prop_list_new();

        g_object_ref_sink(unikey->prop_list);
    }
    IBusProperty *prop    = NULL;
    IBusText     *label   = NULL;
    IBusText     *tooltip = NULL;
    gchar name[32];
    const size_t len_name = 32;
    guint i;

    // Create input method menu {{{
    tooltip = ibus_text_new_from_static_string("");
    for (i = 0; i < NUM_INPUTMETHOD; ++i)
     {

        label = ibus_text_new_from_static_string(Unikey_IMNames[i]);
        snprintf(name, len_name, CONFIG_INPUTMETHOD "_%s", Unikey_IMNames[i]);
        gboolean method_choosen = (Unikey_IM[i] == unikey->im);
        IBusPropState state = method_choosen ? PROP_STATE_CHECKED : PROP_STATE_UNCHECKED;
        prop = ibus_property_new(name,
                                 PROP_TYPE_RADIO,
                                 label,
                                 NULL,
                                 tooltip,
                                 TRUE,
                                 TRUE,
                                 state,
                                 NULL);

        if (!ibus_prop_list_update_property(unikey->menu_im, prop))
            ibus_prop_list_append(unikey->menu_im, prop);
    }
    // }}} create input method menu

    // Create output charset menu {{{
    tooltip = ibus_text_new_from_static_string("");
    for (i = 0; i < NUM_OUTPUTCHARSET; ++i)
    {
        label = ibus_text_new_from_static_string(Unikey_OCNames[i]);
        snprintf(name, len_name, CONFIG_OUTPUTCHARSET "_%s", Unikey_OCNames[i]);
        gboolean charset_choosen = (Unikey_OC[i] == unikey->oc);
        IBusPropState state = charset_choosen ? PROP_STATE_CHECKED : PROP_STATE_UNCHECKED;
        prop = ibus_property_new(name,
                                 PROP_TYPE_RADIO,
                                 label,
                                 NULL,
                                 tooltip,
                                 TRUE,
                                 TRUE,
                                 state,
                                 NULL);

        if (!ibus_prop_list_update_property(unikey->menu_oc, prop))
            ibus_prop_list_append(unikey->menu_oc, prop);
    }
    // }}} create output charset menu

    // Create option menu (for configure unikey) {{{
    // add option property
    // }}} create option menu

    // Create top menu {{{

    // * add input method menu
    for (i = 0; i < NUM_INPUTMETHOD; i++)
    {
        if (Unikey_IM[i] == unikey->im)
            break;
    }
    label = ibus_text_new_from_static_string(Unikey_IMNames[i]);
    tooltip = ibus_text_new_from_static_string(_("Choose input method"));
    prop = ibus_property_new(CONFIG_INPUTMETHOD,
                             PROP_TYPE_MENU,
                             label,
                             NULL,
                             tooltip,
                             TRUE,
                             TRUE,
                             PROP_STATE_UNCHECKED,
                             unikey->menu_im);

    if (!ibus_prop_list_update_property(unikey->prop_list, prop))
        ibus_prop_list_append(unikey->prop_list, prop);

    // * add output charset menu
    for (i = 0; i < NUM_OUTPUTCHARSET; i++)
    {
        if (Unikey_OC[i] == unikey->oc)
            break;
    }
    label = ibus_text_new_from_static_string(Unikey_OCNames[i]);
    tooltip = ibus_text_new_from_static_string(_("Choose output charset"));
    prop = ibus_property_new(CONFIG_OUTPUTCHARSET,
                             PROP_TYPE_MENU,
                             label,
                             NULL,
                             tooltip,
                             TRUE,
                             TRUE,
                             PROP_STATE_UNCHECKED,
                             unikey->menu_oc);

    if (!ibus_prop_list_update_property(unikey->prop_list, prop))
        ibus_prop_list_append(unikey->prop_list, prop);

    // * add option menu
    label = ibus_text_new_from_static_string(_("Full setup..."));
    tooltip = ibus_text_new_from_static_string(_("Full setup utility for IBus-Unikey"));
    prop = ibus_property_new("RunSetupGUI",
                             PROP_TYPE_NORMAL,
                             label,
                             NULL,
                             tooltip,
                             TRUE,
                             TRUE,
                             PROP_STATE_UNCHECKED,
                             NULL);

    if (!ibus_prop_list_update_property(unikey->prop_list, prop))
        ibus_prop_list_append(unikey->prop_list, prop);
    // }}} Create top menu
}

static void ibus_unikey_engine_update_preedit_string(IBusUnikeyEngine *unikey)
{
    // update and display text
    IBusText *text = ibus_text_new_from_static_string(unikey->preeditstr->c_str());
    ibus_engine_update_preedit_text_with_mode((IBusEngine*)unikey,
                                              text,
                                              ibus_text_get_length(text),
                                              TRUE,
                                              IBUS_ENGINE_PREEDIT_COMMIT);
}

static void ibus_unikey_engine_erase_chars(IBusEngine *engine, int num_chars)
{
    int k = num_chars;

    guchar c;
    int i;
    for (i = kUnikey->preeditstr->length() - 1; i >= 0 && k > 0; i--)
    {
        c = kUnikey->preeditstr->at(i);

        // count down if byte is begin byte of utf-8 char
        if (c < (guchar)'\x80' || c >= (guchar)'\xC0')
        {
            k--;
        }
    }

    kUnikey->preeditstr->erase(i+1);
}

static gboolean ibus_unikey_engine_process_key_event(IBusEngine *engine,
                                                     guint       keysym,
                                                     guint       keycode,
                                                     guint       modifiers)
{
    kUnikey = (IBusUnikeyEngine *)engine;

    static gboolean ok = ibus_unikey_engine_process_key_event_preedit(
                                    engine, keysym, keycode, modifiers);

    // check last keyevent with shift
    if (keysym >= IBUS_space && keysym <=IBUS_asciitilde)
        kUnikey->last_key_with_shift = modifiers & IBUS_SHIFT_MASK;
    else
        kUnikey->last_key_with_shift = FALSE;

    return ok;
}

/** Use xev to get key state when debug
 * Ref:
 *      https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent/key/Key_Values
 *      https://wiki.archlinux.org/index.php/X_KeyBoard_extension
 */
static gboolean ibus_unikey_engine_process_key_event_preedit(IBusEngine *engine,
                                                             guint       keyval,
                                                             guint       keycode,
                                                             guint       modifiers)
{
    // A key is released - not really important
    if (modifiers & IBUS_RELEASE_MASK)
        return FALSE;

    // We only care about Control and Modifier 1
    // (Usually Alt_L (0x40), Alt_R (0x6c), Meta_L (0xcd))
    modifiers &= (IBUS_CONTROL_MASK | IBUS_MOD1_MASK);

    // special key, hot key that we won't handle
    if (modifiers & IBUS_CONTROL_MASK
        || modifiers & IBUS_MOD1_MASK // alternate mask
        || keyval == IBUS_Control_L
        || keyval == IBUS_Control_R
        || keyval == IBUS_Tab
        || keyval == IBUS_Return
        || keyval == IBUS_Delete
        || keyval == IBUS_KP_Enter
        || (IBUS_Home <= keyval && keyval <= IBUS_Insert)
        || (IBUS_KP_Home <= keyval && keyval <= IBUS_KP_Delete)
        )
    {
        ibus_unikey_engine_commit(engine);
        return FALSE;
    }

    if ((IBUS_Caps_Lock <= keyval && keyval <= IBUS_Hyper_R)
            || (!(modifiers & IBUS_SHIFT_MASK) && (keyval == IBUS_Shift_L || keyval == IBUS_Shift_R))  // when press one shift key
        )
    {
        return FALSE;
    }

    if (IBUS_KP_Multiply <= keyval && keyval <= IBUS_KP_9)
    {
        ibus_unikey_engine_commit(engine);
        return FALSE;
    }

    // capture BackSpace
    if (keyval == IBUS_BackSpace)
    {
        UnikeyBackspacePress();

        if (!UnikeyBackspaces || kUnikey->preeditstr->empty())
            return FALSE;

        if (kUnikey->preeditstr->length() <= (guint)UnikeyBackspaces)
        {
            kUnikey->preeditstr->clear();
            ibus_engine_hide_preedit_text(engine);
        }
        else
        {
            ibus_unikey_engine_erase_chars(engine, UnikeyBackspaces);
            ibus_unikey_engine_update_preedit_string(kUnikey);
        }

        // change tone position after press backspace
        if (UnikeyBufChars > 0)
        {
            if (kUnikey->oc == CONV_CHARSET_XUTF8)
            {
                kUnikey->preeditstr->append((gchar *)UnikeyBuf, UnikeyBufChars);
            }
            else
            {
                static guchar buf[CONVERT_BUF_SIZE];
                int bufSize = CONVERT_BUF_SIZE;

                latinToUtf(buf, UnikeyBuf, UnikeyBufChars, &bufSize);
                kUnikey->preeditstr->append((gchar *)buf, CONVERT_BUF_SIZE - bufSize);
            }

            ibus_unikey_engine_update_preedit_string(kUnikey);
        }
        return TRUE;
    } // end capture BackSpace

    // capture ascii printable char
    if ((keyval >= IBUS_space && keyval <= IBUS_asciitilde)
        || keyval == IBUS_Shift_L || keyval == IBUS_Shift_R) // sure this have IBUS_SHIFT_MASK
    {

        UnikeySetCapsState(modifiers & IBUS_SHIFT_MASK, modifiers & IBUS_LOCK_MASK);

        // shift + space, shift + shift event
        if ((kUnikey->last_key_with_shift == FALSE && (modifiers & IBUS_SHIFT_MASK)
                && keyval == IBUS_space && !UnikeyAtWordBeginning())
            || (keyval == IBUS_Shift_L || keyval == IBUS_Shift_R) // sure this have IBUS_SHIFT_MASK
           )
        {
            UnikeyRestoreKeyStrokes();
        } // end shift + space, shift + shift event

        else
        {
            UnikeyFilter(keyval);
        }
        // end process keyval

        // process result of ukengine
        if (UnikeyBackspaces > 0)
        {
            if (kUnikey->preeditstr->length() <= (guint)UnikeyBackspaces)
            {
                kUnikey->preeditstr->clear();
            }
            else
            {
                ibus_unikey_engine_erase_chars(engine, UnikeyBackspaces);
            }
        }

        if (UnikeyBufChars > 0)
        {
            if (kUnikey->oc == CONV_CHARSET_XUTF8)
            {
                kUnikey->preeditstr->append((gchar *)UnikeyBuf, UnikeyBufChars);
            }
            else
            {
                static unsigned char buf[CONVERT_BUF_SIZE];
                int bufSize = CONVERT_BUF_SIZE;

                latinToUtf(buf, UnikeyBuf, UnikeyBufChars, &bufSize);
                kUnikey->preeditstr->append((gchar *)buf, CONVERT_BUF_SIZE - bufSize);
            }
        }
        else if (keyval != IBUS_Shift_L && keyval != IBUS_Shift_R) // if ukengine not process
        {
            static char s[6];
            static int n = g_unichar_to_utf8(keyval, s); // convert ucs4 to utf8 char
            kUnikey->preeditstr->append(s, n);
        }
        // end process result of ukengine

        // commit string: if need
        if (kUnikey->preeditstr->length() > 0)
        {
            guchar last_char = kUnikey->preeditstr->at(kUnikey->preeditstr->length() - 1);
            static guint i;
            for (i = 0; i < sizeof(WordBreakSyms); i++)
            {
                if (WordBreakSyms[i] == last_char && WordBreakSyms[i] == keyval)
                {
                    ibus_unikey_engine_commit(engine);
                    return TRUE;
                }
            }
        }
        // end commit string

        ibus_unikey_engine_update_preedit_string(kUnikey);
        return TRUE;
    } //end capture printable char

    // non process key
    ibus_unikey_engine_commit(engine);
    return FALSE;
}

static void ibus_unikey_engine_clean_buffer(IBusEngine *engine)
{
    UnikeyResetBuf();
    kUnikey->preeditstr->clear();
    ibus_engine_hide_preedit_text(engine);
}

static void ibus_unikey_engine_commit(IBusEngine *engine)
{
    kUnikey = (IBusUnikeyEngine *)engine;

    if (kUnikey->preeditstr->length() > 0)
    {
        IBusText *text;

        text = ibus_text_new_from_static_string(kUnikey->preeditstr->c_str());
        ibus_engine_commit_text(engine, text);
    }

    ibus_unikey_engine_clean_buffer(engine);
}
