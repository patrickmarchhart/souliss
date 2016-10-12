#ifndef IRdebug_h
#define IRdebug_h

#include "IRremoteESP8266.h"

void  print_ir_data(decode_results *ir_data);
void  print_ir_code_type(decode_results *ir_data);
void  dumpInfo(decode_results *results);

#endif