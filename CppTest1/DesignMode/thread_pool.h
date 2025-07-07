#pragma once

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include<thread>
#include <mutex>
#include <condition_variable>

//任务信息
struct ctrip_task
{
	int value;
	ctrip_task* next;
};

struct ctrip_thread_info 
{
	int					thread_running;
	int					thread_num;
	int					tasknum;
	struct ctrip_task* tasks;
	std::thread*		threadid;
	std::mutex			mutex;
	std::condition_variable	cv;
};

static void ctrip_init_thread_pool(int thread_num);

static void ctrip_destroy_thread_pool();

static void ctrip_thread_pool_add_task(ctrip_thread_info* t);

static ctrip_task* ctrip_thread_pool_retrieve_task();

//执行队列中任务
static void ctrip_thread_pool_do_task(ctrip_task* t);


static void* ctrip_thread_routine(void* thread_param);


#endif //THREAD_POOL_H_




















