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
	uint64_t childrenElapsed;
	uint64_t hits;
	uint64_t depth;
	const char *label;
};

struct profiler
{
	profiler_anchor anchors[TOT_ANCHORS];

	uint64_t startTSC;
	uint64_t endTSC;
};

static profiler g_profiler;
static uint32_t g_currentAnchor;


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
		profiler_anchor *anchor = &g_profiler.anchors[anchorIdx];
		anchor->depth++;

		m_tag = tag;
		m_start = readCPUTimer();
		m_anchorIdx = anchorIdx;
		if (anchor->depth <= 1)
		{
			m_parentAnchor = g_currentAnchor;
			g_currentAnchor = anchorIdx;
		}
	}

	~FunctionTimer()
	{
		profiler_anchor *anchor = &g_profiler.anchors[m_anchorIdx];
		profiler_anchor *parent = &g_profiler.anchors[m_parentAnchor];

		int64_t elapsed = readCPUTimer() - m_start;

		if (anchor->depth <= 1)
		{
			anchor->elapsed += elapsed;
			anchor->label = m_tag;
			if (g_currentAnchor)
			{
				parent->childrenElapsed += elapsed; // don't include the children run time
			}

			g_currentAnchor = m_parentAnchor;
		}
		anchor->hits++;
		anchor->depth--;

	}

private:
	const char *m_tag;
	uint64_t m_start;
	uint32_t m_anchorIdx;
	uint32_t m_parentAnchor;
	
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

	printf("Total time: %.4f ms (CPU freq: %zu)\n", totalTime, cpuFreq);
	for (uint32_t i = 0; i < TOT_ANCHORS; i++)
	{
		profiler_anchor *anchor = &g_profiler.anchors[i];
		if (anchor->elapsed)
		{
				printf("\t[%s]: %zu clks (%.2f%%", anchor->label, anchor->elapsed, (double) 100 * (anchor->elapsed - anchor->childrenElapsed) / totalTicks);
			if (anchor->childrenElapsed)
			{
				printf(", children %.02f%%", 100.0 * anchor->childrenElapsed / totalTicks);
			}
			printf(") hits: %zu\n", anchor->hits);
		}
	}
}

