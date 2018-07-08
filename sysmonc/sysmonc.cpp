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

void
usage(int argc, char **argv)
{
    fprintf(stdout, "Usage: %s <options>\n", argv[0]);
    fprintf(stdout, "   getcpu: Get CPU from Node\n");
    fprintf(stdout, "   getmem: Get MEM from Node\n");
}

int
main(int argc, char **argv)
{    
    int client_socket;
    int state;
    struct sockaddr_in addr;

    if (argc <= 1) {
        usage(argc, argv);
        exit(1);
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(12345);

    int client_len = sizeof(addr);
    
    connect(client_socket, (struct sockaddr *)&addr, client_len);
    
    char buf[1024];
    sprintf(buf, "%s", argv[1]);
    write(client_socket, buf, 1024);

    read(client_socket, buf, 1024);
    printf("%s", buf);
    close(client_socket);
    return 0;
}
