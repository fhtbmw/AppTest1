#include <iostream>
#include <stdlib.h>
#include "thread_pool.h"

struct ctrip_thread_info g_threadinfo;
int thread_running = 0;

void ctrip_init_thread_pool(int thread_num)
{
	if (thread_num <= 0)
	{
		thread_num = 5;
	}

	thread_running = 1;

	g_threadinfo.threadid = (std::thread*)malloc(sizeof(std::thread) * thread_num);

	int i;
	for (i = 0; i < thread_num; ++i)
	{
		
	}

}

//
//void main()
//{
//	
//}


























