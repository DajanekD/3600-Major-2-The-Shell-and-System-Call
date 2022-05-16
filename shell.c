/*  Group 10
    Group Members: Anika Chowdhury, Christopher Collins, Dajanek Davis, Joshua Thomas
    Project: Major Assignment 2 – The Shell and System Calls
    Course Section: CSCE 3600 Section 003
    Date: 11/23/21
    Description: This file holds the functionality for our shell program. Here we have the interactive and batch modes as well as an execution function to execute the commands. 
*/

#include "major2.h"
#include <errno.h>
#include <signal.h>
/*   built in Path Command
#include <stdio.h>
#include <conio.h>
#include <string.h>
#define buffersize 1024
#define CMDLINE_LEN 1024
#define path 512
#define CMD_LEN 50


     int s = 0;
     int FALSE = 0;
     int TRUE = 0;
     typedef enum {CMD,cd} cmdt;
     char cd1[100], PATH[512];
     char cd2[path];
     char *flag;
     
     
int Execute_Command (char *cmd, int x, char **y)
  {
     char cmdPath[path];
    
     printf ("cmd: %s\n", cmd);
     printf ("New_PATH: %s\n", PATH);
    if (cmd[0] == '/')
     strcpy(cmdPath, cmd);
    else if (cmd[0] == '.')
   {
    if (realpath(cmd, cmdPath) == NULL)
 { 
     fprintf(stderr, "%s: '%s'\n", flag, cmd);
     return 1;
 }
   }
   else
  {
     strcpy(cmdPath, PATH);
     strcat(cmdPath, "/");
     strcat(cmdPath, cmd);
  }
     printf("cmdPath: %s\n", cmdPath);

     if (access(cmdPath) != 0)
 {
     fprintf(stderr, "%s: '%s'", flag, cmd);
     return 1;
 }
 else
   {
     return -1;
   }
}
void changeDirectory(char *target)
  {
     char tempPath[path];
     char tempDir[path];
    if (target[0] == '/')
     strcpy(tempPath, target);
   else
  {
   strcpy(tempPath, cd2);
   strcat(tempPath, "/");
   strcat(tempPath, target);
  }
    if (realpath(tempPath, tempDir) == NULL)
 {
     fprintf(stderr, "%s: cd: '%s': %s\n", flag, tempPath);
     return;
 }
    else
  {
     if (chdir(tempDir) == -1)
 {
     perror("chdir");
     return;
 }
     strcpy(cd2, tempDir);
  }
 }
void CurrentDirectory()
  {
     char tempPath[path];
    if (getcwd(tempPath, path) == NULL)
     perror("getcwd");
    else
     printf("%s\n", tempPath);
  }
int main(int x, char **y, char **z)
  {
     char cmdline[CMDLINE_LEN];
     char cmd[CMD_LEN];
     char **cmd_y = NULL;
     int cmd_x;
     int i;
     int closeShell = FALSE;
     char tempPath[path];
      flag = y[0];
      init();
      
showWelcomeScreen();
      while (closeShell != TRUE)
  {
     showPrompt();
     gets(cmdline);
     parseCmdline(cmdline, cmd, &cmd_y, &cmd_x);
      printf("\n");
    for (i = 0; i < cmd_x; i++)
      printf("arg[%d] = %s\n", i, cmd_y[i]);
      printf("\n");
      switch(getCmdType(cmd))
 {
   case cd:
     changeDirectory(cmd_y[1]);
     break;

   case CMD:
     s = ExecuteCommand(cmd, cmd_x, cmd_y);
     break;
 } 
     freeCmdArgs(&cmd_y, &cmd_x);
  }
     return 0;
}
 */

void processcdCommand(char*);
void pipes(char**);
void exce1(char*);
void exce2(char*);
void exce3(char*);

#define buffer_size 1024


int numOfCommands = 0; // the number of elements in recentCommands
char *recentCommands[20]; // holds the most recent 20 commands that have been executed

char *aliasCommands[20]; // holds list of aliases for commands related to list of real commands

char *realCommands[20]; // holds list of real commands related to the list of aliases


