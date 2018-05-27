#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <libintl.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

#include "engine.h"
#include "utils.h"
#include "unikey.h"

/* command line options */
static gboolean kIBus    = FALSE;
//static gboolean kVerbose = FALSE;
static gboolean kVersion = FALSE;

static const GOptionEntry kEntries[] = {
    {"ibus", 'i', 0, G_OPTION_ARG_NONE, &kIBus, "component is executed by ibus", NULL},
    // verbose is a useless option
    //{"verbose", 'v', 0, G_OPTION_ARG_NONE, &kVerbose, "verbose", NULL},
    {"version", 'V', 0, G_OPTION_ARG_NONE, &kVersion, "print ibus-unikey version", NULL},
    {NULL},
};

static void
ibus_disconnected_cb(IBusBus *bus, gpointer user_data)
{
    ibus_quit();
}

static void
start_component(void)
{
    ibus_init();
    //!< An IBusBus connects with IBus daemon.
    IBusBus *bus = ibus_bus_new();
    if (!ibus_bus_is_connected(bus)) {
        g_warning("start_component: Cannot connect to IBus daemon.\n");
        exit(0);
    }

    g_signal_connect(bus, "disconnected", G_CALLBACK(ibus_disconnected_cb), NULL);

    //!< Factory for creating engine instances.
    IBusFactory *factory = ibus_factory_new(ibus_bus_get_connection(bus));

    IBusComponent *component = ibus_unikey_get_component();

    factory = ibus_factory_new(ibus_bus_get_connection(bus));

    GList *engines = ibus_component_get_engines(component);
    GList *p = NULL;
    const gchar *engine_name = NULL;

    for (p = engines; p != NULL; p = p->next)
    {
        IBusEngineDesc *engine = (IBusEngineDesc *)p->data;
        engine_name = ibus_engine_desc_get_name(engine);
        ibus_factory_add_engine(factory, engine_name, IBUS_TYPE_UNIKEY_ENGINE);
    }

    if (kIBus)
    {
        if (!ibus_bus_request_name(bus, UNIKEY_DBUS_NAME, 0))
        {
            g_debug("start_component: Cannot request " UNIKEY_DBUS_NAME " from IBus daemon synchronously.\n");
            goto fail;
        }
    }
    else
    {
        if (!ibus_bus_register_component(bus, component))
        {
            g_debug("start_component:  Cannot register Unikey component to IBus daemon synchronously.\n");
            goto fail;
        }
    }

    goto check_ok;
fail:
    g_object_unref(component);
    exit(EXIT_FAILURE);
check_ok:
    g_object_unref(component);

    ibus_unikey_init(bus);
    ibus_main();
    ibus_unikey_exit();
    g_object_unref(factory);
    g_object_unref(bus);
}

int
main(gint argc, gchar **argv)
{
    // select a locale based on the user choice of the appropriate environment variables
    setlocale(LC_ALL, "");
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    textdomain(GETTEXT_PACKAGE);

    // Parse the command line
    GOptionContext* context = g_option_context_new("- ibus unikey engine component");
    g_option_context_add_main_entries(context, kEntries, "ibus-unikey");

    GError *error = NULL;
    gboolean parse_ok = g_option_context_parse(context, &argc, &argv, &error);
    g_option_context_free(context);
    if (!parse_ok) {
        g_print("Option parsing failed: %s\n", error->message);
        g_error_free(error);
        exit(EXIT_FAILURE);
    }

    if (kVersion)
    {
        g_print(PACKAGE_STRING " (engine component)\n"
            "  Copyright (C) 2009 - 2012 Ubuntu-VN <http://www.ubuntu-vn.org>\n"
            "  Author: Lê Quốc Tuấn <mr.lequoctuan@gmail.com>\n"
            "  Homepage: <http://ibus-unikey.googlecode.com>\n"
            "  License: GNU GPLv3\n"
        );
        exit(EXIT_SUCCESS);
    }

    start_component();

    return 0;
}
