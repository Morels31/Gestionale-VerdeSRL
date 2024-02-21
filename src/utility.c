#include "common.h"
#include "utility.h"



/*
 *  Clears linux standard input buffer.
 *  (not the <stdio.h> stdin buffer)
 */

void clearStdin(void){
	struct pollfd pIn;												//initialize required argument for calling the poll function
	pIn.fd = 0;
	pIn.events = POLLIN;
	char buff[BUFF_LEN];
	int ret;
	while(1){														//loop until stdin is empty
		if((ret = poll(&pIn, 1, 0))==-1) error("poll() failed");	//call poll(check if stdin has available characters to read)
		if(!ret) break;												//if there aren't available characters in stdin, return
		if(read(0, buff, BUFF_LEN)==-1) error("read() failed");	//else read those characters and loop
	}
}



/*
 *  Reads a line from the linux standard input,
 *  copies the line in an newly allocated area, or,
 *  if 'optionalDest' is not NULL, in the area pointed by it. 
 *
 *  And, if the 'optionalTotChars' variable is not NULL,
 *  it saves the number of characters of the returned string,
 *  in the integer pointed by it.
 *  
 *	(the function doesn't crash if CTRL+D is entered)
 *  
 *    'askStr' = the prompt string that will be printed before the user input. es: "Enter text: ".
 *    'errStr' = the string that will be printed if the readed string is larger than 'maxLen'.
 *    'maxLen' = the maximum length of the string.
 *    'optionalDest' = a pointer to an optional already allocated char buffer.
 *    'optionalTotChars' = an optional integer pointer,
 *      where will be saved the number of characters in the returned string.
 *
 *    returns a pointer to the new string, or,
 *    returns NULL if input is empty.  
 */

char *readLine(char *askStr, char *errStr, int maxLen, char *optionalDest, size_t *optionalTotChars){
	ssize_t readed;
	char buff[maxLen+2];
	while(1){														//loop until string is valid
		printNow(askStr);															
		if((readed = read(0, buff, maxLen+1))<0) error("read() failed"); //read from standard input
		buff[readed] = '\0';										//set a null byte at the end of the string

		if(!readed || readed==1 && buff[0]=='\n'){					//if string is empty return NULL
			if(optionalTotChars) *optionalTotChars = 0;					
			return NULL;
		}
		if(buff[readed-1]=='\n'){									//if last char is newline, remove it
			buff[readed-1] = '\0';
			readed--;
		}
		if(readed<=maxLen) break;									//if string is shorter or equal to 'maxLen' exit loop
		printNow(errStr);
		clearStdin();												//else clear stdin and loop
	}

	char *resultStr;
	if(optionalDest) resultStr = optionalDest;						//if optionalDest is not NULL, save result string there
	else{															//else allocate space for result string
		resultStr = malloc((strlen(buff) + 1) * sizeof(char));
		if(!resultStr) error("malloc() failed");
	}
	
	if(optionalTotChars) *optionalTotChars = readed;
	strcpy(resultStr, buff);										//copy string(included \0) from buffer to str
	memset(buff, 0, (maxLen + 2) * sizeof(char));					//for security set all the characters of the buffer to 0
	return resultStr;

}



/*
 *  This variadic function takes in input multiple strings,
 *  and displays them like this:
 *
 *  'askStr'
 *      1: 'arg[1]'
 *      2: 'arg[2]'
 *      3: 'arg[3]'
 *      ...
 *
 *  And asks the user to select beetween the n options.
 *
 *    returns the selected number, or
 *    returns 0 if input is empty
 */


unsigned choice(char *askStr, ...){
	va_list argptr;
	unsigned index;
	char *str;
	char inputStr[10];
	unsigned long inputNum;
	char invalidStr[] = "\n\n\nInvalid Number.\n\n";

	while(42){

		va_start(argptr, askStr);
		printf("%s\n", askStr);

		index = 1;
		str = va_arg(argptr, char *);
		if(!str) return -1;	//palmface

		while (str) {
			printf("\t%u: %s\n", index++, str);
			str = va_arg(argptr, char *);
		}

		va_end(argptr);
		fflush(stdout);

		if(!readLine("", invalidStr, 9, inputStr, NULL)) return 0;

		errno = 0;
		inputNum = strtoul(inputStr, NULL, 10);
		if(errno) error("strtoul() failed");

		if(inputNum > 0 && inputNum < index && inputNum < UINT_MAX) return (unsigned) inputNum;

		printNow(invalidStr);
	}
}


