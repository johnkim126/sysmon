#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int
main(int argc, char **argv)
{    
    int client_socket;
    int state;
    struct sockaddr_in addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(12345);

    int client_len = sizeof(addr);
    
    connect(client_socket, (struct sockaddr *)&addr, client_len);
    
    char buf[1024];
    while (1) {
        fgets(buf, 1024, stdin);

        buf[1024 - 1] = '0';
        write(client_socket, buf, 1024);

        if (strncmp(buf, "quit", 4) == 0) {
            std::cout << "quit" << std::endl;
            break;
        }
        read(client_socket, buf, 1024);
        std::cout << "buf: "  << buf << std::endl;
    }
    close(client_socket);
    return 0;
}
