#include "winHead.h"
#include <conio.h>


int main(int args, char *argv[]) {

    int bytes_check = 0 ;

    //messages initialized
    const char *message = " Hello how's it going.";

    //concatnates message and argv[2] into new message
    const char *newMessage = strcat(argv[2],message);

    while(bytes_check < 1) {
        bytes_check = 0;
        char read [1024];

        WSADATA d;
        if (WSAStartup(MAKEWORD(2, 2), &d)) {
            fprintf(stderr, "Failed to initialize.\n");
            return 1;
        }

        if (args < 3) {
            fprintf(stderr, "usage: tcp_client hostname port\n");
            return 1;
        }

        printf("Configuring remote address to second computer...\n");
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_socktype = SOCK_DGRAM;
        struct addrinfo *peer_address;
        // get IP from command line
        if (getaddrinfo(argv[1] , "8080", &hints, &peer_address)) {
            fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }
        printf("Remote address is: ");
        char address_buffer[100];
        char service_buffer[100];
        getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
                    address_buffer, sizeof(address_buffer),
                    service_buffer, sizeof(service_buffer),
                    NI_NUMERICHOST | NI_NUMERICSERV);
        printf("%s %s\n", address_buffer, service_buffer);
        printf("Creating socket...\n");
        SOCKET socket_peer;
        socket_peer = socket(peer_address->ai_family,
                             peer_address->ai_socktype, peer_address->ai_protocol);
        if (!ISVALIDSOCKET(socket_peer)) {
            fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
            return 1;

        }

        int timeout = 10000; // 5 seconds timeout
        if (setsockopt(socket_peer, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0) {
            fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
            CLOSESOCKET(socket_peer);
            return 1;
        }

        //messages sending to the second computer
        printf("Sending: %s\n", message, argv[2]);

        //check for timeout to resend message here

        //sending to computer 2
        int bytes_sent = sendto(socket_peer, newMessage, strlen(newMessage), 0, peer_address->ai_addr,
                                peer_address->ai_addrlen);
        printf("Sent %d bytes.\n", bytes_sent);

            //timeout if nothing is received.
            int bytes_received = recvfrom(socket_peer,
                                          read, 1024,
                                          0,
                                          (struct sockaddr *) &peer_address, &peer_address->ai_addrlen);
            //receive checks for bytes if empty it continues
            bytes_check = bytes_received;

        printf("%d bytes received. \n Message received: %s\n",bytes_received,read);

        Sleep(15);
        CLOSESOCKET(socket_peer);
        WSACleanup();
    }
    printf("Finished.\n");
    return 0;
}