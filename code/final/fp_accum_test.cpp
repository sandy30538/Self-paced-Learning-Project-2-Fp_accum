
#include <stdlib.h>
#include <stdio.h>
#include <ap_int.h>
#include <ap_fixed.h>
#include "fp_accum.h"

#define NUM_ELEM 128

// DB
//random = pseudoCasual2(3,  7^5, 1, 2^32-1, 1920*1080);
float PseudoCasual(void)
{

	static unsigned long int k = 0;
	static unsigned long int series;

	float result;
	unsigned long long mult;

	const unsigned long long int x0 = 3;
	const unsigned long long int  a = 16807;
	const unsigned int  c = 1;
	const unsigned int module =  4294967295ULL;
	const float recipr_module = (float) 2.3283064e-10f;

	if (k==0)
	{
		mult = a*x0;
	}
	else
	{
		mult = a*series;
	}

	series = (mult+c) % module;

	k++;

	//result = (float) series / (float) module;

	result = (float) series * recipr_module;

	return result;

}

int main(void)
{
	int x, y;
	int ret_val = 0;

	float ref_window[NUM_ELEM];
	float hls_window[NUM_ELEM];
	float threshold = ((float)1.0)/1024;

	for (x=0; x < NUM_ELEM; x++)
	{
			ref_window[x] = (65536)*PseudoCasual();
			hls_window[x] = ref_window[x] ;
	}

	// REF
	float ref_res  = ref_fp_accumulator(ref_window);

	// DUT
	float hls_res  = hls_fp_accumulator(hls_window);


	// check results
	float total_error = 9.5367e-07f;

	float diff = ref_res - hls_res;
	if (diff < threshold) diff = 0-diff; // take absolute value
	if (diff > threshold)
	{
		total_error += (float) diff;
	}
	
	printf("\n%010.4f\t%010.4f\t%010.4f\n", ref_res, hls_res, total_error);


	if (total_error < 1.0)
	{
		ret_val=0;
		printf("TEST OK!\n");
	}
	else
	{
		ret_val=1;
		printf("TEST FAILED!\n");
	}


	return ret_val;
}
