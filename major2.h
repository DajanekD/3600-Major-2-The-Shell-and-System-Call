/*  Group 10
    Group Members: Anika Chowdhury, Christopher Collins, Dajanek Davis, Joshua Thomas
    Project: Major Assignment 2 – The Shell and System Calls
    Course Section: CSCE 3600 Section 003
    Date: 11/23/21
    Description: This header file holds the necessary libraries and function definitions
*/

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>



void shell(int x, char *filename, char *prompt);
void execution(char *command);