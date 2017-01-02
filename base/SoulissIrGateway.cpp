decode_results  ir_data;
IRrecv *irrecv;
IRsend *irsend;
int ir_status = 0;

int data_old = 0; 
int data_new = 0; 
int counter	 = 0;

int debug = 0;

void Souliss_IR_Debug(int value)
{
	debug = value;
}


void Souliss_IR_Setup(int send_pin, int recv_pin)
{
	irrecv = new IRrecv(recv_pin);
	irsend = new IRsend(send_pin);
	irrecv -> enableIRIn();			// Start the receiver
	irsend -> begin();				//Start the IR transmitter
}

void Souliss_IR_scan()
{
	if (irrecv->decode(&ir_data))
	{
		if (debug == 1)
		{
			Serial.println(ir_data.decode_type);
			Serial.println(ir_data.bits);
			Serial.println(ir_data.value, HEX);
		}
		ir_status = 1;
		delay(100);
		irrecv->resume();
	}
}

void Souliss_IR_control_remote_node(U8 *memory_map, int target_address, int command_slot, int target_command, int ir_decode_type, int ir_command)
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

void Souliss_IR_control_local_node(U8 *memory_map, int command_slot, int target_command, int ir_decode_type, unsigned long ir_command)
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

void Souliss_IR_control_irdevice(U8 *memory_map, int typical, int command_slot, int parameter, int ir_decode_type, int nbits, unsigned long ir_command)
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
			if (ir_decode_type == LG)
			{
				irsend->sendLG(ir_command, nbits);
			}
			else if (ir_decode_type == NEC)
			{
				irsend -> sendNEC(ir_command, nbits);
			}
			else if (ir_decode_type == RC5)
			{
				irsend->sendRC5(ir_command, nbits);
			}
			else if (ir_decode_type == RC6)
			{
				irsend -> sendRC6(ir_command, nbits);
			}
			else if (ir_decode_type == SAMSUNG)
			{
				irsend->sendSAMSUNG(ir_command, nbits);
			}
			else if (ir_decode_type == SONY)
			{
				irsend -> sendSony(ir_command, nbits);
			}
		}
	}
}