/*
 *  Reads an username from the user, checks its validity,
 *  and then, if its valid, saves it in the already allocated 'dest' char buffer.
 *  (that has to be at least MAX_USERNAME_LEN+1 chars)
 *
 *    'askStr' = the prompt string that will be printed before the user input. es: "Enter password: ".
 *    'dest' = a pointer to an optional already allocated char buffer, long at least MAX_USERNAME_LEN+1.
 */

void readUsername(char *askStr, char *dest){
	char longStrErr[] = "The entered username is too long.\n";
	char emptyStrErr[] = "Username can't be empty!";

	while(1){
		while(!readLine(askStr, longStrErr, MAX_USERNAME_LEN, dest, NULL)) printf("\n%s\n", emptyStrErr);
		if(!checkUsername(dest)) break;
		printf("The entered username is invalid. (has to contain only alphanumeric chars or any of %s)\n", USERNAME_CHARSET);
		fflush(stdout);
	}
}



/*
 *  Disables the input echo of the terminal, reads a password,
 *  checks it's validity, and saves it in the already allocated 'dest' char buffer,
 *  (that has to be at least MAX_PASSWORD_LEN+1 chars)
 *  finally re-enables the echo of the terminal.
 *
 *  (disabling terminal input echo found on: https://stackoverflow.com/questions/1786532/c-command-line-password-input)
 *
 *    'askStr' = the prompt string that will be printed before the user input. es: "Enter password: ".
 *    'dest' = a pointer to an optional already allocated char buffer, long at least MAX_PASSWORD_LEN+1.
 */

void readPassword(char *askStr, char *dest){
	char longStrErr[] = "The entered password is too long.\n";
	char emptyStrErr[] = "Password can't be empty!";
	static struct termios oldt, newt;

	tcgetattr( STDIN_FILENO, &oldt);								//saving the old settings of STDIN_FILENO and copy settings for restoring
	newt = oldt;
	newt.c_lflag &= ~(ECHO);										//setting the approriate bit in the termios struct
	tcsetattr( STDIN_FILENO, TCSANOW, &newt);						//enabling new STDIN_FILENO settings -> disable input echo

	while(1){
		while(!readLine(askStr, longStrErr, MAX_PASSWORD_LEN, dest, NULL)) printf("\n%s\n", emptyStrErr);
		printNow("\n");
		if(!checkPassword(dest)) break;
		printf("The entered password is invalid. (has to be at least %d chars, and has to contain only alphanumeric chars or any of %s)\n", MIN_PASSWORD_LEN, PASSWORD_CHARSET);
		fflush(stdout);
	}

	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);						//resetting old STDIN_FILENO settings -> re-enable input echo
}





void readPKSpecie(char *nomeLatino, char *colore, size_t *sizeNomeLatino, size_t *sizeColore){

	while(!readLine("Insert specie's latin name: ", "String too long.\n\n", MAX_NOME_LATINO_LEN, nomeLatino, sizeNomeLatino)) printf("The name can't be empty.\n\n");

	if(!readLine("Insert color (Empty for green plants): ", "String too long.\n\n", MAX_COLORE_LEN, colore, sizeColore)) colore[0] = '\0';
}


void readInt(char *askStr, int *dest){
	char buff[BUFF_LEN];

	while(1){
		while(!readLine(askStr, "String too long.\n\n", BUFF_LEN-1, buff, NULL)) printf("The input can't be empty.\n\n");

		*dest = atoi(buff);
		if(*dest) break;
		printf("An invalid value or 0 has been inserted, retry..\n\n");
	}
}






/*
 *  Checks if 'str' is a valid string:
 *  (it's not empty, has <= 'maxSize' chars,
 *  and contains only alphanumeric characters,
 *  or any of the characters in the string pointed by 'charset').
 *
 *    'str' = string to check.
 *    'charset' = string containing the additional valid characters,
 *      other than the alphanumeric ones.
 *    'maxSize' = the maximum valid size of the string.
 *
 *    returns 0 if 'str' is valid, else
 *    returns 1
 */

int checkGenericString(char *str, const char *charset, size_t maxSize){
	if(!str) error("NULL argument");
	size_t l = strlen(str);
	if(!l || l>maxSize) return 1;

	char *p;
	for(size_t i=0; i<l; i++){
		if(!isalnum(str[i])){
			p = (char *) charset;
			while(*p!='\0' && str[i]!=*p) p++;
			if(*p=='\0') return 1;
		}
	}
	return 0;
}



