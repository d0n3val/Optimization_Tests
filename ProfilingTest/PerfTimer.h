#include <Windows.h>

class PerfTimer
{
	double frequency;
	double start;

public:

	PerfTimer()
	{
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		frequency = (double) f.QuadPart;
		Start();
	}

	void Start()
	{
		LARGE_INTEGER s;
		QueryPerformanceCounter(&s);
		start = (double) s.QuadPart;
	}

	double GetTime()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000.0 * (double(now.QuadPart) - start)) / frequency;
	}
};