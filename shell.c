//Kevin Kell
//Distributed Systems Project2 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include "shell.h"

int shell(char *input) {

	char *parsed_input[count_arguments(input) + 1];
	parse_arguments(input, parsed_input);

	if(strcmp("exit", *parsed_input) == 0) {
		wait_for_all_children();
		exit(0);
	}

	if(strcmp("cd", *parsed_input) == 0) {
		cd(*(parsed_input + 1));
	}

	int pid = fork();

	if(pid < 0) {
		perror("fork");
		exit(1);
	}
	if(pid > 0) {
		wait(NULL);
	}
	else {
		if(execvp(*parsed_input, parsed_input) == -1) {
			perror("execv");
			exit(1);
		}
	}

	return 0;
}

int count_arguments(char *input) {
	int count = 0;
	char *current;

	current= input;
	while(*current == ' ') {
		current++;
	}

	while(*current != '\0') {
		if(*current == ' ') {
			current++;
		}
		else {
			count++;
			current++;
			while(*current != ' ' && *current != '\0') {
				current++;
			}
		}
	}

	return count;
}

void parse_arguments(char *input, char **parsed_input) {
	char **current;
	char *saveptr;
	char *token;

	current = parsed_input;
	token = strtok_r(input, "\n ", &saveptr);

	if(strlen(token) > 0) {
		*current = token;
		current++;
	}

	while((token = strtok_r(NULL, " \n", &saveptr)) != NULL) {
		if(strlen(token) > 0) {
			*current = token;
			current++;
		}	
	}

	*current = NULL;
}

void wait_for_all_children() {
	int pid;
	while((pid = wait(NULL))) {
		if(errno == ECHILD) {
			break;
		}
	}
}

void cd(char *path) {
	char *new_path = path;
	char buffer[strlen(path) + strlen(getenv("HOME")) + 1];

	if(*new_path == '~') {
		strcpy(buffer, getenv("HOME"));
		strcat(buffer, (path + 1));
		new_path = buffer;
	}

	if(chdir(new_path) != 0) {
		perror("chdir");
		wait_for_all_children();
		exit(1);
	}
}
