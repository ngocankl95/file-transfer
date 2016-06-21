#include "net-lib.h"
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	int send_byte;
	char buffer[256];

	if(argc < 4)
	{
		error(1);
		exit(0);
	}
	int socket = sender_init(argv[1], 5001);

	//send message to receive all listening client
	//not implemented
//	if(strcmp(argv[2], "-l")==0)
//	{
//		write(socket, "list-all", 9);
//		bzero(buffer, 256);
//		if(read(socket, buffer, 256) >= 0)
//			printf("%s", buffer);
//		else
//			error(11);
//		exit(0);
//	}
//	else 

	//check syntax
	//prep message
	//send through socket
	if(strcmp(argv[2],"-s")==0)
	{
		bzero(buffer, 256);
		fp = fopen(argv[3], "rb");
		printf("Sending file %s to %s/%s\n", argv[3], argv[1], argv[4]);
		strcat(buffer, argv[2]);
		strcat(buffer, argv[4]);
		if(fp==NULL)
			error(14);
		send_byte = write(socket, buffer, strlen(buffer));
		bzero(buffer, 256);
		while(1)
			if((read(socket, buffer, 256) > 0)&&(strstr(buffer, "r") != NULL))
				break;
		_send(fp, socket);
		exit(0);
	}
	if(strcmp(argv[2], "-r")==0)
	{
		bzero(buffer, 256);
		fp = fopen(argv[4], "wb");
		printf("Receiving file %s, write to %s\n", argv[3], argv[4]);
		strcat(buffer, argv[2]);
		strcat(buffer, argv[3]);
		if(fp == NULL)
			error(14);
		send_byte = write(socket, buffer, strlen(buffer));
		while((read(socket, buffer, 256) > 0)&&(strstr(buffer, "r") != NULL))
		_receive(fp, socket);
	}
}
