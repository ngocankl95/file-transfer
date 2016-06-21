#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

typedef uint8_t byte;

void error(int msg_code);
int sender_init(char *server_addr, int portno);
int listener_init(int portno);

void _send(FILE *_file, int socket);
void _receive(FILE *_file, int socket);
