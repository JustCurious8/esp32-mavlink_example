// ------------------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------------------
//#include "Arduino.h"
#include "interface.h"


// ----------------------------------------------------------------------------------
//   Autopilot Interface Class
// ----------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
//   Con/De structors
// ------------------------------------------------------------------------------

Autopilot_Interface::Autopilot_Interface()
{

	system_id    = 0; // system id
	autopilot_id = 0; // autopilot component id
	companion_id = 0; // companion computer component id
	current_messages.sysid  = system_id;
	current_messages.compid = autopilot_id;
}


/*
// ------------------------------------------------------------------------------
//   Read Messages
// ------------------------------------------------------------------------------
*/

uint8_t data = 0;
void Autopilot_Interface::read_messages() 
{ 
  mavlink_message_t message;
  mavlink_status_t status;
  int len = uart_read_bytes(UART_NUM_2, &data, 1, 20 / portTICK_RATE_MS);

  if(len>0) 
  {
    // Try to get a new message
    if(mavlink_parse_char(MAVLINK_COMM_0, data, &message, &status)) 
    {
     
			// Note this doesn't handle multiple message sources.
			current_messages.sysid  = message.sysid;
			current_messages.compid = message.compid;

			// Handle Message ID
			switch (message.msgid)
			{

				case MAVLINK_MSG_ID_HEARTBEAT:
				{
					//Serial.write("<<MAVLINK_MSG_ID_HEARTBEAT\n");
					mavlink_msg_heartbeat_decode(&message, &(current_messages.heartbeat));	
					break;
				}

				case MAVLINK_MSG_ID_SYS_STATUS:
				{
					//Serial.write("<<MAVLINK_MSG_ID_SYS_STATUS\n");
					mavlink_msg_sys_status_decode(&message, &(current_messages.sys_status));
					break;
				}

				case MAVLINK_MSG_ID_BATTERY_STATUS:
				{
					//Serial.write("<<MAVLINK_MSG_ID_BATTERY_STATUS\n");
					mavlink_msg_battery_status_decode(&message, &(current_messages.battery_status));
					break;
				}

				case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
				{
					mavlink_msg_global_position_int_decode(&message, &(current_messages.global_position_int));
                                        printf("Altitude above ground: %i\n",current_messages.global_position_int.relative_alt);
					break;
				}

				default:
				{
					// printf("Warning, did not handle message id %i\n",message.msgid);
					break;
				}
      
      }
        
    }
      
  }
    

}

// ------------------------------------------------------------------------------
//  Set Stream Data  
// ------------------------------------------------------------------------------

int Autopilot_Interface::set_message_interval()
{
  mavlink_command_long_t com = { 0 };
	com.target_system    = system_id;
	com.target_component = autopilot_id;
	com.command          = 511;
	com.confirmation     = 0;
	com.param1           = 33; 
	com.param2           = 1000000;

	// Encode
	mavlink_message_t message;
	mavlink_msg_command_long_encode(system_id, companion_id, &message, &com);
                
  char buf[MAVLINK_MAX_PACKET_LEN];
  // Translate message to buffer
  unsigned len = mavlink_msg_to_send_buffer((uint8_t*)buf, &message);
  
  uart_write_bytes(UART_NUM_2, (const char *)buf, (uint8_t)len);
  uart_write_bytes(UART_NUM_2, (const char *)"\n", sizeof("\n"));
  return len;
}

