run: ftp_server.o ftp_client.o
	gcc ftp_client.o net-lib.o -o ftp_client
	gcc ftp_server.o net-lib.o -o ftp_server -lsqlite3
	chmod a+x ftp_server
	chmod a+x ftp_client

ftp_client.o: net-lib.o
	gcc -c ftp_client.c -o ftp_client.o

ftp_server.o: net-lib.o
	gcc -c ftp_server.c -o ftp_server.o

net-lib.o: net-lib.c
	gcc -c net-lib.c -o net-lib.o

clean:
	rm -rf *.o ftp_server ftp_client
