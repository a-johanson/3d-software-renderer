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

#include "vector_matrix.h"
#include "phys_helper.h"


void Euler_YGravitation(pParticle p, real h)
{
	p->position[0]+=h*p->velocity[0];
	p->position[1]+=h*p->velocity[1];
	p->position[2]+=h*p->velocity[2];
	
	p->velocity[0]+=h*-0.3*p->velocity[0];
	p->velocity[1]+=h*p->g;
	p->velocity[2]+=h*-0.3*p->velocity[2];
}
