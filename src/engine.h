#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <ibus.h>

/**
 * Returns a numerical value which represents the unique
 * identifier of unikey engine
 */
#define IBUS_TYPE_UNIKEY_ENGINE     (ibus_unikey_engine_get_type())

void    ibus_unikey_init            (IBusBus *bus);
void    ibus_unikey_exit            (void);

/**
 * Use G_DEFINE_TYPE to define it
 */
GType   ibus_unikey_engine_get_type (void);

#endif

