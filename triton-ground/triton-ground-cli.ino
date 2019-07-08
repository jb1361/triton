
/*
 Name:		triton-ground-cli.ino
 Created:	7/02/2019 10:22:27 PM
 Author:	Justin
*/

#define LINE_BUF_SIZE 128   //Maximum input string length
#define ARG_BUF_SIZE 64     //Maximum argument string length
#define MAX_NUM_ARGS 8      //Maximum number of arguments

char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];

int cmd_help();
int cmd_test();
int cmd_getGPSData();
int cmd_activateFlywheel();
int cmd_deactivateFlywheel();
int cmd_flywheelStatus();
int cmd_getIMUData();

int (*commands_func[])() {
	&cmd_help,
	&cmd_test,
	&cmd_getGPSData,
	&cmd_activateFlywheel,
	&cmd_deactivateFlywheel,
	&cmd_flywheelStatus,
	&cmd_getIMUData
};

//List of command names
const char* commands_str[] = {
	"--help",
	"--test",
	"--get gpsdata",
	"--activateFlywheel",
	"--deactivateFlywheel",
	"--flywheelStatus",
	"--getIMUData"
};

int num_commands = sizeof(commands_str) / sizeof(char*);

void cli_init() {
	Serial.println("Welcome to this simple Arduino command line interface (CLI).");
	Serial.println("Type \"--help\" to see a list of commands.");
}

void run_cli() {
	read_line();
}

void read_line() {
	String command;
	if (Serial.available()) {
		command = Serial.readStringUntil('\n');
		if (command.charAt(0) == '-' && command.charAt(1) == '-') {
			Serial.println(command);
			execute(command);
		}
	}
}

int execute(String command) {
	for (int i = 0; i < num_commands; i++) {
		if (strcmp(command.c_str(), commands_str[i]) == 0) {
			return(*commands_func[i])();
		}
	}
	Serial.println("Invalid command. Type \"--help\" for more.");
	return 0;
}

int cmd_help() {
	Serial.println("The following commands are available:");

	for (int i = 0; i < num_commands; i++) {
		Serial.print("  ");
		Serial.println(commands_str[i]);
	}
	Serial.println("");

}

int cmd_test() {
	TransmitData("test");
}

int cmd_getGPSData() {
	TransmitData("gps");
}

int cmd_activateFlywheel() {
	TransmitData("activateFlywheel");
}

int cmd_deactivateFlywheel() {
	TransmitData("deactivateFlywheel");
}

int cmd_flywheelStatus() {
	TransmitData("flywheelStatus");
}

int cmd_getIMUData() {
	TransmitData("IMUData");
}
