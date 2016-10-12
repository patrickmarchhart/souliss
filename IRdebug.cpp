/////////////////////////////////////////
//                                     //
//     Sample Code for IR-Debuging     //
//                                     //
/////////////////////////////////////////

#include "IRdebug.h"
#include "Arduino.h"       

void  print_ir_data(decode_results *ir_data)
{
	// Panasonic has an Address
	if (ir_data->decode_type == PANASONIC) {
		Serial.print(ir_data->panasonicAddress, HEX);
		Serial.print(":");
	}

	// Print Code
	Serial.print(ir_data->value, HEX);
}

void  print_ir_code_type(decode_results *ir_data)
{
	switch (ir_data->decode_type)
	{
	default:
	case UNKNOWN:      Serial.print("UNKNOWN");       break;
	case NEC:          Serial.print("NEC");           break;
	case SONY:         Serial.print("SONY");          break;
	case RC5:          Serial.print("RC5");           break;
	case RC6:          Serial.print("RC6");           break;
	case DISH:         Serial.print("DISH");          break;
	case SHARP:        Serial.print("SHARP");         break;
	case JVC:          Serial.print("JVC");           break;
	case SANYO:        Serial.print("SANYO");         break;
	case MITSUBISHI:   Serial.print("MITSUBISHI");    break;
	case SAMSUNG:      Serial.print("SAMSUNG");       break;
	case LG:           Serial.print("LG");            break;
	case WHYNTER:      Serial.print("WHYNTER");       break;
	case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break;
	case PANASONIC:    Serial.print("PANASONIC");     break;
	}
}


void  dumpInfo(decode_results *results)
{
	// Show Encoding standard
	Serial.print("Encoding  : ");
	print_ir_code_type(results);
	Serial.println("");

	// Show Code & length
	Serial.print("Code      : ");
	print_ir_data(results);
	Serial.print(" (");
	Serial.print(results->bits, DEC);
	Serial.println(" bits)");
}