#include "winHead.h"

int main() {
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
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    int run = 1;
    int ret = 0;
    char read[1024];
    char tmpBuff[1024];
    char message[1024];
    char message2[1024]
    char ip[16];

    while (run == 1) {
        int bytes_received = recvfrom(socket_listen,
                                      read, 1024,
                                      0,
                                      (struct sockaddr *) &client_address, &client_len);

        //split read into Ip and Message
        strcpy(tmpBuff,read);

        char *ptr = strtok(tmpBuff, " ");
        if (ptr != NULL) {
            strcpy(ip, ptr);
            ptr = strtok(NULL, "");
            if (ptr != NULL) {
                strcpy(message, ptr);
            }else {
                    printf("No Message found.\n");
                }
            }else {
            printf("No IP address found\n");
        }

        printf("\nReceived (%d bytes)\n",bytes_received);
        printf("Message Received: %s\n", message);

        printf("Remote address is: ");
        char address1_buffer[100];
        char service1_buffer[100];
        getnameinfo(((struct sockaddr *) &client_address),
                    client_len,
                    address1_buffer, sizeof(address1_buffer),
                    service1_buffer, sizeof(service1_buffer),
                    NI_NUMERICHOST | NI_NUMERICSERV);
        printf("%s %s\n", address1_buffer, service1_buffer);
        run += 1;
    }

    // start second remote address socket to send to receiver over port 8081
    do {
        printf("Configuring remote address to second computer...\n");
        struct addrinfo hints2;
        memset(&hints2, 0, sizeof(hints2));
        hints2.ai_socktype = SOCK_DGRAM;
        struct addrinfo *peer_address;
        // get IP from message
        if (getaddrinfo((ip), "8081", &hints2, &peer_address)) {
            fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
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
        SOCKET socket_listen;
        socket_peer = socket(peer_address->ai_family,
                             peer_address->ai_socktype, peer_address->ai_protocol);
        if (!ISVALIDSOCKET(socket_peer)) {
            fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
            return 1;

        }
        //foward message to third computer
        int tBytes = sendto(socket_peer,message, strlen(message), 0,
                            (struct sockaddr *) &peer_address, peer_len);
        printf("Bytes &n sent\n", tBytes);
        printf("Message sent: &s\n", message);

        //Listen for message from the third computer
        int bytes_received = recvfrom(socket_peer,
                                      read, 1024,
                                      0,
                                      (struct sockaddr *) &client_address, &client_len);
        message2 = read;

        CLOSESOCKET(socket_peer);

        ret = 2;

    } while (ret == 1);

    //function to send back to original sender

    int bytes_sent = sendto(socket_peer,message2, strlen(message2),0, client_address->ai_addr, client_address->ai_addrlen);
    printf("Sent %d bytes.\n", bytes_sent);

    CLOSESOCKET(socket_listen);

    WSACleanup();

    printf("Finished.\n");
    return 0;
}
