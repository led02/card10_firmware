#ifndef EPICARDIUM_MODULES_CONFIG_H_INCLUDED
#define EPICARDIUM_MODULES_CONFIG_H_INCLUDED

#include <stdbool.h>

enum EpicConfigOption {
    #define CARD10_SETTING(identifier, spelling, type, default_value) Option ## identifier,
	#include "modules/config.def"
    _EpicOptionCount
};

//initialize configuration values and load card10.cfg
void load_config(void);

bool config_get_boolean(enum EpicConfigOption option);
long config_get_integer(enum EpicConfigOption option);
double config_get_float(enum EpicConfigOption option);
const char* config_get_string(enum EpicConfigOption option);


#endif//EPICARDIUM_MODULES_CONFIG_H_INCLUDED