int pid;
int pipe1[2];//Pipes
int pipe2[2];


/* Function Name: shell
    Parameters: int for what mode to go into, char* for the name of the batch file, char* for what prompt to use
    Return: NONE
    Description: This function decides what mode of shell to go into and calls for execution of commands line by line
*/

void shell(int x, char *filename, char *prompt)
{

	for (int s = 0; s < 20; s++) // initializing aliasCommands with NULL
	{
        	aliasCommands[s] = NULL;
	}

	for (int t = 0; t < 20; t++) // initializing realCommands with NULL
	{
        	realCommands[t] = NULL;
	}
	
	if(x == 1) // Interactive Mode

	{
		printf("Activating Interactive Mode...\n");
		char u_input[buffer_size]; // holds input from u		while(1)
		while (1)
		{
          		printf("%s: ", prompt);
		  	fgets(u_input, buffer_size, stdin); //get command 
			
			if (strstr(u_input, "newshell ") != NULL) // if newshell is called it is probably wanting to switch to batch mode
			{
				x = 2; // go to batch mode once this if block is finished

				const char b[3] = " \n"; // holds the characters to use as delimeters for strtok
				char *token; // will be used with strtok to parse newshell
				char* Array[3]; // holds newshell command parsed
				int counter3 = 0; // num of elements in Array
				
				//This line through the while loop is the parsing process for the line of input
				token = strtok(u_input, b);

				while (token != NULL)
				{
					Array[counter3] = malloc(strlen(token) + 1);
					
					if (Array[counter3] == NULL)
					{
						perror("Error");
						exit(0);
					}


					strcpy(Array[counter3], token);
					token = strtok(NULL, b);
					counter3++;
				}

				strcpy(filename, Array[1]); // copy the name of the file given with newshell to filename to be used in batch mode

				break; // break from while loop to go to batch mode
			}

			// The following if and else statements update the recentCommands array to hold the most recent command to be executed
			if (numOfCommands > 19) // if the recentCommands array is filled then we must delete the oldest command (the first command in the array) and shift all the elements forward.
			{
				for (int l = 0; l < 19; l++) // deleting oldest command and shifting all elements
				{
					free(recentCommands[l]);
					recentCommands[l] = malloc(strlen(recentCommands[l+1]) + 1);
					
					if (recentCommands[l] == NULL)
                                        {
                                                perror("Error");
                                                exit(0);
                                        }

					strcpy(recentCommands[l], recentCommands[l+1]);
				}

				// adding new command(s) to array
				free(recentCommands[19]);
				recentCommands[19] = malloc(strlen(u_input) + 1);
				
				if (recentCommands[19] == NULL)
                                {
                                	perror("Error");
                                        exit(0);
                                }

				strcpy(recentCommands[19], u_input);
			}

			else // if the array is not filled, add the new command to the next available slot in the array
			{
				recentCommands[numOfCommands] = malloc(strlen(u_input) + 1);
				
				if (recentCommands[numOfCommands] == NULL)
				{
					perror("Error");
					exit(0);
				}

				strcpy(recentCommands[numOfCommands], u_input);
				numOfCommands++; // update the number of elements in the array
			}

		  	execution(u_input); // execute command
		}

	
	}
	
	if(x == 2) // batch mode
	{
		printf("Activating Batch Mode...\n");
		FILE *fp; // pointer to file that will be opened
		char line[513]; // holds the line from input
		fp = fopen(filename, "r"); // opens the file with read permission

		if (fp == NULL)
		{
			perror("Failed to open batch file");
			exit(0);
		}


		// while it is not the end of the file do the following
		while (fgets(line, 512, fp) != NULL) // read in a line from the file
		{
			int size = strlen(line); // size of the line

			// These if/else statements print out the command(s) to be executed
			if (line[size-1] == '\n')
			{
				printf("Command(s) to execute: %s", line);
			}

			else
			{
				printf("Command(s) to execute: %s\n", line);
			}

			// The following if and else statements update the recentCommands array to hold the most recent command to be executed
			if (numOfCommands > 19) // if the recentCommands array is filled then we must delete the oldest command (the first command in the array) and shift all the elements forward.
			{
				for (int l = 0; l < 19; l++) // deleting oldest command and shifting all elements
				{
					free(recentCommands[l]);
					recentCommands[l] = malloc(strlen(recentCommands[l+1]) + 1);
					
					if (recentCommands[l] == NULL) // checking whether memory is allocated
                                        {
                                                perror("Error");
                                                exit(0);
                                        }


					strcpy(recentCommands[l], recentCommands[l+1]);
				}

				// adding new command(s) to array
				free(recentCommands[19]);
				recentCommands[19] = malloc(strlen(line) + 1);
				
				if (recentCommands[19] == NULL) // checking whether memory is allocated
				{
					perror("Error");
					exit(0);
				}

				
				strcpy(recentCommands[19], line);
			}

			else // if the array is not filled, add the new command to the next available slot in the array
			{
				recentCommands[numOfCommands] = malloc(strlen(line) + 1);
				
				if (recentCommands[numOfCommands] == NULL) // checking whether memory is allocated
				{
					perror("Error");
					exit(0);
				}


				strcpy(recentCommands[numOfCommands], line);
				numOfCommands++; // update the number of elements in the array
			}

			execution(line); // calling for execution of the line of input
		}
    	fclose(fp); // close the pointer to the file
		printf("Finished reading file. Thank you for using our shell program!\n");
		
	}
	
	else
	{
	printf("Sorry, but that input is invalid");
	}

    	// This for loop free up memory used by recentCommands
	for (int i = 0; i < numOfCommands; i++)
	{
		free(recentCommands[i]);
	}

	for (int u = 0; u < 20; u++) // This for loop frees up memory used by aliasCommands
	{
		if (aliasCommands[u] != NULL)
		{
			free(aliasCommands[u]);
		}
	}

	for (int v = 0; v < 20; v++) // This for loop frees up memory used by realCommands
	{
		if (realCommands[v] != NULL)
		{
			free(realCommands[v]);
		}
	}	

}

