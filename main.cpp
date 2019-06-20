#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include<string>
#include <unistd.h>
using namespace std;
string line;
class AutoResetEvent
{
  public:
  explicit AutoResetEvent(bool initial = false);

  ~AutoResetEvent();
  void Set();
  void Reset();

  bool WaitOne();

  private:
  AutoResetEvent(const AutoResetEvent&);
  AutoResetEvent& operator=(const AutoResetEvent&); // non-copyable
  bool flag_;
  pthread_mutex_t protect_;
  pthread_cond_t signal_;
};

AutoResetEvent::AutoResetEvent(bool initial)
: flag_(initial)
{
  pthread_mutex_init(&protect_, NULL);
  pthread_cond_init(&signal_, NULL);
}

void AutoResetEvent::Set()
{
  pthread_mutex_lock(&protect_);
  flag_ = true;
  pthread_mutex_unlock(&protect_);
  pthread_cond_signal(&signal_);
}

void AutoResetEvent::Reset()
{
  pthread_mutex_lock(&protect_);
  flag_ = false;
  pthread_mutex_unlock(&protect_);
}

bool AutoResetEvent::WaitOne()
{
  pthread_mutex_lock(&protect_);
  while( !flag_ ) // prevent spurious wakeups from doing harm
    pthread_cond_wait(&signal_, &protect_);
  flag_ = false; // waiting resets the flag
  pthread_mutex_unlock(&protect_);
  return true;
}

AutoResetEvent::~AutoResetEvent()
{
  pthread_mutex_destroy(&protect_);
  pthread_cond_destroy(&signal_);
}


AutoResetEvent event;

void *otherthread(void *)
{
 	while (true)
 		{
 			event.WaitOne();
  			cout << "Hello from the second thread: "<< line << endl;
			event.Set();
			sleep(1);
		}
}


int main()
{
  pthread_t h;
  ifstream infile("L2.txt");
  pthread_create(&h, NULL, &otherthread, NULL);
  while (getline(infile, line)) {
        cout << "Hello from the first thread: "<< line << endl;
  		event.Set();
  		sleep(1);
		  }
	infile.close();
	event.Reset();
  return 0;
}
