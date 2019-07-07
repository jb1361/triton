/*
 Name:		help.ino
 Created:	7/02/2019
 Author:	Justin
*/

void cmd_help(int num_commands, char* commands_str) {
	Serial.println("The following commands are available:");

	for (int i = 0; i < num_commands; i++) {
		Serial.print("  ");
		Serial.println(commands_str[i]);
	}
	Serial.println("");
	Serial.println("You can for instance type \"help led\" for more info on the LED command.");
}

