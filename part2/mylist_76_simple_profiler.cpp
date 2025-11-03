#include <cstdint>
#include <iostream>
#include <format>
#include <vector>
#include <sys/time.h>
#include <x86intrin.h>

#include "util.h"


#define TimeBlock(label) FunctionTimer _(label);
#define TimeFunction TimeBlock(__FUNCTION__);

struct timestamp
{
	std::string tag;
	uint64_t elapsed;
};

static std::vector<struct timestamp> g_timestamps{};
static unsigned int g_counter{0};

static uint64_t g_startTime;

static u64 getOSTimerFreq(void)
{
    return 1000000; // us
}


static u64 readOSTimer(void)
{
    struct timeval val;
    gettimeofday(&val, NULL);

    u64 res = val.tv_sec * getOSTimerFreq() + val.tv_usec;
    return res;
}


static inline u64 readCPUTimer(void)
{
    return __rdtsc();
}


static u64 getApproxCPUFreq(u32 millisToWait)
{
    // millisToWait: how long to wait to approximate cpu freq.
    // The longer the wait time, the better
    u64 stopCond = getOSTimerFreq() * millisToWait / 1000;
    u64 osStart = readOSTimer();
    u64 osEnd = readOSTimer();
    u64 osElapsed = 0;
    u64 cpuStart = readCPUTimer();
    while (osElapsed < stopCond)
    {
        osEnd = readOSTimer();
        osElapsed = osEnd - osStart;
    }
    u64 cpuEnd = readCPUTimer();
    u64 cpuElapsed = cpuEnd - cpuStart;

    return getOSTimerFreq() * cpuElapsed / osElapsed;
}



class FunctionTimer
{
public:
	FunctionTimer(std::string tag)
	{
		m_functionName = tag;
		m_start = readCPUTimer();
	}

	~FunctionTimer()
	{
		uint64_t stop = readCPUTimer();
		g_timestamps.push_back({m_functionName, stop - m_start});
		g_counter++;
	}

private:
	uint64_t m_start;
	std::string m_functionName;
};



void beginProfiler()
{
	g_timestamps.reserve(32);
	g_startTime = __rdtsc();
}


void endProfiler()
{
	uint64_t stop = __rdtsc();
	uint64_t cpuFreq = getApproxCPUFreq(100);
	uint64_t totalTicks = stop - g_startTime;
	double totalTime = totalTicks / (double)cpuFreq;

	std::cout << std::format("Total time: {:.4f} ms (CPU freq: {})\n", totalTime, cpuFreq);
	for (const auto &i : g_timestamps)
	{
		std::cout << std::format("\t{}: {} ({:.2f}%)\n", i.tag, i.elapsed, (double) 100 * i.elapsed / totalTicks);
	}
}

