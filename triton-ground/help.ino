void help() {
	Serial.println("The following commands are available:");

	for (int i = 0; i < num_commands; i++) {
		Serial.print("  ");
		Serial.println(commands_str[i]);
	}
	Serial.println("");
	Serial.println("You can for instance type \"help led\" for more info on the LED command.");
}

