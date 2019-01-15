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
#include "sysmond.h"

Sysmond *mond;

int
connect_socket(char *server_ip, char *port)
{
    int client_socket, state, client_len;
    struct sockaddr_in addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(server_ip);
    addr.sin_port = htons(atoi(port));

    client_len = sizeof(addr);
    connect(client_socket, (struct sockaddr *)&addr, client_len);
    
    return client_socket;
}

void
set_cpu(char *server_ip, char *port, char *hostname)
{
    int     client_socket;
    char    buf[1024];
    
    client_socket = connect_socket(server_ip, port);
    
    sprintf(buf, "setcpu,%s,%lf", hostname, mond->GetCpuUsage());
    write(client_socket, buf, 1024);
    close(client_socket);
}

void
set_mem(char *server_ip, char *port, char *hostname)
{
    int     client_socket;
    char    buf[1024];
    
    client_socket = connect_socket(server_ip, port);
    
    sprintf(buf, "setmem,%s,%lu,%lu,%lu", hostname, 
    mond->GetTotalMem(), mond->GetUseMem(), mond->GetFreeMem());
    write(client_socket, buf, 1024);
    close(client_socket);
}

int
main(int argc, char **argv)
{
    char *hostname, *master_ip, *port;

    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <Master IP> <Port> <Hostname>" << std::endl;
        exit(1);
    }

    mond = new Sysmond();
    master_ip = argv[1];
    port = argv[2];
    hostname = argv[3];

    while (1) {
        set_cpu(master_ip, port, hostname);
        set_mem(master_ip, port, hostname);
        sleep(1);
    }
    return 0;
}
