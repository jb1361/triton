
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

bool error_flag = false;

int cmd_help();
int cmd_test();

int (*commands_func[])() {
	&cmd_help,
	&cmd_test
};

//List of command names
const char* commands_str[] = {
	"help",
	"test"
};

int num_commands = sizeof(commands_str) / sizeof(char*);

void cli_init() {
	Serial.println("Welcome to this simple Arduino command line interface (CLI).");
	Serial.println("Type \"--help\" to see a list of commands.");
	Serial.print("> ");
}

void run_cli() {
	//Serial.print("> ");

	read_line();
	/*if (!error_flag) {
		parse_line();
	}*/
	if (!error_flag) {
		execute();
		Serial.print("> ");
	}

	memset(line, 0, LINE_BUF_SIZE);
	memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);

	error_flag = false;
}

void read_line() {
	String line_string;

	if (Serial.available()) {
		line_string = Serial.readStringUntil('\n');
		if (!line_string.charAt(0) == '-' && !line_string.charAt(1) == '-') 
			error_flag = true;
	}
}

int execute() {
	for (int i = 0; i < num_commands; i++) {
		if (strcmp(args[0], commands_str[i]) == 0) {
			return(*commands_func[i])();
		}
	}

	//Serial.println("Invalid command. Type \"help\" for more.");
	return 0;
}

int cmd_help() {
	Serial.println("The following commands are available:");

	for (int i = 0; i < num_commands; i++) {
		Serial.print("  ");
		Serial.println(commands_str[i]);
	}
	Serial.println("");
	Serial.println("You can for instance type \"help led\" for more info on the LED command.");
}

int cmd_test() {
	Serial.println(F("test command"));
}

