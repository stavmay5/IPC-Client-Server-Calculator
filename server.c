/*
		Stav Macri 324084722 - סתיו מכרי
		Nadav Swartz 208296400 - נדב שוורץ
*/

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>


#define MAX_ARRAY_SIZE 5
#define MAX_STRING_LEN 25



void handleError(int error_code);
char** loadLinesToBuffer(int fd, int* num_of_students);
void UserInput(int signal);


int main(int argc, char** argv)
{
	signal(SIGUSR1, UserInput);

	while(1)
	{
		pause();
	}

	while(wait(NULL) != -1);

	exit(0);
}



void UserInput(int sig)


	if(sig != SIGUSR1) return;

	int status;

	if(fork() == 0)
	{

		int fd = open("to_srv.txt", O_RDONLY);
		handleError(fd);

		int line_count = 0;
		char** data = loadLinesToBuffer(fd, &line_count);
		close(fd);

		handleError(unlink("to_srv.txt"));

		int sentProcessID = atoi(data[0]);
		int num1 = atoi(data[1]);
		int num2 = atoi(data[2]);
		int operation = atoi(data[3]);

		char responseBuffer[MAX_STRING_LEN];

		switch(operation)
		{

			case 1:
				sprintf(responseBuffer, "Result is: %d\n", num1 + num2);
				break;
			case 2:
				sprintf(responseBuffer, "Result is: %d\n", num1 - num2);
				break;
			case 3:
				sprintf(responseBuffer, "Result is: %d\n", num1 * num2);
				break;
			case 4:
				if (num2 == 0) strcpy(responseBuffer, "You can't divide by 0!\n");
				else
					sprintf(responseBuffer, "Result is: %d\n", num1 / num2);

				break;
			default:

				printf("Invalid operation entered..");
				kill(sentProcessID, SIGUSR2);
				break;

		}

		char pathBuffer[MAX_STRING_LEN];
		sprintf(pathBuffer, "response_client_%d.txt", sentProcessID);
		int userFd = open(pathBuffer, O_RDWR | O_CREAT | O_TRUNC, 0666);
		handleError(userFd);

		int bytesWrite = write(userFd, responseBuffer, strlen(responseBuffer));
		if(bytesWrite < 0)
		{
			close(userFd);
			exit(-1);
		}
		close(userFd);

		kill(sentProcessID, SIGUSR1);
		exit(0);
	}

	wait(&status);

	signal(SIGUSR1, UserInput);


}


void handleError(int error_code)
{

        if(error_code < 0)
		{
                perror(NULL);
                exit(-1);
        }
}

char** loadLinesToBuffer(int fd, int* num_of_students)
{

        handleError(fd);

        char** namesArray = NULL;
        char buffer[MAX_ARRAY_SIZE][MAX_STRING_LEN] = {0};
        char current_char = '\0';
        int i = 0;
        int numOfStudents = 0;

        int bytesRead = -1;
        do {
                bytesRead = read(fd, &current_char, 1);
                //if error then handle it.
                handleError(bytesRead);

                if(bytesRead  <= 0) break;

                if(current_char == '\n')
				{

                        buffer[numOfStudents][i] = '\0';
                        numOfStudents++;

                        i = 0;
                        continue;
                }
                buffer[numOfStudents][i] = current_char;
                i++;

        } while(bytesRead > 0);


        namesArray = (char**)calloc(numOfStudents, sizeof(char*));

        for(int i = 0; i < numOfStudents; i++)
		{
                namesArray[i] = (char*)calloc(strlen(buffer[i])+1, sizeof(char));
                strcpy(namesArray[i], buffer[i]);

        }

        *num_of_students = numOfStudents;
        return namesArray;
}
