#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "PerfTimer.h"

float lt_sin[360];
float lt_cos[360];

static const float  EPSILON = 0.0001f;
static const float  INFINITY = 3.4e38f;
static const float  PI = 3.14159265358979323846f;
static const float  TWO_PI = 6.28318530717958647692f;
static const float  QUAD_PI = 9.86960440108935861883f;
static const float  HALF_PI = 1.57079632679489661923f;
static const float  QUARTER_PI = 0.78539816339744830961f;
static const float  INV_PI = 0.31830988618379067154f;
static const float  INV_TWO_PI = 0.15915494309189533576f;
static const float  DEG_TO_RAD = 0.01745329251994329577f;
static const float  RAD_TO_DEG = 57.2957795130823208768f;

void fill_lookup_tables()
{
	for(int i = 0 ; i < 360 ; ++i)
	{
		lt_sin[i] = sin(DEG_TO_RAD*i);
		lt_cos[i] = cos(DEG_TO_RAD*i);
	}
}

// ---------------------------------------------------
void cos_lookup()
{
	float x = lt_cos[45];
	float y = lt_cos[90];
	float z = lt_cos[180];
}
// ---------------------------------------------------
void cos_no_lookup()
{
	float x = cos(DEG_TO_RAD*45);
	float y = cos(DEG_TO_RAD*90);
	float z = cos(DEG_TO_RAD*180);
}

void VecAdd(float* result, float* a, float *b)
{
	*result = *a + *b;
}

void VecAdd_restrict(float * _restrict result, float* a, float * b)
{
	*result = *a + *b;
}

void call_add()
{
	float a, b, c;

	a = 0.0f;
	b = c = 66.0f;

	for(int i = 0; i<999; i++)
		VecAdd(&a, &b, &c);
}

void call_add_restrict()
{
	float a, b, c;

	a = 0.0f;
	b = c = 66.0f;

	for(int i = 0; i<999; i++)
		VecAdd_restrict(&a, &b, &c);
}


// ---------------------------------------------------
void cache_aligned()
{
	struct S 
	{
		int a, b;
		long space[9999];

		S(): a(0), b(0) {}
	} myS;

	//myS.a = 11;
	//myS.b = 22;
	int p = myS.a + myS.b;
}
// ---------------------------------------------------
void cache_not_aligned()
{
	struct S 
	{
		int a;
		long space[9999];
		int b;

		S(): a(0), b(0) {}
	} myS;

	//myS.a = 22;
	//myS.b = 11;
	int p = myS.a + myS.b;
}
// ---------------------------------------------------
void no_constructor()
{
	struct p2
	{
		float x,y;

		void Init()
		{
			x = y = 0.0f;
		}
	};

	p2 objects[999];
}

// ---------------------------------------------------
void no_constructor2()
{
	struct p2
	{
		float x,y;

		p2()
		{
			x = 0;
			y = 0;
		}
	};

	p2 objects[999];
}

// ---------------------------------------------------
void no_constructor3()
{
	struct p2
	{
		float x,y;

		p2() : x(0.0f), y(0.0f)
		{}
	};

	p2 objects[999];
}

// ---------------------------------------------------
void test_dist_sqrt()
{
	float range = 9999.0f;

	float x = 123.123f;
	float y = 567.567f;

	bool in_range = sqrt(x*x + y*y) < range;	
}

// ---------------------------------------------------
void test_dist_no_sqrt()
{
	float range = 9999.0f * 9999.0f;

	float x = 123.123f;
	float y = 567.567f;

	bool in_range = (x*x + y*y) < range;	
}

#define MEASURE(function, repetitions) \
	timer.Start();\
	for(int i=0; i<repetitions; ++i) function();\
	printf("%s: %.3f ms\n", #function, timer.GetTime());\

int main( int argc, const char *argv[] ) {
	
	PerfTimer timer;
	fill_lookup_tables();
	
	// ---------------------------------------------------
	timer.Start();
	Sleep(1000);
	printf("Sleep(1000): %.3f ms\n", timer.GetTime());
	// ---------------------------------------------------
	MEASURE(test_dist_sqrt, 99999);
	MEASURE(test_dist_no_sqrt, 99999);
	MEASURE(no_constructor, 999);
	MEASURE(no_constructor2, 999);
	MEASURE(no_constructor3, 999);
	MEASURE(cos_lookup, 999);
	MEASURE(cos_no_lookup, 999);
	MEASURE(cache_aligned, 9999);
	MEASURE(cache_not_aligned, 9999);
	MEASURE(call_add, 9999);
	MEASURE(call_add_restrict, 9999);

	getchar();
}