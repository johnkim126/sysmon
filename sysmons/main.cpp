#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <mutex>

using namespace std;

typedef struct {
    double            cpu_usage;
    unsigned long     free_mem;
    unsigned long     total_mem;
    unsigned long     use_mem;
} res_info_t;

unordered_map<string, res_info_t *> list;
std::mutex mtx;

static void
set_cpu(char *str)
{
    std::string hostname;
    char        *c_host, *c_cpu;
    res_info_t  *res_info;
    double      cpu_usage;
    
    strtok(str, ",\n");
    c_host = strtok(NULL, ",\n");
    if (c_host == NULL) {
        std::cerr << "c_host is null\n" << std::endl;
        return;
    }

    hostname = c_host;
    std::lock_guard<std::mutex> guard(mtx);
    
    c_cpu = strtok(NULL, ",\n");
    if (c_cpu) {
        cpu_usage = atof(c_cpu);
    } else {
        cpu_usage = 0.0;
    }

    auto host_info = list.find(hostname);
    if (host_info == list.end()) {
        res_info = (res_info_t *)calloc(1, sizeof(res_info_t));
        res_info->cpu_usage = cpu_usage;
        list.insert({hostname, res_info});
        std::cout << "insert host cpu: " << hostname << "," << cpu_usage << std::endl;
    } else {
        res_info = (res_info_t *)host_info->second;
        res_info->cpu_usage = cpu_usage;
        std::cout << "change host cpu: " << hostname << "," << cpu_usage << std::endl;
    }
}

static void
set_mem(char *str)
{
    std::string     hostname;
    char            *c_host, *c_total, *c_use, *c_free;
    res_info_t      *res_info;
    unsigned long   free_mem, total_mem, use_mem;
    
    strtok(str, ",\n");
    c_host = strtok(NULL, ",\n");
    if (c_host == NULL) {
        std::cerr << "c_host is null\n" << std::endl;
        return;
    }

    hostname = c_host;
    std::lock_guard<std::mutex> guard(mtx);
    
    c_total = strtok(NULL, ",\n");
    c_use = strtok(NULL, ",\n");
    c_free = strtok(NULL, ",\n");

    total_mem = free_mem = use_mem = 0;

    if (c_total) {
        sscanf(c_total, "%lu", &total_mem);
    }
    if (c_use) {
        sscanf(c_use, "%lu", &use_mem);
    }
    if (c_free) {
        sscanf(c_free, "%lu", &free_mem);
    }
    auto host_info = list.find(hostname);
    if (host_info == list.end()) {
        res_info = (res_info_t *)calloc(1, sizeof(res_info_t));
        res_info->total_mem = total_mem;
        res_info->use_mem = use_mem;
        res_info->free_mem = free_mem;
        list.insert({hostname, res_info});
        std::cout << "insert host mem: " << hostname << "," << total_mem << "," << use_mem << "," << free_mem << std::endl;
    } else {
        res_info = (res_info_t *)host_info->second;
        res_info->total_mem = total_mem;
        res_info->use_mem = use_mem;
        res_info->free_mem = free_mem;
        std::cout << "change host mem: " << hostname << "," << total_mem << "," << use_mem << "," << free_mem << std::endl;
    }
}

static void
print_dstat(int socket)
{
    std::lock_guard<std::mutex> guard(mtx);
    for (auto it: list) {
        std::string hostname;
        res_info_t *res_info;
        
        hostname = it.first;
        res_info = (res_info_t *) it.second;
        
        //TODO: write to socket
        std::cout << hostname << ", cpu: " << res_info->cpu_usage;
        std::cout << ",mem: " << res_info->use_mem << "/" << res_info->free_mem;
        std::cout << "/" << res_info->total_mem << std::endl;
    }
}

void
process(int socket)
{
    char cmd[1024];
    read(socket, cmd, sizeof(cmd));

    printf("call: %s\n", cmd);
    if (strncmp(cmd, "setcpu", 6) == 0) {
        set_cpu(cmd);
    } else if (strncmp(cmd, "setmem", 6) == 0) {
        set_mem(cmd);
    } else if (strncmp(cmd, "dstat", 4) == 0) {
        print_dstat(socket);
    }
    close(socket);
}

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

    while (1) {
        socklen_t client_len = 0;
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        std::thread thr(process, client_socket);
        thr.detach();
    }
    close(server_socket);
    return 0;
}
