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
int cmd_exit();

int (*commands_func[])() {
	&cmd_help,
	& cmd_exit
};

//List of command names
const char* commands_str[] = {
	"help",
	"exit"
};

int num_commands = sizeof(commands_str) / sizeof(char*);

void cli_init() {
	Serial.println("Welcome to this simple Arduino command line interface (CLI).");
	Serial.println("Type \"help\" to see a list of commands.");
}

void run_cli() {
	Serial.print("> ");

	read_line();
	if (!error_flag) {
		parse_line();
	}
	if (!error_flag) {
		execute();
	}

	memset(line, 0, LINE_BUF_SIZE);
	memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);

	error_flag = false;
}

void read_line() {
	String line_string;

	while (!Serial.available());

	if (Serial.available()) {
		line_string = Serial.readStringUntil("\n");
		if (line_string.length() < LINE_BUF_SIZE) {
			line_string.toCharArray(line, LINE_BUF_SIZE);
			Serial.println(line_string);
		}
		else {
			Serial.println("Input string too long.");
			error_flag = true;
		}
	}
}

void parse_line() {
	char* argument;
	int counter = 0;

	argument = strtok(line, " ");

	while ((argument != NULL)) {
		if (counter < MAX_NUM_ARGS) {
			if (strlen(argument) < ARG_BUF_SIZE) {
				strcpy(args[counter], argument);
				argument = strtok(NULL, " ");
				counter++;
			}
			else {
				Serial.println("Input string too long.");
				error_flag = true;
				break;
			}
		}
		else {
			break;
		}
	}
}

int execute() {
	for (int i = 0; i < num_commands; i++) {
		if (strcmp(args[0], commands_str[i]) == 0) {
			return(*commands_func[i])();
		}
	}

	Serial.println("Invalid command. Type \"help\" for more.");
	return 0;
}