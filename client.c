/*
		Stav Macri 324084722 - סתיו מכרי
		Nadav Swartz 208296400 - נדב שוורץ
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_ARRAY_SIZE 5
#define MAX_STRING_LEN 25



void handleError(int error_code);
void handleSignal(int sig);
void handleAlarm(int sig);
void handleSignal2(int sig);

int main(int argc, char** argv)
{

	if(argc - 1 != 4)
	{
		perror("Enter : [server.id] [num1] [num2] [operation]");
		exit(-1);
	}


	signal(SIGUSR1, handleSignal);
	signal(SIGUSR2, handleSignal2);
	signal(SIGALRM, handleAlarm);

	srand(time(NULL));

	int randomNumber = (rand() % 5) + 5;
	sleep(randomNumber);

	struct stat buffer_path;
	int count = 0;
	while((stat("to_srv.txt", &buffer_path) == 0) && count < 10)
	{
		++count;
		printf("Someone is currently accessing the server -- Please wait untill server is free..\nCurrent Attempt:%d -- retrying..\n", count);
		int randomNumber = (rand() % 5) + 1;
		sleep(randomNumber);
	}

	if(count >= 10)
	{
		printf("ERROR:\nto_srv.txt is opened so it's accessed by another user for a while.. Server probably didn't respond..");
		exit(-1);
	}

	int fd = open("to_srv.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	handleError(fd);

	char buffer[50];
	sprintf(buffer, "%d\n%s\n%s\n%s\n", getpid(), argv[2], argv[3], argv[4]);

	int bytesWrite = write(fd, buffer, strlen(buffer));
	if(bytesWrite < 0)
	{
		close(fd);
		handleError(bytesWrite);
	}
	close(fd);

	kill(atoi(argv[1]), SIGUSR1);

	alarm(15);

	pause();

	exit(-1);
}

void handleSignal2(int sig)
{
	unlink("to_srv.txt");

	printf("ERROR: Invalid operation entered, please try again.\n");
	exit(-1);
}

void handleAlarm(int sig)
{

	unlink("to_srv.txt");

	printf("ERROR: SERVER IS NOT RESPONDING.\nServer didn't respond within 15 seconds.\n");
	exit(-1);
}

void handleSignal(int sig)
{
	if(sig != SIGUSR1) return;

	char buffer[MAX_STRING_LEN];
	sprintf(buffer, "response_client_%d.txt", getpid());
	int responseFromServer = open(buffer, O_RDONLY);
	handleError(responseFromServer);

	if(unlink(buffer) < 0)
	{
		perror("Error deleting file");
		exit(-1);
	}

	memset(buffer, 0, sizeof(buffer));

	int bytesRead = read(responseFromServer, buffer, sizeof(buffer)-1);
	if(bytesRead < 0)
	{
		close(responseFromServer);
		handleError(bytesRead);
	}
	close(responseFromServer);

	buffer[strlen(buffer)] = '\0';

	printf("%s", buffer);
	exit(0);
}

void handleError(int error_code)
{

        if(error_code < 0)
		{
                perror(NULL);
                exit(-1);
        }
}