/* Function Name: execution
    Parameters: char* for the line of commands to execute
    Return: NONE
    Description: This function parses the given line of commands and executes each command in sequence. It also houses the implementation for the built in commands.
*/

void execution(char* line)
{
	const char a[3] = ";\n"; // holds the characters to use as delimeters for strtok
	char *token; // will be used with strtok to parse the line
	char* lineArray[100]; // will hold each command after parsing in a separate slot
	int counter = 0; // keeps track of how many elements are in lineArray (i.e. how many commands are stored in lineArray)
	
	//This line through the while loop is the parsing process for the line of input
	
	token = strtok(line, a);
	while (token != NULL)
	{
		lineArray[counter] = malloc(strlen(token) + 1);
		
		if (lineArray[counter] == NULL) // checking whether memory is allocated
		{
			perror("Error");
			exit(0);
		}
		
		strcpy(lineArray[counter], token);
		token = strtok(NULL, a);
		counter++;
	}	
	//printf("execution counter %d\n", counter);
	// Now that the line is parsed we will iterate over lineArray and execute each command
	for (int j = 0; j < counter; j++)
	{
//printf("execution j %d\n", counter);
		char* command = lineArray[j]; // The command to be executed
		char* commandArray[20]; // this array holds the pieces of the command such as the command and any options that go with it. They each go in a different slot.
		int counter2 = 0; // holds the number of elements in the commandArray

		if (strstr(command, "=") != NULL) // This if block parses alias assignment commands
		{
			char word[7] = "";
			
			int q = 0;
			char c = command[q];
			while (c != ' ')
			{
				strcat(word, &c);
				q++;
				c = command[q];
			}
			char word2[10] = "";
			q++;
			c = command[q];
			while (c != '=')
			{
				strcat(word2, &c);
				q++;
				c = command[q];
			}

			char word3[20] = "";
			q++;
			q++;
			c = command[q];
			while (c != '\'')
			{
				strcat(word3, &c);
				q++;
				c = command[q];
			}

			commandArray[counter2] = malloc(strlen(word) + 1);
			
			if (commandArray[counter2] == NULL) // checking whether memory is allocated
			{
				perror("Error");
				exit(0);
			} 

			strcpy(commandArray[counter2], word);
			counter2++;

			commandArray[counter2] = malloc(strlen(word2) + 1);
			
			if (commandArray[counter2] == NULL) // checking whether memory is allocated
                        {
                                perror("Error");
                                exit(0);
                        }

			strcpy(commandArray[counter2], word2);
			counter2++;

			commandArray[counter2] = malloc(strlen(word3) + 1);
			
			if (commandArray[counter2] == NULL) // checking whether memory is allocated
                        {
                                perror("Error");
                                exit(0);
                        }

			strcpy(commandArray[counter2], word3);
			counter2++;
			
			commandArray[counter2] = NULL; // Adding NULL to end of commandArray for execvp
			counter2++; // incrementing counter for num of elements in commandArray
		}
		else if (strstr(command, "|") != NULL) // This if block parses | commands
		{
			int pipecounter = 0;
			const char *tmp = command;
			while(tmp = strstr(tmp, "|"))
			{
   			pipecounter++;
   			tmp++;
			}
			//printf("pipecounter value %d\n", pipecounter);
			
			//printf("Made it to pipe 1 %s\n", command);
			char word[7] = "";
			int q = 0;
			char c = command[q];
			while (c != ' ')
			{
				//printf("command[q]: %c and this is q: %d\n", command[q], q);
				strcat(word, &c);
				q++;
				c = command[q];
			}
			char word2[10] = "";
			q++;
			q++;
			q++;
			
			c = command[q];
			//printf("this is what c0 is %c\n", c);
			while (c != '|' && c != ' ' && c != '\000')
			{
				strcat(word2, &c);
				q++;
				c = command[q];
			}

			char word3[20] = "";
			q++;
			q++;
			q++;
			c = command[q];
			  //printf("this is what c1 is %c\n", c);
			while (pipecounter == 2 && c != '|' && c != ' ' && c != '\000')
			{
				
			//	printf("word 3 while loop: %c\n", c);
				strcat(word3, &c);
			//	printf("word 3 while loop: %s\n", word3);
				q++;
				c = command[q];
			}
			
			
			/*char word4[20] = "";
                        q++;
                        q++;
			q++;
                        c = command[q];
			printf("this is what c2 is %c\n", c);
                        while (c != '|' && c != ' ' && c != '\000')
                        {
                                printf("word 4 while loop: %c\n", c);
                                strcat(word4, &c);
                                printf("word 4 while loop: %s\n", word4);
                                q++;
                                c = command[q];
                        }*/
			


			commandArray[counter2] = malloc(strlen("|") + 1);
                        strcpy(commandArray[counter2], "|");
                        counter2++;

			commandArray[counter2] = malloc(strlen(word) + 1);
			strcpy(commandArray[counter2], word);
			counter2++;
			commandArray[counter2] = malloc(strlen(word2) + 1);
			
			strcpy(commandArray[counter2], word2);
			counter2++;

			commandArray[counter2] = malloc(strlen(word3) + 1);
			strcpy(commandArray[counter2], word3);
			counter2++;
			

			//commandArray[counter2] = malloc(strlen(word4) + 1);
                        //strcpy(commandArray[counter2], word4);
                        //counter2++;



			commandArray[counter2] = NULL; // Adding NULL to end of commandArray for execvp
			counter2++; // incrementing counter for num of elements in commandArray
			//printf("this is commandArray %s\n", commandArray[0]);
			//printf("this is commandArray[1] %s\n", commandArray[1]);
			//printf("this is commandArray[2] %s\n", commandArray[2]);
			//printf("this is commandArray[3] %s\n", commandArray[3]);
			//printf("this is commandArray[4] %s\n", commandArray[4]);
                        //printf("this is commandArray[5] %s\n", commandArray[5]);


			}
			// Dajanek: For redirection
			/*	if ((strstr(command, "<") != NULL)||(strstr(command, ">") != NULL)) // This if block parses | commands
		{
			int argcounter = 0;
			const char *tmp = command;
			const char *tmp1 = command;
			while(tmp = strstr(tmp, "<"))
			{
   			argcounter++;
   			tmp++;
			}
			while(tmp1 = strstr(tmp, ">"))
			{
			    argcounter++;
			    tmp1++;
			}
			printf("argcounter value %d\n", argcounter);
			
			printf("Made it to pipe 1 %s\n", command);
			char word[7] = "";
			int q = 0;
			char c = command[q];
			while (c != ' ')
			{
				printf("command[q]: %c and this is q: %d\n", command[q], q);
				strcat(word, &c);
				q++;
				c = command[q];
			}
			char word2[10] = "";
			q++;
			q++;
			q++;
			
			c = command[q];
			printf("this is what c0 is %c\n", c);
			while (c != '<' && c != ' ' && c != '\000' && c != '>')
			{
				strcat(word2, &c);
				q++;
				c = command[q];
			}

			char word3[20] = "";
			q++;
			q++;
			q++;
			c = command[q];
			  printf("this is what c1 is %c\n", c);
			while (argcounter == 2 && c != '<' && c != ' ' && c != '\000' && c != '>')
			{
				
				printf("word 3 while loop: %c\n", c);
				strcat(word3, &c);
				printf("word 3 while loop: %s\n", word3);
				q++;
				c = command[q];
			}
		} */
		else if (strstr(command, "alias") != NULL) // this else if block parses regular alias commands
		{
			// This through the while loop is the parsing process for the command into its individual pieces.
                        token = strtok(command, " ");

                        while (token != NULL)
                        {
                                commandArray[counter2] = malloc(strlen(token) + 1);
                                
				if (commandArray[counter2] == NULL) // checking whether memory is allocated
				{
					perror("Error");
					exit(0);
				}

				strcpy(commandArray[counter2], token);
                                token = strtok(NULL, " ");
                                counter2++;
                        }

                        commandArray[counter2] = NULL; // Adding NULL to end of commandArray for execvp
                        counter2++; // incrementing counter for num of elements in commandArray
		}
		else if (strstr(command, "|") != NULL) // this else if block parses pipe commands
		{
			//printf("Im here pipe 2");
			// This through the while loop is the parsing process for the command into its individual pieces.
                        token = strtok(command, " ");

                        while (token != NULL)
                        {
                                commandArray[counter2] = malloc(strlen(token) + 1);
                                
				if (commandArray[counter2] == NULL) // checking whether memory is allocated
				{
					perror("Error");
					exit(0);
				}

				strcpy(commandArray[counter2], token);
                                token = strtok(NULL, " ");
                                counter2++;
                        }

                        commandArray[counter2] = NULL; // Adding NULL to end of commandArray for execvp
                        counter2++; // incrementing counter for num of elements in commandArray
		}
		// Dajanek: for redirection
		/*	else if((strstr(command, "<") != NULL) || (strstr(command, ">") != NULL))
		{
		    token = strtok(command, " ");

                        while (token != NULL)
                        {
                                commandArray[counter2] = malloc(strlen(token) + 1);
                                strcpy(commandArray[counter2], token);
                                token = strtok(NULL, " ");
                                counter2++;
                        }

                        commandArray[counter2] = NULL; // Adding NULL to end of commandArray for execvp
                        counter2++;
		} */
		else // this else block parse all other kinds of commands
		{
			// This through the while loop is the parsing process for the command into its individual pieces.
			token = strtok(command, " ");

			while (token != NULL)
			{
				commandArray[counter2] = malloc(strlen(token) + 1);
				
				if (commandArray[counter2] == NULL) // checking whether memory is allocated
				{
					perror("Error");
					exit(0);
				}
				
				strcpy(commandArray[counter2], token);
				token = strtok(NULL, " ");
				counter2++;
			}

			commandArray[counter2] = NULL; // Adding NULL to end of commandArray for execvp
			counter2++; // incrementing counter for num of elements in commandArray
		}

		int choiceIndex = -1; // decides whether to activate else if statement to execute aliased commands
                for (int w = 0; w < 20; w++)// if the command matches an aliased command we set choiceIndex to the index where the aliased command was found
                {
			if (aliasCommands[w] != NULL)
			{
                        	if (strcmp(commandArray[0], aliasCommands[w]) == 0)
                        	{
                                	choiceIndex = w;
                                	break;
                        	}
			}
                }

		// if the command is cd, follow this implementation
		if (strcmp(commandArray[0], "cd") == 0)
		{
		//printf("This is cd: %s",commandArray[1]);
		processcdCommand(commandArray[1]);
		
		}
		else if (strcmp(commandArray[0], "|") == 0)
                {
                //printf("This is pipe: %s\n",commandArray[0]);
		pipes(&commandArray[1]);
                //processcdCommand(commandArray[1]);

                }


		// if the command is exit, follow this implementation
		else if (strcmp(commandArray[0], "exit") == 0)
		{
		    for (int index = j+1; index <= counter; index++)
	          {
	              char* commd = lineArray[index]; // The command to be executed
	              if(lineArray[index] != NULL)
	              {
	              lineArray[index];
		          execution(line);
	              }
	              else 
	              exit(0);
		}
		}
		// This is redirection
		/*	else if ((strcmp(commandArray[1], "<") == 0)||(strcmp(commandArray[1], ">")))
		        if(*commandArray[indx] == '>' && commandArray[indx+1])
                {
                  char* filename = strtok(NULL, " ");
                  int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			        if (fd < 0)
                    {
                      printf("Couldn't run\n");
                      exit(0);
                    }
                    dup2(fd, 1);
                    close(fd);
                    execvp(commandArray[0], commandArray); //execute the cmd
                    printf("%s: command not found\n",commandArray[0]);
                    exit(0);
			    }
			    else 
			   {
			       char* filename = strtok(NULL, " ");
			       printf("Filename for input redirection %s: \n",filename);
                   int fd2 = open(filename, O_RDONLY);
                //bad pipe
               if (fd2 < 0)
                {
                    printf("Couldn't run\n");
                    exit(0);
                }
                  dup2(fd2,0);
                  close(fd2);
                  execvp(commandArray[0], commandArray);
                  printf("%s: command not found\n",commandArray[0]);
                  exit(0);
			   }
               char* filename = strtok(NULL, " ");
			   free(filename);
	*/

		// if the command is path, follow this implementation
		else if (strcmp(commandArray[0], "path") == 0)
		{
      		/*	 
                         printf("%s", PATH);
                          else
                          printf("%s", commandArray[0]) ;
                           printf(" ");
                       }  

                        printf("\n"); 
	        */  
		}

		// if the command is alias, follow this implementation
		else if (strcmp(commandArray[0], "alias") == 0)
		{
			if (counter2 == 2) // just alias
			{
				printf("List of Aliased Commands:\n");

				for (int r = 0; r < 20; r++)
				{
					if (aliasCommands[r] != NULL)
					{
						printf("Alias: %s\tReal Command: %s\n", aliasCommands[r], realCommands[r]);
					}
					//printf("commandArray[%d] = %s\n", r, commandArray[r]);
				}
			}

			else if (counter2 == 3) // alias -c
			{
				if (strcmp(commandArray[1], "-c") == 0)
				{
					for (int r = 0; r < 20; r++)
					{
						free(aliasCommands[r]);
						free(realCommands[r]);
						aliasCommands[r] = NULL;
						realCommands[r] = NULL;
					}
				}
			}

			else if (counter2 == 4) // alias assignment and deletion
			{
				if (strcmp(commandArray[1], "-r") == 0) // alias deletion
				{
					for (int r = 0; r < 20; r++)
					{
						if (strcmp(commandArray[2], aliasCommands[r]) == 0)
						{
							free(aliasCommands[r]);
							free(realCommands[r]);
							aliasCommands[r] = NULL;
							realCommands[r] = NULL;
							break;
						}
					}
				}

				else // alias assignment
				{
					for (int r = 0; r < 20; r++) 
					{	
						if (aliasCommands[r] == NULL)
						{
							aliasCommands[r] = malloc(strlen(commandArray[1]) + 1);
							if (aliasCommands[r] == NULL) // checking whether memory is allocated
							{
								perror("Error");
								exit(0);
							}
							strcpy(aliasCommands[r], commandArray[1]);

							realCommands[r] = malloc(strlen(commandArray[2]) + 1);
							
							if (realCommands[r] == NULL) // checking whether memory is allocated
                                                        {
                                                                perror("Error");
                                                                exit(0);
                                                        }

							strcpy(realCommands[r], commandArray[2]);
							
							break;
						}	
					}
					
				}
			}

			else
			{
				fprintf(stderr, "That command is invalid.\n");
			}	
		}// else if alias

		// if the command is myhistory, follow this implementation
		else if (strcmp(commandArray[0], "myhistory") == 0)
		{
			if (counter2 == 2) // if there is just one argument, print out the list of recent commands
			{
				printf("Here is a list of your recent commands: \n");

				for (int m = 0; m < numOfCommands; m++)
				{
					printf("%d: %s\n", m+1, recentCommands[m]);
				}
			}

			else if (counter2 == 3) // if there are two arguments, specifically -c as the second argument, then delete all entries in recentCommands
			{
				if (strcmp(commandArray[1], "-c") == 0)
				{
					for (int o = 0; o < numOfCommands; o++)
					{
						free(recentCommands[o]);
					}

					numOfCommands = 0;
					printf("Command History Cleared\n");
				}
			}

			else if (counter2 == 4) // if there are 3 arguments to myhistory, specifically -e as the second argument, then execute the numbered command from recentCommands.
			{
				if (strcmp(commandArray[1], "-e") == 0)
				{
					int index = atoi(commandArray[2]) - 1;
					printf("Executing following command again: %s", recentCommands[index]);
					char* repeatCommand = malloc(strlen(recentCommands[index]) + 1);
					
					if (repeatCommand == NULL) // checking whether memory is allocated
					{
						perror("Error");
						exit(0);
					}

					strcpy(repeatCommand, recentCommands[index]);
					
					// This if/else block updates recentCommands with the repeated command
					if (numOfCommands > 19)
					{
						for (int l = 0; l < 19; l++)
						{
							free(recentCommands[l]);
							recentCommands[l] = malloc(strlen(recentCommands[l+1]) + 1);
							
							if (recentCommands[l] == NULL) // checking whether memory is allocated
							{
								perror("Error");
								exit(0);
							}

							strcpy(recentCommands[l], recentCommands[l+1]);
						}

						free(recentCommands[19]);
						recentCommands[19] = malloc(strlen(repeatCommand) + 1);
						
						if (recentCommands[19] == NULL) // checking whether memory is allocated
						{
							perror("Error");
							exit(0);
						}

						strcpy(recentCommands[19], repeatCommand);
					}

					else
					{
						recentCommands[numOfCommands] = malloc(strlen(repeatCommand) + 1);
						
						if (recentCommands[numOfCommands] == NULL) // checking whether memory is allocated
						{
							perror("Error");
							exit(0);
						}

						strcpy(recentCommands[numOfCommands], repeatCommand);
						numOfCommands++;
					}
					
					execution(repeatCommand); // execute the repeated command
					free(repeatCommand); // deleting the memory allocted
				}
			}

		}

		else if (choiceIndex != -1) // if the command is an aliased command do the following to execute
		{
			printf("Aliased Command: %s\tReal Command: %s\n", commandArray[0], realCommands[choiceIndex]);
			printf("Executing command: %s\n", realCommands[choiceIndex]);
			execution(realCommands[choiceIndex]);
		}

		else if (strcmp(command, " ") == 0)
		{
			commandArray[counter2] = malloc(strlen(" ") + 1);
			
			if (commandArray[counter2] == NULL) // checking whether memory is allocated
			{
				perror("Error");
				exit(0);
			}

			strcpy(commandArray[counter2], " ");
			counter2++;

			commandArray[counter2] = NULL;
			counter2++;
		}


		else // if a built in command or aliased command is not being called, then call execvp to execute commands
		{
			pid_t pid;
			pid = fork(); //create a child process
			if (pid > 0) //if parent, wait for child to finish execution
			{
				wait((int *)0);
			}

			else // if child, then use execvp to execute given commands
			{
				execvp(commandArray[0], commandArray);
				
				// if execvp does not work then this error statement will print out.
				printf("Sorry. That command is invalid.\n");
				exit(1);
			}

			printf("\n");
		}

		// deallocating memory for commandArray
		for (int q = 0; q < counter2; q++)
		{
			free(commandArray[q]);
		}
	}

	// deallocating memory for lineArray
	for (int p = 0; p < counter; p++)
	{
		free(lineArray[p]);
	}

}