/*
 *  Check if 'username' is a valid username:
 *  (it's not empty, has <= MAX_USERNAME_LEN chars,
 *  and contains only alphanumeric characters, - or _)
 *
 *    'username' = string to check.
 *
 *	  returns 0 if 'username' is valid, else
 *    returns 1
 */

int checkUsername(char *username){
	return checkGenericString(username, USERNAME_CHARSET, MAX_USERNAME_LEN);
}



/*
 *  Check if 'psw' is a valid password:
 *  (it's not empty, has <= MAX_PASSWORD_LEN and >= MIN_PASSWORD_LEN chars,
 *  and contains only alphanumeric characters, or any of the characters in the PASSWORD_CHARSET string).
 *
 *    'password' = string to check.
 *
 *	  returns 0 if 'password' is valid, else
 *    returns 1
 */

int checkPassword(char *psw){
	if(!psw) error("NULL argument");
	if(strlen(psw)<MIN_PASSWORD_LEN) return 1;
	return checkGenericString(psw, PASSWORD_CHARSET, MAX_PASSWORD_LEN);
}



/*
 *  Check if 'hostname' is a valid hostname:
 *  (it's not empty, has <= MAX_HOSTNAME_LEN chars,
 *  begins with an alphanumeric character,
 *  and contains only alphanumeric characters, - or _ or .)
 *
 *    'hostname' = string to check.
 *
 *	  returns 0 if 'hostname' is valid, else
 *    returns 1
 */

int checkHostname(char *hostname){
	if(!hostname) error("NULL argument");
	if(hostname[0]=='-' || hostname[0]=='_' || hostname[0]=='.') return 1;
	return checkGenericString(hostname, HOSTNAME_CHARSET, MAX_HOSTNAME_LEN);
}



/*
 *  Check if 'dbName' is a valid database name:
 *  (it's not empty, has <= MAX_DB_NAME_LEN chars,
 *  and contains only alphanumeric characters, _ or $)
 *
 *    'dbName' = string to check.
 *
 *	  returns 0 if 'dbName' is valid, else
 *    returns 1
 */

int checkDBName(char *dbName){
	return checkGenericString(dbName, DB_NAME_CHARSET, MAX_DB_NAME_LEN);
}



/*
 *  Check if 'port' is a valid port number.
 *  (is beetween 0 and 65535)
 *
 *    'port' = the number to check
 *
 *	  returns 0 if 'port' is valid, else
 *    returns 1
 */

int checkPort(unsigned port){
	return port == 0 || port > 65535;
}



/*
 *  Parses the command line arguments.
 *  (if the same option are given multiple times, only the last will be used)
 *
 *    'argc' = the main argc variable.
 *    'argv' = the main argv variable.
 *    'dbInfo' = pointer to a struct where the info will be saved
 *
 *    returns the number of parsed options
 */

unsigned parseCmdLine(int argc, char **argv, dbI *dbInfo){

	unsigned parsed = 0;
	for(int i=1; i<argc; i++){
		if(argv[i][0]!='-' || argv[i][0]=='\0') continue;
		parsed++;
		switch(argv[i][1]){
			case 'h':
				if(i+1>=argc || checkHostname(argv[i+1])) err("invalid hostname");
				strcpy(dbInfo->hostname, argv[i+1]);
				break;

			case 'p':
				if(i+1>=argc) err("invalid port number");
				errno = 0;
				unsigned long port = strtoul(argv[i+1], NULL, 10);
				if(errno) error("strtoul() failed");

				if(checkPort(port)) err("invalid port number");
				dbInfo->port = port; 
				break;

			case 'n':
				if(i+1>=argc || checkDBName(argv[i+1])) err("invalid DB name");
				strcpy(dbInfo->dbName, argv[i+1]);
				break;

			case 'e':
				printf("%s\n", (char[8]){67,108,97,117,100,105,111,0});
				fflush(stdout);
				exit(3);

			case '?':
				printf("Options:\n\t-h (hostname)\n\t-p (port)\n\t-n (database name)\n\t-? display this help and exit\n");
				exit(0);

			default:
			invalid:
				printf("Invalid option '%s', use -? for help.\n", argv[i]);
				exit(1);
		}
		if(argv[i][2]!='\0') goto invalid;
	}
	return parsed;
}
