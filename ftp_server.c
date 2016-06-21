#include <stdio.h>
#include <sqlite3.h>
#include <time.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "net-lib.h"

void server_exe(int socket, char *client_name);
void init_database(char *db_file);
void loging(char *host_addr, char *file_addr, char *type);

sqlite3* database;
char *db_addr = "transfer_history.db";

int main(int argc, char *argv[])
{
	int socket = listener_init(5001);
	int new_socket, clilen, pid;
	struct sockaddr_in cli_add;
	byte buffer[256];

	clilen = sizeof(cli_add);
	init_database(db_addr);

	while(1)
	{
		new_socket = accept(socket, (struct sockaddr *) &cli_add, &clilen);
		if (new_socket < 0)
			error(31);
		pid = fork();
		if (pid<0)
		{
			error(33);
		}
		if (pid == 0)
		{
			close(socket);
			server_exe(new_socket, (char *)inet_ntoa(cli_add.sin_addr));
			exit(0);
		}
		else
			close(new_socket);

	}
	return 0;
}

void server_exe(int socket, char *client_name)
{
	int n;
	FILE *fp;
	char comm_str[256];
	char file_addr[256];

	byte buffer[256];
	bzero(file_addr, 256);
	n = read(socket, comm_str, 255);
	if(n == -1)
		error(15);
	write(socket, "r", 1);
	memmove(file_addr, comm_str+2, strlen(comm_str) - 2);
	if(strstr(comm_str, "-r") != NULL)
	{
		fp = fopen(file_addr, "rb");
		printf("Sending file %s\n", file_addr);
		_send(fp, socket);
		printf("%s\n",client_name);
		loging(client_name, file_addr, "Download");
		return;
	}
	if(strstr(comm_str, "-s") != NULL)
	{
		fp = fopen(file_addr, "wb");
		printf("Receiving file %s\n", file_addr);
		_receive(fp,socket);
		loging(client_name, file_addr, "Upload");
		return;
	}
}

void init_database(char *db_file)
{
	if(sqlite3_open(db_file, &database))
		error(17);
	printf("Database connection established\n");
}

void loging(char *host_addr, char *file_addr, char *type)
{
	char statement[1000];
	uint32_t c_time = time(NULL);
	char *err_code;
	//statement preparation
	sprintf(statement, "INSERT INTO HISTORY VALUES (%ld,\"%s\", \"%s\", \"%s\");", c_time, type, host_addr, file_addr);
	//exec statement
	if(sqlite3_exec(database, statement, 0, 0, &err_code) != SQLITE_OK)
	{
		printf("Log failed, code %s\n", err_code);
		error(17);
	}
}
