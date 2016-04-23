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

#ifndef INC_PHYSHELPER
#define INC_PHYSHELPER

#include "vector_matrix.h"
#include "engine.h"

struct _Particle {
real position[3];
real velocity[3];
real spin[3];
real g;

real size;
real rotation[3];

real lifespan;
int state;
pRenderTree renderEl;
};

typedef struct _Particle Particle;
typedef Particle * pParticle;



void Euler_YGravitation(pParticle p, real h);


#endif
