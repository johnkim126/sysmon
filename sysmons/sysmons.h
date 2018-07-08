#include <thread>

class Sysmon {
public:
    Sysmon();
    ~Sysmon();
    double GetCpuUsage(void);

private:
    std::thread g_thread;
};