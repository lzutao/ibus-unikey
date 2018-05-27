#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <libintl.h>
#include <locale.h>
#include <gtk/gtk.h>

#include "dlg_main_setup.h"

#define _(string) gettext(string)

static gboolean kVersion = FALSE;

static const GOptionEntry entries[] =
{
    { "version", 'V', 0, G_OPTION_ARG_NONE, &kVersion, "print ibus-unikey version", NULL },
    { NULL },
};

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    textdomain(GETTEXT_PACKAGE);

    // Parse the command line
    GOptionContext *context = g_option_context_new("- ibus unikey setup component");
    g_option_context_add_main_entries(context, entries, "ibus-unikey");
    GError *error = NULL;
    gboolean parse_ok = g_option_context_parse(context, &argc, &argv, &error);

    if (!parse_ok)
    {
        g_print("Option parsing failed: %s\n", error->message);
        g_error_free(error);
        exit(EXIT_FAILURE);
    }

    if (kVersion)
    {
        g_print(PACKAGE_STRING " (setup component)"
            "\n  Copyright (C) 2009 - 2012 Ubuntu-VN <http://www.ubuntu-vn.org>"
            "\n  Author: Lê Quốc Tuấn <mr.lequoctuan@gmail.com>"
            "\n  Homepage: <http://ibus-unikey.googlecode.com>"
            "\n  License: GNU GPL3"
            "\n");
        return 0;
    }

    gtk_init(&argc, &argv);

    gtk_window_set_default_icon_from_file(PKGDATADIR "/icons/ibus-unikey.png", NULL);

    GtkWidget* main_dlg = unikey_main_setup_dialog_new();       // create main dlg
    g_signal_connect(main_dlg, "destroy", gtk_main_quit, NULL); // connect with signal

    gtk_dialog_run(GTK_DIALOG(main_dlg));

    return 0;
}
