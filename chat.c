/*******************************************
* Group Name  : MK

* Member1 Name: Kevin Truong
* Member1 EID: truongak
* Member1 SIS ID: XXXXXX
* Member1 Login ID: XXXXXX

* Member2 Name: Michelle Cortes
* Member2 EID: mcortes2
* Member2 SIS ID: XXXXXX
* Member2 Login ID: XXXXXX
********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>

#include <string.h>
#include <strings.h>
#include <arpa/inet.h>

#include <regex.h>

void client(const char * server_id,const char * port){
	// client socket file descriptor
	int sockfd;
	// initialize address structs
	struct addrinfo hints, *res;

	// configure address info
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	// load up address structs with getaddrinfo()
	getaddrinfo(server_id, port, &hints, &res);

	// socket creation and verification
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1) {
		printf("socket creation failed...\n"); exit(0);
	}

	// connect the client socket to server socket and verification
	printf("Connecting to server...\n");
	if (connect(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
		printf("connection with the server failed...\n"); exit(0);
	}
	printf("Connected!\nConnected to a friend! You send first.\n");

	// start chatting loop with server user
	while (1) {
		// initialize message
		int max_message_size = 140;
		char message[141];
		
		while(1) {
			// prompt client to send first message
			printf("You: ");
			fgets(message, sizeof(message), stdin);
			message[strcspn(message, "\n")] = '\0';

			// send client message to server
			if (strlen(message) >= max_message_size) {
				printf("Error: input message too long.\n");
				// clear input stream
				char c; while ((c = getchar()) != '\n' && c != EOF) {}
			} else {
				int bytes_sent = send(sockfd, message, strlen(message), 0);
				if (bytes_sent == -1) { perror("Error sending message"); exit(1); }
				break;
			}
		}

		// receive message from the server
		int bytes_received = recv(sockfd, message, sizeof(message) - 1, 0);
		if (bytes_received <= 0) { perror("Error receiving message"); exit(1);}
		message[bytes_received] = '\0';

		// print the received message
		printf("Friend: %s\n", message);
	}
	
	// close the socket after chatting
	close(sockfd);
}

void server(const char * port){
	// server socket and connection file descriptors
	int sockfd, connfd;
	// initialize address structs
	struct addrinfo hints, *res, *cli;
	// initialize cli to point to the clientAddr structure
	socklen_t len = sizeof(struct sockaddr_storage);
	struct sockaddr_storage clientAddr;
	cli = (struct addrinfo *)&clientAddr;
	// configure address info
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	printf("Welcome to chat!\n");

    // load up address structs with getaddrinfo()
	getaddrinfo(NULL, port, &hints, &res);

	// socket creation and verification
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        printf("socket creation failed...\n"); exit(0);
    }
	
    // binding newly created socket to given ip and verification
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
        printf("socket bind failed...\n"); exit(0);
    }

	// print listening ip in human readable format
	char host[256];
	gethostname(host, sizeof(host));
	struct hostent *host_info = gethostbyname(host);
	char* host_address = host_info ? inet_ntoa(*(struct in_addr*)host_info->h_addr) : "";
	printf("Waiting for connection on %s port %s\n", host_address, port);

    // server is ready to listen on all available network interfaces and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("listen failed...\n"); exit(0);
    }

    // accept the data packet from client and verification
    connfd = accept(sockfd, NULL, NULL);
    if (connfd < 0) {
		perror("server accept failed");
		exit(0);
    }
	
	printf("Found a friend! You receive first.\n");

	// start chatting loop with client
	while (1) {
		// initialize message
		int max_message_size = 140;
		char message[141];

		// receive message from a client
		int bytes_received = recv(connfd, message, sizeof(message) - 1, 0);
		if (bytes_received <= 0) { perror("Error receiving message"); exit(1);}
		message[bytes_received] = '\0';
		// Print the received message
		printf("Friend: %s\n", message);

		while(1) {
			// prompt client to send first message
			printf("You: ");
			fgets(message, sizeof(message), stdin);
			message[strcspn(message, "\n")] = '\0';

			// send client message to server
			if (strlen(message) >= max_message_size) {
				printf("Error: input message too long.\n");
				// clear input stream
				char c; while ((c = getchar()) != '\n' && c != EOF) {}
			} else {
				int bytes_sent = send(connfd, message, strlen(message), 0);
				if (bytes_sent == -1) { perror("Error sending message"); exit(1); }
				break;
			}
		}
	}

    // close the socket after chatting
    close(connfd);
    close(sockfd);
}


int main(int argc, char* argv[]){
	// initialize port to 0 and server id to null
    char *port = NULL;
    char *server_id = NULL;
	char *server_port = "12345";

	// initialize regex
    regex_t regex;
	
	// port and ip pattern validators
	char *port_pattern = "^[0-9]{2,5}$";
	char *ip_pattern = "^([0-9]{1,3}\\.){3}[0-9]{1,3}$";

	// parse and validate opt codes -s -p -h
	int opt;
    while ((opt = getopt(argc, argv, "p:s:h")) != -1) {
        switch (opt) {
            case 'p':
				if(regcomp(&regex, port_pattern, REG_EXTENDED)){
					printf("couldn't compile");
				}
				if (regexec(&regex, optarg, 0, NULL, 0)!=0){
					printf("Port is invalid\n");
					exit(1);
				}
                port = optarg;
                break;
            case 's':
				if(regcomp(&regex, ip_pattern, REG_EXTENDED)){
					printf("couldn't compile");
				}
				if (regexec(&regex, optarg, 0, NULL, 0)!=0){
					printf("IP is invalid\n");
					exit(1);
				}
                server_id = optarg;
                break;
            case 'h':
                printf("Usage: %s -p <port> -s <server_id> -h\n", argv[0]);
                exit(0);
        }
    }

	// check exclusive or on server id and port args
	if (port == NULL ^ server_id == NULL) {
        fprintf(stderr, "Error: Both -p and -s options are required.\n");
        exit(1);
    }

	// run client or server
	if (port == NULL && server_id == NULL) {
		server(server_port);
        exit(1);
    } else {
		client(server_id, port);
        exit(1);
	}


  	return 0;
}
