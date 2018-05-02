/*
 *  Simple C program that simulates a Shell interface
 *
 *  Created on: Feb 18, 2018
 *      Author: tommy lin
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLINE 80  // command line length

int main(void) {

	char *args[MAXLINE / 2 + 1];   	 // maximum 40 args
	int should_run = 1;   		 // 1 for still running, 0 to exit program
	char userInputs[MAXLINE];    //storage for user storage
	printf("CS149 Shell from Tommy Lin\n");
	while (should_run != 0) {
		printf("Tommy-008>");
		fflush(stdout);

		//Step 1: get input from user

		fgets(userInputs, MAXLINE, stdin);

		int i = 0;
		char *p = strtok(userInputs, " ");
		while (p != NULL) {

			args[i] = p;
			//This gets rid of the new line created by fgets
			char *pos;
			if ((pos = strchr(args[i], '\n')) != NULL) {
				*pos = '\0';
			}

			p = strtok(NULL, " ");
			i++;
		}
		args[i] = NULL;  // last element is set to Null(0)

		//if the first args is exit, terminate
		if (strcmp(args[0], "exit") == 0) {
			should_run = 0;
			return 0;
		}



		//checks for & in the last slot of args, if true then background is set to true
		int background = 0;
		if (strcmp(args[i - 1], "&") == 0) {
			background = 1;
			args[i - 1] = NULL;
		}

		//Step 2: fork a child process with fork

		pid_t pid = fork();
		int status;
		if (pid < 0) {
			printf("Fork failed.\n");
			exit(1);
		}
		//Step 3: child process invokes execvp

		if (pid == 0) {
			if (execvp(args[0], args) == -1) {
				printf("Error Msg: Execute failed.\n");
			}
			exit(1);
		}

		//Step 4: if command has '&', parent doesn't wait

		else {
			if (background) {
                while(pid != wait(NULL)) // This makes sure other commands with "&" will not have parents               waiting also
                    printf(
						"pid: %d is running in the background. Parent doesn't wait.\n",
						pid);

			} else {
				wait(&status);
			}
		}
	}
	return 0;
}
