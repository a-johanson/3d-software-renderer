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

#ifndef INC_WORKER_THREADS
#define INC_WORKER_THREADS


#include "vector_matrix.h"


struct _ThreadInfo {
int task;
real a[3];
real b[3];
real c[3];
real ca[3];
real cb[3];
real cc[3];
};

typedef struct _ThreadInfo ThreadInfo;




void WorkerThread(void * ThreadInfo);
int CreateWorkerThreads();
void WaitForThreadsToFinish();
void AssignTask_RasterTriangle(pVector a, pVector b, pVector c, pVector ca, pVector cb, pVector cc);


#endif
