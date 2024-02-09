#include "winHead.h"

int main(int args, char *argv[]) {
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;

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

    //messages sending to the second computer
    const char *message = " Hello how's it going.";
    printf("Sending: %s\n", message, argv[2]);

    //concatnates message and argv[2] into new message
    const char *newMessage = strcat(argv[2],message);
    int bytes_sent = sendto(socket_peer,newMessage, strlen(newMessage),0,peer_address->ai_addr, peer_address->ai_addrlen);
    printf("Sent %d bytes.\n", bytes_sent);

    //receive checks for bytes if empty it continues
    char read [1024];
    int bytes_check = 1 ;
    do {
        int bytes_received = recvfrom(socket_peer,
                                      read, 1024,
                                      0,
                                      (struct sockaddr *) &peer_address, &peer_address->ai_addrlen);
        printf("%d bytes received. \n Message received:\n",bytes_received,read);
        bytes_check = bytes_received;
        Sleep(5);
    } while(bytes_check > 0);


    freeaddrinfo(peer_address);
    CLOSESOCKET(socket_peer);

    WSACleanup();

    printf("Finished.\n");
    return 0;
}