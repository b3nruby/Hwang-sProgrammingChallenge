#include "winHead.h"

char* sendMessage(char* ipnum, char*message){
    char read[1024];
    // start second remote address socket to send to receiver over port 8081
    printf("Configuring remote address to second computer...\n");
    struct addrinfo hints2;
    memset(&hints2, 0, sizeof(hints2));
    hints2.ai_socktype = SOCK_DGRAM;
    struct addrinfo *peer_address;
    // get IP from message
    if (getaddrinfo((ipnum), "8081", &hints2, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        exit;
    }
    printf("Remote address is: ");
    char address2_buffer[100];
    char service2_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
                address2_buffer, sizeof(address2_buffer),
                service2_buffer, sizeof(service2_buffer),
                NI_NUMERICHOST | NI_NUMERICSERV);
    printf("%s %s\n", address2_buffer, service2_buffer);
    printf("Creating socket...\n");
    SOCKET socket_peer;
    socket_peer = socket(peer_address->ai_family,
                         peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_peer)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        exit;

    }
    //forward message to third computer
    int tBytes = sendto(socket_peer,message, strlen(message), 0,peer_address->ai_addr, peer_address->ai_addrlen);
    printf("Bytes %d sent\n", tBytes);
    printf("Message sent to computer 3: %s\n", message);

    //Listen for message from the third computer
    int bytes_received = recvfrom(socket_peer,
                                  read, 1024,
                                  0,
                                  (struct sockaddr *) &peer_address, &peer_address->ai_addrlen);

    printf("Message received from computer 3: %s\n ", read);
    printf("Byte received %d\n ", bytes_received);
    message = read;
    CLOSESOCKET(socket_peer);
    return message;
}


int main() {
    int num = 1;

    while (num == 1) {
        char read[1024];
        char tmpBuff[1024];
        char message[1024];
        char ip[16];
        char* message2;

        //basic start for socket set up
        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }
        printf("Configuring local address...\n");

        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;
        struct addrinfo *bind_address;
        getaddrinfo(0, "8080", &hints, &bind_address);

        //socket creation
        printf("Creating socket...\n");
        SOCKET socket_listen;
        socket_listen = socket(bind_address->ai_family,
                               bind_address->ai_socktype, bind_address->ai_protocol);
        if (!ISVALIDSOCKET(socket_listen)) {
            fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }
        printf("Binding socket to local address...\n");
        if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
            fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }
        freeaddrinfo(bind_address);
        //start of receiving from computer 1

            struct sockaddr_storage client_address;
            socklen_t client_len = sizeof(client_address);
                int bytes_received = recvfrom(socket_listen,
                                              read, 1024,
                                              0,
                                              (struct sockaddr *) &client_address, &client_len);
                //split read into Ip and Message
                strcpy(tmpBuff, read);
                char *ptr = strtok(tmpBuff, " ");
                if (ptr != NULL) {
                    strcpy(ip, ptr);
                    ptr = strtok(NULL, "");
                    if (ptr != NULL) {
                        strcpy(message, ptr);
                    } else {
                        printf("No Message found.\n");
                    }
                } else {
                    printf("No IP address found\n");
                }
                //print out message and the number of bytes
                printf("Received %d bytes from computer 1\n", bytes_received);
                printf("Message Received: %s\n", message);

                // check to see if user wants to drop message
                char userInput[10];
                while (1) {
                    printf("Do you want to drop the message? (yes/no): ");
                    scanf("%s", userInput);
                    if (strcmp(userInput, "yes") == 0) {
                        printf("Message dropped. Exiting function.\n");
                        break;
                    } else if (strcmp(userInput, "no") == 0) {
                        printf("Continuing with processing...\n");
                        //function that will send and receive to computer 3 and return a message from computer 3
                        message2 = sendMessage(ip, message);
                        //function to send back to original sender
                        int bytes_sent = sendto(socket_listen, message2, strlen(message2), 0,
                                                (struct sockaddr *) &client_address, client_len);
                        printf("Sent %d bytes to computer 1.\n", bytes_sent);
                        printf( "Message sent to computer 1: %s\n", message2);
                        break;
                    } else {
                        printf("Invalid input. Please enter 'yes' or 'no'.\n");
                    }
                }
            CLOSESOCKET(socket_listen);
            WSACleanup();
            //check if to stay open or closed
            printf("Do you wanna stay open? (yes/no): ");
            scanf("%s", userInput);
            if(strcmp(userInput, "no") == 0){
                break;
            }
    }

    printf("Finished.\n");
    return 0;
}
