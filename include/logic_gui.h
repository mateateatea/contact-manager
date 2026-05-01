#ifndef LOGIC_GUI_H
#define LOGIC_GUI_H

#include "structs.h"

int contact_add_gui(struct ContactArray *arr, 
                    const char *first_name, 
                    const char *last_name, 
                    const char *phone, 
                    const char *email, 
                    const char *address, 
                    const char *note);

#endif