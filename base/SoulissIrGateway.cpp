#define command_1_code_type		NEC
#define command_1_nBits			32
#define command_1_value			0x7E8154AB

decode_results  ir_data;
IRrecv *irrecv;
IRsend *irsend;
int ir_status = 0;

int data_old, data_new, counter = 0;

void Souliss_ir_setup(int send_pin, int recv_pin)
{
	irrecv = new IRrecv(recv_pin);
	irsend = new IRsend(send_pin); //an IR led is connected to GPIO pin 5
	irrecv -> enableIRIn();  // Start the receiver
	irsend -> begin();		//Start the IR transmitter
}

void Souliss_IR_scan()
{
	if (irrecv->decode(&ir_data))
	{
		Serial.println(ir_data.value,HEX);
		ir_status = 1;
		delay(100);
		irrecv->resume();
	}
}

void Souliss_ir_control_remote_node(U8 *memory_map, int target_address, int command_slot, int target_command, int ir_decode_type, int ir_command)
{	
	if (ir_status == 0)
	{
		Souliss_IR_scan();
	}
	if (ir_status >= 1)
	{
		if ((ir_data.decode_type == ir_decode_type) & (ir_data.value == ir_command))
		{
			Souliss_RemoteInput(target_address, command_slot, target_command);
			ir_status = 0;
		}
		else
		{
			ir_status++;
			if (ir_status == 3)
			{
				ir_status = 0;
			}
		}
	}
}

void Souliss_ir_control_local_node(U8 *memory_map, int command_slot, int target_command, int ir_decode_type, unsigned long ir_command)
{
	if (ir_status == 0)
	{
		Souliss_IR_scan();
	}
	if (ir_status >= 1)
	{
		if ((ir_data.decode_type == ir_decode_type) & (ir_data.value == ir_command))
		{
			memory_map[MaCaco_IN_s + command_slot] = target_command;
			ir_status = 0;
		}
		else
		{
			ir_status++;
			if (ir_status == 3)
			{
				ir_status = 0;
			}
		}
	}
}

void Souliss_ir_control_device(U8 *memory_map, int typical, int command_slot, int parameter, int ir_decode_type, int nbits, unsigned long ir_command)
{
	if (counter == 0)
	{
		data_old = memory_map[MaCaco_OUT_s + command_slot];
		counter++;
	}
	else
	{
		data_new = memory_map[MaCaco_OUT_s + command_slot];
		counter = 0;
	}
	
	
	if (data_new != data_old)
	{
		if ( (memory_map[MaCaco_TYP_s + command_slot] == typical) & (memory_map[MaCaco_OUT_s + command_slot] == parameter))
		{
			if (ir_decode_type == NEC)
			{
				irsend -> sendNEC(ir_command, nbits);
			}
		}
	}


	/*if (ir_decode_type == NEC)
	{
		irsend->sendNEC(ir_command, nbits);
	}
	else if (ir_decode_type == RC6)
	{
		irsend->sendRC6(ir_command, nbits);
	}*/
}

// void Souliss_ir_send_command(int ir_decode_type, int nbits, unsigned long ir_command, int optional_parameter)
// {
	// irsend.begin();
	// if (ir_decode_type == NEC)
	// {
		// irsend.sendNEC(ir_command, nbits);
	// }
	// else if (ir_decode_type == RC6)
	// {
		// irsend.sendRC6(ir_command, nbits);
	// }
	// else if (ir_decode_type == RC5)
	// {
		// irsend.sendRC5(ir_command, nbits);
	// }
	// else if (ir_decode_type == COOLIX)
	// {
		// irsend.sendCOOLIX(ir_command, nbits);
	// }
	// else if (ir_decode_type == LG)
	// {
		// irsend.sendLG(ir_command, nbits);
	// }
	// else if (ir_decode_type == WHYNTER)
	// {
		// irsend.sendWhynter(ir_command, nbits);
	// }
	// else if (ir_decode_type == SONY)
	// {
		// irsend.sendSony(ir_command, nbits);
	// }
	// else if (ir_decode_type == PANASONIC)
	// {
		// irsend.sendPanasonic(optional_parameter, ir_command);
	// }
	// else if (ir_decode_type == JVC)
	// {
		// irsend.sendJVC(ir_command, nbits, optional_parameter);
	// }
	// else if (ir_decode_type == SAMSUNG)
	// {
		// irsend.sendSAMSUNG(ir_command, nbits);
	// }
	// else if (ir_decode_type == SHARP)
	// {
		// irsend.sendSharp(optional_parameter, ir_command);
	// }

// }

