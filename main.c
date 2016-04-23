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
#include <stdio.h>
#include <math.h>
#include "tinyptc.h"
#include "vector_matrix.h"
#include "3d_helper.h"
#include "phys_helper.h"
#include "wthreads.h"
#include "engine.h"


#define MAX_STARS		50


void InitParticle(pParticle p, real x, real y, real z);
real Random(real min, real max);




extern CRITICAL_SECTION CriticalSection;



int main()
{
	const real keyboard_sens=6.0*M_PI;
	//const real mouse_sens=0.0025;
	unsigned long long clk_freq, clk_now, clk_prev;
	real elapsed_time;
	unsigned char keyboard[256];
	//int mouse_x, mouse_y;
	real angle_plane=0.0, angle_up=0.0;
	int i;
	
	//int frame_counter=0;
	
	real cam[4]={0.0, 0.5, -7.0, 1.0};
	real lookat[4]={0.0, 0.0, 0.0, 1.0};
	real vup[4]={0.0, -1.0, 0.0,1.0};

	pVector wing_colors = LoadColorFile("wings.color");
	pTriangleStrip wing_strip = LoadTriangleStrip("wings.strip");
	
	pVector cone_colors = LoadColorFile("cone.color");
	pTriangleStrip cone_strip = LoadTriangleStrip("cone.strip");
	
	
	pVector star_colors = LoadColorFile("star.color");
	pTriangleStrip star_strip = LoadTriangleStrip("star.strip");
	
	
	RenderTree troot;
	pRenderTree ding1, ding2;
	

	Particle stars[MAX_STARS];

	if(!QueryPerformanceFrequency((LARGE_INTEGER *)&clk_freq))
		return 1;
	
	troot.strip=NULL;
	troot.mesh=NULL;
	troot.child=NULL;
	troot.sibling=NULL;
	
	srand(12525);
	
	
	for(i=0;i<MAX_STARS;i++)
	{
		InitParticle(&stars[i],0.0, 1.0, 0.0);
		stars[i].renderEl=Add_Tree_Child(&troot);
		stars[i].renderEl->strip=star_strip;
		stars[i].renderEl->colors=star_colors;
	}
	
	
	
	
	ding1=Add_Tree_Child(&troot);
	ding2=Add_Tree_Child(&troot);
	
	
	ding1->strip=wing_strip;
	ding1->colors=wing_colors;
	
	ding2->strip=cone_strip;
	ding2->colors=cone_colors;
	
	
	MatrixLoadXRotation(ding1->m_current, -0.5);
	MatrixLoadScaling(ding1->m_temp, 0.5, 0.5, 0.5);
	MatrixMult4x4(ding1->m_current, ding1->m_temp, ding1->m_current);
	MatrixLoadTranslation(ding1->m_temp, 0.0, 1.0, 0.15);
	MatrixMult4x4(ding1->m_current, ding1->m_temp, ding1->m_current);
	
	
	MatrixLoadScaling(ding2->m_current, 0.4, 1.04, 0.4);
	

	
	CreateWorkerThreads();
	
	
	
    if (!ptc_open("3de", WIDTH, HEIGHT))
		return 1;
	
	
	//ptc_mouse(&mouse_x, &mouse_y);
	
	QueryPerformanceCounter((LARGE_INTEGER *)&clk_prev);
	
    while (1)
    {
		PrepareScene(0x00004080);
		
		
		/* Input */
		
		
		/* Mouse */
		//ptc_mouse(&mouse_x, &mouse_y);
		//printf("%i, %i\r\n", mouse_x, mouse_y);
		//angle_plane+=mouse_sens*(real)mouse_x;
		//angle_up+=mouse_sens*(real)mouse_y;
		
		/* Clock */
		QueryPerformanceFrequency((LARGE_INTEGER *)&clk_freq);
		QueryPerformanceCounter((LARGE_INTEGER *)&clk_now);
		elapsed_time=(real)(clk_now-clk_prev)/(real)clk_freq;
		
		
		/* Keyboard */
		GetKeyboardState(keyboard);
		
		
		if(keyboard[VK_UP]&0x80)
		{
			cam[0]+=elapsed_time*keyboard_sens*(lookat[0]-cam[0]);
			//cam[1]+=keyboard_sens*(lookat[1]-cam[1]);
			cam[2]+=elapsed_time*keyboard_sens*(lookat[2]-cam[2]);
		}
		if(keyboard[VK_DOWN]&0x80)
		{
			cam[0]-=elapsed_time*keyboard_sens*(lookat[0]-cam[0]);
			//cam[1]-=keyboard_sens*(lookat[1]-cam[1]);
			cam[2]-=elapsed_time*keyboard_sens*(lookat[2]-cam[2]);
		}
		
		if(keyboard[VK_RIGHT]&0x80)
		{
			angle_plane-=elapsed_time*keyboard_sens*0.15;
		}
		if(keyboard[VK_LEFT]&0x80)
		{
			angle_plane+=elapsed_time*keyboard_sens*0.15;
		}
		
		if(keyboard['A']&0x80)
		{
			angle_up+=elapsed_time*keyboard_sens*0.1;
		}
		if(keyboard['Q']&0x80)
		{
			angle_up-=elapsed_time*keyboard_sens*0.1;
		}
		
		if(keyboard['W']&0x80)
		{
			cam[1]+=elapsed_time*keyboard_sens*0.25;
		}
		if(keyboard['S']&0x80)
		{
			cam[1]-=elapsed_time*keyboard_sens*0.25;
		}
		
		
		if(angle_plane<0.0)
			angle_plane+=2.0*M_PI;
		else if(angle_plane>2.0*M_PI)
			angle_plane-=2.0*M_PI;
		
		if(angle_up<-0.4*M_PI)
			angle_up=-0.4*M_PI;
		else if(angle_up>0.2*M_PI)
			angle_up=0.2*M_PI;
		
		
		lookat[0]=cam[0]-sin(angle_plane)*cos(angle_up);
		lookat[1]=cam[1]-sin(angle_up);
		lookat[2]=cam[2]+cos(angle_plane)*cos(angle_up);
		//lookat[3]=1.0;
		
		
		
		Create_WC_to_CC_Matrix(troot.m_temp, cam, lookat, vup);
		
		
		
		/*Animation*/
		
		MatrixLoadZRotation(ding1->m_temp, elapsed_time*0.5*M_PI);
		MatrixMult4x4(ding1->m_current, ding1->m_current, ding1->m_temp);
		
		
		for(i=0;i<MAX_STARS;i++)
		{
			Euler_YGravitation(&stars[i], elapsed_time);
			
			stars[i].lifespan+=elapsed_time;
			
			stars[i].rotation[0] += elapsed_time*stars[i].spin[0];
			stars[i].rotation[1] += elapsed_time*stars[i].spin[1];
			stars[i].rotation[2] += elapsed_time*stars[i].spin[2];
			
			if(stars[i].position[1]<0.0)
			{
				stars[i].position[1]=0.0;
				stars[i].velocity[1] *= -0.86;
				stars[i].spin[0] *= -0.95;
				stars[i].spin[1] *= -0.95;
				stars[i].spin[2] *= -0.95;
			}
			
			if(stars[i].lifespan<1.5)
			{
				stars[i].size=stars[i].lifespan/1.5;
			}
			else if(stars[i].lifespan>16.0)
			{
				InitParticle(&stars[i],0.0, 1.0, 0.0);
			}
			else if(stars[i].lifespan>15.0)
			{
				stars[i].size=0.0;
			}
			else if(stars[i].lifespan>12.0)
			{
				stars[i].size=(15.0-stars[i].lifespan)/3.0;
			}
			else if(stars[i].lifespan>1.5)
			{
				stars[i].size=1.0;
			}
			
			MatrixLoadScaling(stars[i].renderEl->m_current, stars[i].size, stars[i].size, stars[i].size);
			MatrixLoadXRotation(stars[i].renderEl->m_temp, stars[i].rotation[0]);
			MatrixMult4x4(stars[i].renderEl->m_current, stars[i].renderEl->m_temp, stars[i].renderEl->m_current);
			MatrixLoadYRotation(stars[i].renderEl->m_temp, stars[i].rotation[1]);
			MatrixMult4x4(stars[i].renderEl->m_current, stars[i].renderEl->m_temp, stars[i].renderEl->m_current);
			MatrixLoadZRotation(stars[i].renderEl->m_temp, stars[i].rotation[2]);
			MatrixMult4x4(stars[i].renderEl->m_current, stars[i].renderEl->m_temp, stars[i].renderEl->m_current);
			MatrixLoadTranslation(stars[i].renderEl->m_temp, stars[i].position[0], stars[i].position[1], stars[i].position[2]);
			MatrixMult4x4(stars[i].renderEl->m_current, stars[i].renderEl->m_temp, stars[i].renderEl->m_current);
		}
		
		
		doRenderTree(&troot);
		WaitForThreadsToFinish();
		
        RenderScene();
		
		clk_prev=clk_now;
    }
	
	DeleteCriticalSection(&CriticalSection);
}


real Random(real min, real max)
{
	return ((real)rand()/(real)RAND_MAX)*(max-min) + min;
}


void InitParticle(pParticle p, real x, real y, real z)
{
	p->position[0]=x;
	p->position[1]=y;
	p->position[2]=z;
	
	p->velocity[0]=Random(-25.0, 25.0);
	p->velocity[1]=Random(12.0, 35.0);
	p->velocity[2]=Random(-25.0, 25.0);
	
	
	p->spin[0]=Random(-0.3*M_PI, 0.3*M_PI);
	p->spin[1]=Random(-0.3*M_PI, 0.3*M_PI);
	p->spin[2]=Random(-0.3*M_PI, 0.3*M_PI);
	
	p->g=-9.81;
	
	p->size=0.0;
	p->lifespan=0.0;
	p->state=0;
	
	p->rotation[0]=0.0;
	p->rotation[1]=0.0;
	p->rotation[2]=0.0;
}
