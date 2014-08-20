#include "init.h"

int init_tomcrypt_lib()
{
    //set up system prng
    if (register_prng(&sprng_desc) == -1)  {
        printf("Error registering sprng");
        return -1;
    }

    if (register_hash(&sha1_desc) == -1) {
        printf("Error registering sha1");
        return -1;
    }
    
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
