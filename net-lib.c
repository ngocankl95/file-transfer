#include "net-lib.h"



void error(int msg_code)
{
	switch(msg_code){
		//error code 0x: Syntax Error
		case 1:
			 printf("Syntax error: Use ./ftp_client server_address [-r||-s 'source file name' 'destination file name']\n");
			 break;
		//error code 1x: Runtime error
		case 10:
			printf("Error opening socker\n");
			break;
		case 11:
			printf("Server Error: No such server\n");
			break;
		case 12:
			printf("Server Error: Can't connect to designated server\n");
			break;
		case 13:
			printf("Server Error: Can't send file\n");
			break;
		case 14:
			printf("File not found");
			break;
		case 15:
			printf("Socket Error: Can't read from socket\n");
			break;
		case 16:
			printf("Message format error: Invalid message\n");
			break;
		case 17:
			printf("Database error\n");
			break;
		case 33:
			printf("Process Error: Can't create child process\n");
			break;
		case 20:
			printf("Server binding error\n");
			break;
		case 31:
			printf("Accept failed\n");
			break;
		case 32:
			printf("Fork failed\n");
		//Not specified: General Error
		default:
			printf("General Error\n Code %d\n", msg_code);
			break;
	}
	exit(0);
}

int sender_init(char *server_addr, int portno)
{
	int sockfd;
	struct sockaddr_in server_address;
	struct hostent *server;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);//init socket
	if (sockfd < 0)
		error(10);//Socket error - Code 10
	
	//try to find host
	//if error, send code 11 (Server error)
	server = gethostbyname(server_addr);
	if(server == NULL)
		error(11);
	server_address.sin_family = AF_INET;//socket family Internet socket
	
	bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
	//convert port number to network format
	server_address.sin_port = htons(portno);
	
	//try connect to server
	//if error, send error code 12 (Server error)
	if(connect(sockfd, (struct sockaddr *) &server_address, 
				sizeof(server_address)) < 0)
		error(12);
	//return socket
	return sockfd;
}

int listener_init(int portno)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);//open socket
        if (sockfd < 0)
                error(10);//error code 10

	struct sockaddr_in server_addr, client_addr;
	
	bzero((char *) &server_addr, sizeof(server_addr));//init listener addr
	
	server_addr.sin_family = AF_INET;//sockadd family = Internet 
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portno);

	//bind socket
	//if error, send error code 20
	if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) <0)
		error(20);
	
	listen(sockfd, 10);//start listening
	return sockfd;//return socket
}

void _send(FILE *_file, int socket)
{
	int read_byte, send_byte;
	byte buffer[256];
	while(1)
	{
		read_byte = fread(buffer, sizeof(uint8_t), 256, _file);
		if(read_byte <= 0)
			break;
		do
		{
			send_byte = write(socket, buffer, read_byte);
			if(send_byte == read_byte)
				break;
		}while((send_byte < 0)&&(errno == EINTR));
		if (send_byte == -1) 
			error(11);
	}
	if(read_byte == -1)
		error(14);
	fclose(_file);
}

void _receive(FILE *_file, int socket)
{
	int received_byte, write_byte;
	byte buffer[256];
	while(1)
	{
		bzero(buffer, 256);
		received_byte = read(socket, buffer, 256);
		if(received_byte < 1)
			break;
		do
		{
			write_byte = fwrite(buffer, 1, received_byte, _file);
			if(write_byte == received_byte)
				break;
		} while((write_byte == -1)&&(errno == EINTR));
		if (write_byte == -1)
			error(14);
	}
	if(received_byte == -1)
		error(11);
	fclose(_file);
}


