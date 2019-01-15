#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <iostream>
#include <thread>
#include <atomic>
#include "sysmons.h"

#define STAT_SIZE 512

typedef struct {
    long long unsigned    user; //time spent in user mode.
    long long unsigned    nice; //time spent in user mode with low priority.
    long long unsigned    system; //time spent in system mode.
    long long unsigned    idle; //time spent in the idle task.
    long long unsigned    iowait; //time waiting for I/O to complete.
    long long unsigned    irq; //time servicing hardware interrupts.
    long long unsigned    softirq; //time servicing software interrupts.
    long long unsigned    steal; //time spent in other operating systems when running in a virtualized environment.
    long long unsigned    guest; //time spent running a virtual CPU for guest operating systems.
    long long unsigned    guest_nice; //time spent running a low priority virtual CPU for guest operating systems.
} cpu_info_t;

typedef struct {
    double            cpu_usage;
    unsigned long     free_mem;
    unsigned long     total_mem;
    unsigned long     use_mem;
} res_info;

std::atomic<int> is_running;

res_info rinfo;

uint32_t
get_total_active_time(cpu_info_t *info)
{
    uint32_t total;
    total = info->user + info->nice + info->system;
    total += info->irq + info->softirq + info->steal + info->guest + info->guest_nice;

    return total;
}

uint32_t
get_total_idle_time(cpu_info_t *info)
{
    uint32_t total;
    total = info->idle + info->iowait;

    return total;
}

uint32_t
get_total_time(cpu_info_t *info)
{
    return get_total_active_time(info) + get_total_idle_time(info);
}

cpu_info_t *
get_cpu_info(void)
{
    FILE        *fp;
    cpu_info_t  *info;
    char        *str;
    char        buf[STAT_SIZE];
    
    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        return NULL;
    }
    fgets(buf, STAT_SIZE, fp);
    fclose(fp);

    str = strstr(buf, "cpu ");
    if (str == NULL) {
        return NULL;
    }

    info = (cpu_info_t *)calloc(1, sizeof(cpu_info_t));

    sscanf(str,  "cpu  %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
        &info->user, &info->nice, &info->system, &info->idle, &info->iowait,
        &info->irq, &info->softirq, &info->steal, &info->guest, &info->guest_nice);   

    return info;
}

void
set_memory_info(void)
{
    struct sysinfo info;
    sysinfo(&info);

    rinfo.total_mem = info.totalram;
    rinfo.free_mem = info.freeram;
    rinfo.use_mem = rinfo.total_mem - rinfo.free_mem;
}

void
gather_thread(void *arg)
{
    cpu_info_t  *info;
    uint32_t    total, active;

    while (is_running) {
        set_memory_info();
        info = get_cpu_info();
        total = get_total_time(info);
        active = get_total_active_time(info);
	    rinfo.cpu_usage = 100.f * ((double)active / (double)total);
        //printf("CPU Usage: %lf\n", cpu_usage);
        sleep(1);
    }
}

Sysmon::Sysmon() {
    is_running = 1;
}

Sysmon::~Sysmon() {
    is_running = 0;
}

double
Sysmon::GetCpuUsage(void)
{
    return rinfo.cpu_usage;   
}

unsigned long
Sysmon::GetFreeMem(void)
{
    return rinfo.free_mem;
}

unsigned long
Sysmon::GetTotalMem(void)
{
    return rinfo.total_mem;
}

unsigned long
Sysmon::GetUseMem(void)
{
    return rinfo.use_mem;
}