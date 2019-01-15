#include <thread>

class Sysmond {
public:
    Sysmond();
    ~Sysmond();
    double GetCpuUsage(void);
    unsigned long GetFreeMem(void);
    unsigned long GetTotalMem(void);
    unsigned long GetUseMem(void);

private:
    std::thread g_thread;
};