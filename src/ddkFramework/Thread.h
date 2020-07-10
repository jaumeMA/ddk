#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef WIN32
#define _WINSOCKAPI_ // Avoid compiling errors
#endif

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void *ThreadFunc(void* arg); 

 /**
 * @class Thread
 * @brief This class manages the creation and destruction of threads
 */
class Thread {

	protected:
		pthread_t		    tHandle;		/**< Thread Handle */
		pthread_mutex_t		tMutex;			/**< Thread Mutex */
		pthread_cond_t		tCondition;		/**< Thread Condition */
		bool				isStopped;		/**< Flag to control the running status of the thread */

	public:	

		/** Constructor */
		Thread();

		/** Destructor */
		virtual ~Thread();
		
		/** Starts the thread. */
		virtual bool start();

		/** Stops the thread. */
		virtual bool stop();

		/** Kills the thread. */
		void kill();

		/** Executes the thread. */
		void execute();

		/** Interrupt the thread. */
		bool checkStop();

		/** wait for the end of thread */
		int waitFor(unsigned long timeout);

		/** waits to be notified or the timeout before continuing the execution*/
		int waitUntil(unsigned long timeout);

		/** Unblocks the threads currently waiting for this condition.*/
		void notify();

		/** Virtual function that that is called by the thread. All subclasses should implement it. */
		virtual void run()=0;

		static void sleep(unsigned long milliseconds);

private:
	Thread(const Thread &);
	Thread &operator =(const Thread &);
};

#endif  // _THREAD_H_