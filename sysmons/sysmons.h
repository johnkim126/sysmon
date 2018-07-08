#include <thread>

class Sysmon {
public:
    Sysmon();
    ~Sysmon();
    double GetCpuUsage(void);
    unsigned long GetFreeMem(void);
    unsigned long GetTotalMem(void);
    unsigned long GetUseMem(void);

private:
    std::thread g_thread;
};