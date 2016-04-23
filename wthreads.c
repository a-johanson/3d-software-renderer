// Copyright 2010 Arne Johanson
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <process.h>
#include <windows.h>
#include "wthreads.h"
#include "engine.h"


int thread_num=0;
ThreadInfo * wthread;

CRITICAL_SECTION CriticalSection; 



void WorkerThread(void * tin)
{
	ThreadInfo * myti = (ThreadInfo *)tin;
	
	while(1)
	{
		switch(myti->task)
		{
			case 1:
				aglDrawTriangle(myti->a, myti->b, myti->c, myti->ca, myti->cb, myti->cc);
				myti->task=0;
			break;
			
			default:
				Sleep(0);
			break;
		}
	}
}

/* Returns the number of threads created. */
int CreateWorkerThreads()
{
	int threadid,i;
	SYSTEM_INFO sysinf;
	
	
	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x80000400) ) 
		return 0;

	
	
	GetSystemInfo(&sysinf);
	
	if((wthread = (ThreadInfo *)malloc(sysinf.dwNumberOfProcessors*sizeof(ThreadInfo)))==NULL)
			return 0;

	for(i=0; i<sysinf.dwNumberOfProcessors; i++)
	{
		wthread[i].task=0;
		if(_beginthreadex( NULL, 0, (void *)WorkerThread, &wthread[i], 0, &threadid )==0)
			return 0;
		thread_num++;
	}
	
	return sysinf.dwNumberOfProcessors;
}


void WaitForThreadsToFinish()
{
	int i;
	
	for(i=0; i<thread_num; i++)
	{
		while(wthread[i].task!=0)
		{
			Sleep(0);
		}
	}
}


void AssignTask_RasterTriangle(pVector a, pVector b, pVector c, pVector ca, pVector cb, pVector cc)
{
	int i;
	
	do {
		for(i=0; i<thread_num; i++)
		{
			if(wthread[i].task==0)
				break;
		}
	} while (i>=thread_num);
	
	wthread[i].a[0]=a[0];
	wthread[i].a[1]=a[1];
	wthread[i].a[2]=a[2];
	
	wthread[i].b[0]=b[0];
	wthread[i].b[1]=b[1];
	wthread[i].b[2]=b[2];
	
	wthread[i].c[0]=c[0];
	wthread[i].c[1]=c[1];
	wthread[i].c[2]=c[2];
	
	wthread[i].ca[0]=ca[0];
	wthread[i].ca[1]=ca[1];
	wthread[i].ca[2]=ca[2];
	
	wthread[i].cb[0]=cb[0];
	wthread[i].cb[1]=cb[1];
	wthread[i].cb[2]=cb[2];
	
	wthread[i].cc[0]=cc[0];
	wthread[i].cc[1]=cc[1];
	wthread[i].cc[2]=cc[2];
	
	wthread[i].task=1;
}



