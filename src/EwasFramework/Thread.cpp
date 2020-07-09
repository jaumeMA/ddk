#include "Thread.h"

#include "WASLogger.h"
#include <exception>

#ifdef WIN32
#include <windows.h>
#else
#include <chrono>
#include <thread>
#endif

/* Definition of the ThreadFunc */
 void* ThreadFunc(void* pv) {
	int retval = 0;
	try {
		(reinterpret_cast<Thread *>(pv))->execute();
	} catch(std::exception& e) {		
		WAS_LOG_ERROR("Thread: An error occured while calling run: " << e.what());
#ifndef __APPLE__
		exit(-1);
#endif
	} catch(...) {
		WAS_LOG_ERROR("Thread: An error occured while calling run.");
#ifndef __APPLE__
		exit(-1);
#endif
	}

	return NULL;
}

/** Constructor */
Thread::Thread(): isStopped(true) {
	pthread_mutex_init(&tMutex, NULL);
	pthread_cond_init(&tCondition, NULL);
}

/** Destructor */
Thread::~Thread() {
	pthread_mutex_destroy(&tMutex);
	pthread_cond_destroy(&tCondition);
}

/** Starts the thread. */
bool Thread::start() {
	int retval = 0;

	if(isStopped)
	{
		pthread_attr_t	attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		pthread_setcanceltype  (PTHREAD_CANCEL_ASYNCHRONOUS,  NULL); 
	
		isStopped = false;

		do {

			retval = pthread_create(&tHandle, &attr, ThreadFunc, this);
			if(retval)
			{ 
				WAS_LOG_ERROR("Thread: An error occurred while starting thread: " << strerror(errno));

				Thread::sleep(static_cast<unsigned long>(100));
			}
		}while(retval != 0);

		pthread_attr_destroy(&attr);
	
		return true;
	}

	return false;
}

/** Executes the thread. */
void Thread::execute() {

    // Call Thread Run function
	run();

	pthread_mutex_lock(&tMutex);
	isStopped = true;
	pthread_cond_signal(&tCondition);
	pthread_mutex_unlock(&tMutex);
}

/** Stops the thread. */
bool Thread::stop() {
		
	if(!isStopped) 
	{
		pthread_mutex_lock(&tMutex);
		isStopped = true;
		pthread_cond_signal(&tCondition);
		pthread_cond_wait(&tCondition, &tMutex);
		pthread_mutex_unlock(&tMutex);

		return true;
	}

	return false;
}

/** Interrupt the thread. */
bool Thread::checkStop() {
	return isStopped;
}

/** wait for the end of thread */
int Thread::waitFor(unsigned long timeout)
{ 
	int retval = 0;

	if(!isStopped) 
	{
		struct timespec time_to_wait = {time(NULL) + (int) (timeout/1000), 0};

		pthread_mutex_lock(&tMutex);
		isStopped = true;
		if((retval = pthread_cond_timedwait(&tCondition, &tMutex, &time_to_wait)) == ETIMEDOUT)
		{
			kill();
		}
		pthread_mutex_unlock(&tMutex);
	}

	return retval;
}

/** wait for the end of thread without killing the thread*/
int Thread::waitUntil(unsigned long timeout)
{
	struct timespec time_to_wait = {time(NULL) + (int) (timeout/1000), 0};

	pthread_mutex_lock(&tMutex);
	int retval = pthread_cond_timedwait(&tCondition, &tMutex, &time_to_wait);
	pthread_mutex_unlock(&tMutex);
	return retval;
}

/** Unblocks the threads currently waiting for this condition.*/
void Thread::notify()
{
	pthread_mutex_lock(&tMutex);
	pthread_cond_signal(&tCondition);
	pthread_mutex_unlock(&tMutex);
}

void Thread::kill()
{
	#ifdef WIN32
	HANDLE wHandle = pthread_getw32threadhandle_np (tHandle);
	TerminateThread(wHandle, -1);
	#else
	pthread_cancel(tHandle);
	#endif

	isStopped = true;
}

void Thread::sleep(unsigned long milliseconds)
{
	#ifdef WIN32
	Sleep(milliseconds);
	#else
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	#endif
}