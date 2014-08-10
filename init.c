#include "init.h"

int init_tomcrypt_lib()
{
    //set up system prng
    register_prng(&sprng_desc);

    //set up math processor
    ltc_mp = gmp_desc;

    return 0;
}

int init_mosquitto_lib()
{
    mosquitto_lib_init();

    return 0;
}

int init_jansson_lib()
{

    return 0;
}
