#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include "sysmons.h"

int
main(int argc, char **argv)
{    
    int server_socket;
    int client_socket;
    int state;
    struct sockaddr_in addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(12345);

    bind(server_socket, (struct sockaddr *)&addr, sizeof(addr));

    listen(server_socket, 3);

    Sysmon *sysmon = new Sysmon();

    char buf[1024];
    while (1) {
        socklen_t client_len = 0;
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        
        std::cout << "client_socket" << std::endl;
        read(client_socket, buf, 1024);

        std::cout << "buf: " << buf << std::endl;
        if (strncmp(buf, "exit", 4) == 0) {
            close(client_socket);            
            break;
        } else if (strncmp(buf, "getcpu", 6) == 0) {
            char cpu_usage[1024];
            sprintf(cpu_usage, "CPU Usage: %lf\n", sysmon->GetCpuUsage());
            write(client_socket, cpu_usage, 1024);
        } else if (strncmp(buf, "getmem", 6) == 0) {
            char mem_usage[1024];
            sprintf(mem_usage, "MEM Usage: Use: %lu, Free: %lu, Total: %lu\n",
                sysmon->GetUseMem(), sysmon->GetFreeMem(), sysmon->GetTotalMem());
            write(client_socket, mem_usage, 1024);
        }
        close(client_socket);
    }
    close(server_socket);

    delete sysmon;
    return 0;
}
