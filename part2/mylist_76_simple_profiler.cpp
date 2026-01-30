#include <cstdint>
#include <iostream>
#include <format>
#include <vector>
#include <sys/time.h>
#include <x86intrin.h>

#include "util.h"


#define TimeBlock(label) FunctionTimer _(label, __COUNTER__ + 1);
#define TimeFunction TimeBlock(__FUNCTION__);


#define TOT_ANCHORS 4096


struct profiler_anchor
{
	uint64_t elapsed;
	uint64_t hits;
	const char *label;
};

struct profiler
{
	profiler_anchor anchors[TOT_ANCHORS];

	uint64_t startTSC;
	uint64_t endTSC;
};

static profiler g_profiler;
static unsigned int g_counter{0};


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
	FunctionTimer(const char *tag, uint32_t anchorIdx)
	{
		m_tag = tag;
		m_start = readCPUTimer();
		m_anchorIdx = anchorIdx;
	}

	~FunctionTimer()
	{
		profiler_anchor *anchor = &g_profiler.anchors[m_anchorIdx];

		anchor->label = m_tag;
		anchor->elapsed += readCPUTimer() - m_start;
		anchor->hits++;
	}

private:
	const char *m_tag;
	uint64_t m_start;
	uint32_t m_anchorIdx;
};



void beginProfiler()
{
	g_profiler.startTSC = __rdtsc();
}


void endProfiler()
{
	g_profiler.endTSC = __rdtsc();
	uint64_t cpuFreq = getApproxCPUFreq(100);
	uint64_t totalTicks = g_profiler.endTSC - g_profiler.startTSC;
	double totalTime = totalTicks / (double)cpuFreq;

	std::cout << std::format("Total time: {:.4f} ms (CPU freq: {})\n", totalTime, cpuFreq);
	for (uint32_t i = 0; i < TOT_ANCHORS; i++)
	{
		profiler_anchor *anchor = &g_profiler.anchors[i];
		if (anchor->elapsed)
		{
			std::cout << std::format("\t{}: {} ({:.2f}%) hits: {}\n", anchor->label, anchor->elapsed, (double) 100 * anchor->elapsed / totalTicks, anchor->hits);
		}
	}
}