void processcdCommand(char* cmd) 
{
	char * home_directory = malloc(sizeof(char)*512);//gets home directory
	home_directory = getenv("HOME");//changes the environment to home
	char* directory = malloc(sizeof(char)*512);
	directory = strtok(cmd," ");
	//directory = strtok(NULL, " ");
	//printf("This is cd directory: %s\n", directory);//checks for directory
	if (directory == NULL)
	{
		chdir(getenv("HOME"));//changes environment to home
	}
	else
	{
		if (chdir(directory) < 0)
		{
			perror("error\n");//error handler
		}
	}
}


void exec1(char * cmd) {
//printf("exec1: command %s\n", cmd);
  // input from stdin (already done)
  // output to pipe1
  dup2(pipe1[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  // exec
   execlp(cmd, cmd, NULL);
  // exec didn't work, exit
  //  execlp("ls", "ls", NULL);

  perror("bad exec1 fail");
  _exit(1);
}


void exec2(char * cmd) {
 //printf("exec2: command %s\n", cmd); 
// input from pipe1
  dup2(pipe1[0], 0);
  // output to pipe2
  dup2(pipe2[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
    execlp(cmd, cmd, NULL);
  // execlp("grep", "grep", "root", NULL);
  // exec didn't work, exit
  perror("bad exec2 fail");
  _exit(1);
}

void exec3(char * cmd) {
  // input from pipe2
  dup2(pipe2[0], 0);
  // output to stdout (already done)
  // close fds
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
    execlp(cmd, cmd, NULL);
 // execlp("grep", "grep", "sbin", NULL);
  // exec didn't work, exit
 perror("bad exec3 fail");
  _exit(1);
}


void pipes(char ** cmds) {

  // create pipe1
  if (pipe(pipe1) == -1) {
    perror("bad pipe1");
    exit(1);
  }

  // fork (ps aux)
  if ((pid = fork()) == -1) {
    perror("bad fork1");
    exit(1);
  } else if (pid == 0) {
    // stdin --> ps --> pipe1
//printf("fork 1 creating pipe %s\n", cmds[0]);
//printf("fork 2 creating pipe %s\n", cmds[1]);
//printf("fork 3 creating pipe %s\n", cmds[2]);
//printf("fork 4 creating pipe %s\n", cmds[3]);

 exec1(cmds[0]);

  }
  // parent
//wait(0);
  // create pipe2
  if (pipe(pipe2) == -1) {
    perror("bad pipe2");
    exit(1);
  }

  // fork (grep root)
  if ((pid = fork()) == -1) {
    perror("bad fork2");
    exit(1);
  } else if (pid == 0) {
    // pipe1 --> grep --> pipe2
  //  printf("This is is commands: %s\n",cmds[1]);
    exec2(cmds[1]);
  }
  // parent
//wait(0);
  // close unused fds
  
  if(*cmds[2] != '\000'){
  close(pipe1[0]);
  close(pipe1[1]);
//printf("this is cmds [2]: %c\n", *cmds[2]);
  // fork (grep sbin)
  if ((pid = fork()) == -1) {
    perror("bad fork3");
    exit(1);
  } else if (pid == 0) {
    // pipe2 --> grep --> stdout
    exec3(cmds[2]);
  }
  // parent
printf("\nPress enter to continue...\n");
}

}
 

