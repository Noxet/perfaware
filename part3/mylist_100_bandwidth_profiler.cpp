#include <cstdio>
#include <cstdint>
#include <sys/time.h>
#include <x86intrin.h>

#include "util.h"

#define PROFILER 1

#ifndef PROFILER
#define PROFILER 0
#endif


struct profiler_anchor
{
	uint64_t elapsed;
	uint64_t childrenElapsed;
	uint64_t elapsedAtRoot;
	uint64_t hits;
	uint64_t bytesProcessed;	// How many bytes a block is supposed to handle, so we can calc. throughput
	const char *label;
};


#define TOT_ANCHORS 4096

struct profiler
{
	profiler_anchor anchors[TOT_ANCHORS];

	uint64_t startTSC;
	uint64_t endTSC;
};

static profiler g_profiler;

/**
 * Timing-related functions
 */
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


/* 
 * we want to be able to turn off the profiler
 * in case we want to verify that it doesn't affect the runtime too much
 */
#if PROFILER


/**
 * Profiler-related functions
 */
#define TimeBandwidth(label, bytes) FunctionTimer _(label, bytes, __COUNTER__ + 1);
#define TimeBlock(label) TimeBandwidth(label, 0);
#define TimeFunction TimeBlock(__FUNCTION__);


static uint32_t g_currentAnchor;


class FunctionTimer
{
public:
	FunctionTimer(const char *tag, uint64_t bytesProcessed, uint32_t anchorIdx)
	{
		m_parentAnchor = g_currentAnchor;
		g_currentAnchor = anchorIdx;

		m_tag = tag;
		m_anchorIdx = anchorIdx;

		profiler_anchor *anchor = &g_profiler.anchors[anchorIdx];
		m_oldElapsedAtRoot = anchor->elapsedAtRoot;
		anchor->bytesProcessed += bytesProcessed;

		m_start = readCPUTimer();
	}

	~FunctionTimer()
	{
		int64_t elapsed = readCPUTimer() - m_start;

		profiler_anchor *anchor = &g_profiler.anchors[m_anchorIdx];
		profiler_anchor *parent = &g_profiler.anchors[m_parentAnchor];


		anchor->elapsed += elapsed;
		parent->childrenElapsed += elapsed; // don't include the children run time
		// overwrite value, to not count the recursive calls
		anchor->elapsedAtRoot = m_oldElapsedAtRoot + elapsed;
		anchor->label = m_tag;

		g_currentAnchor = m_parentAnchor;
		anchor->hits++;
	}

private:
	const char *m_tag;
	uint64_t m_start;
	uint32_t m_anchorIdx;
	uint32_t m_parentAnchor;
	uint64_t m_oldElapsedAtRoot;
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

	printf("Total time: %.4f ms (CPU freq: %zu)\n", 1000.0 * totalTime, cpuFreq);
	for (uint32_t i = 0; i < TOT_ANCHORS; i++)
	{
		profiler_anchor *anchor = &g_profiler.anchors[i];
		uint64_t elapsedSelf = anchor->elapsed - anchor->childrenElapsed;
		if (anchor->elapsed)
		{
			printf("\t%s[%zu]: %zu clks (%.2f%%", anchor->label, anchor->hits, anchor->elapsedAtRoot, (double) 100 * (anchor->elapsed - anchor->childrenElapsed) / totalTicks);
			if (anchor->elapsedAtRoot != elapsedSelf)
			{
				printf(", %.02f%% w/ children", 100.0 * anchor->elapsedAtRoot / totalTicks);
			}
			printf(")\n");
		}

		if (anchor->bytesProcessed)
		{
		    double time = (anchor->elapsed - anchor->childrenElapsed) / (double)cpuFreq;
		    double megabytesProcessed = anchor->bytesProcessed / 1024.0 / 1024.0;
		    double gigabytesPerSecond = megabytesProcessed / 1024.0 / time;
		    printf("\t%s processed %.2f MB in %.2f ms - throughput = %.2f GB/s\n", anchor->label, megabytesProcessed, 1000.0 * time, gigabytesPerSecond);
		}
	}
}

#else


#define TimeBlock(...)
#define TimeFunction

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
}
#endif
