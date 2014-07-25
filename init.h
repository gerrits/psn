#ifndef _PSN_INIT_H_
#define _PSN_INIT_H_

#include <gmp.h>

#include "tomcrypt.h"
#include "mosquitto.h"
#include "jansson.h"

int init_tomcrypt_lib();
int init_mosquitto_lib();
int init_jansson_lib();


#endif /* _INIt_H_ */