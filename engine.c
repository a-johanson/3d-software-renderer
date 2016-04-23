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

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "engine.h"
#include "tinyptc.h"
#include "vector_matrix.h"
#include "3d_helper.h"
#include "wthreads.h"




int pixels[SIZE];
real zbuffer[SIZE];

real aspect_ratio=((real)HEIGHT)/((real)WIDTH);
real fov2 = 0.2*M_PI; /* Der halbe FOV-Winkel */
real xres=WIDTH;
real yres=HEIGHT;

//real u_len=20, v_len=u_len*aspect_ratio;
real n_min=0.125, n_max=1000.0;

extern CRITICAL_SECTION CriticalSection;
extern int surface_width;
extern int surface_height;



pVector onbv, onbn, onbb;



void RenderScene()
{
	ptc_update(pixels);
}

void PrepareScene(int bg_color)
{
	int y,x;

	for(y=0 ; y<HEIGHT; y++)
	{
		for(x=0 ; x<WIDTH; x++)
		{
			pixels[(y*WIDTH) + x] = bg_color;
			zbuffer[(y*WIDTH) + x] = n_max;
		}
	}
}



pVector LoadColorFile(const char* filename)
{
	FILE* file;
	char str[256];
	int len, line;
	real red, green, blue;
	pVector colors=NULL;
	

	file = fopen(filename, "r");
	if (!file)
		return NULL;
	
	if(fgets(str, 256, file)==NULL)
	{
		fclose(file);
		return NULL;
	}
	
	if(sscanf(str, "%i", &len)!=1)
	{
		fclose(file);
		return NULL;
	}
	
	colors = Vector_Create(len*3);
	
	line=0;
	while(fgets(str, 256, file)!=NULL && line<len)
	{
		if(sscanf(str, "%f %f %f", &red, &green, &blue)==3)
		{
			colors[line*3]=red;
			colors[(line*3)+1]=green;
			colors[(line*3)+2]=blue;
			line++;
		}
	}
	
	fclose(file);
	
	return colors;
}


pTriangleStrip LoadTriangleStrip(const char* filename)
{
	FILE* file;
	char str[256];
	int len, numpts, line, tempi, tempi2;
	real tempr;
	pTriangleStrip strip=NULL;
	

	file = fopen(filename, "r");
	if (!file)
		return NULL;
	
	if(fgets(str, 256, file)==NULL)
	{
		fclose(file);
		return NULL;
	}
	
	if(sscanf(str, "%i %i", &len, &numpts)!=2)
	{
		fclose(file);
		return NULL;
	}
	
	strip = Create_TriangleStrip(len,numpts);
	
	line=0;
	while(fgets(str, 256, file)!=NULL && line<numpts*3)
	{
		if(sscanf(str, "%f", &tempr)==1)
		{
			strip->points[line]=tempr;
			line++;
		}
	}
	
	line=0;
	while(fgets(str, 256, file)!=NULL && line<len)
	{
		if(sscanf(str, "%i %i", &tempi, &tempi2)==2)
		{
			strip->a_point[line]=tempi;
			strip->action[line]=tempi2;
			line++;
		}
	}
	
	fclose(file);
	
	return strip;
}



void aglDrawTriangle(pVector a, pVector b, pVector c, pVector colora, pVector colorb, pVector colorc)
{
	int ax,ay,bx,by,cx,cy;
	int p1x, p2x;
	int i, itemp;
	pVector vtemp;
	real zcoeff[3], rcoeff[3], gcoeff[3], bcoeff[3];

	/*Ist das Dreieck degeneriert?*/
	
	
	/* Liegen alle Punkte vor dem Betrachter? */
	if(a[2] <= n_min || b[2] <= n_min || c[2] <= n_min || a[2] >= n_max || b[2] >= n_max || c[2] >= n_max)
		return;
	
	/* Ist das Dreieck dem Betrachter zugewand?
	Berechne dazu die 3. Komponente des (nicht normierten) Normalenvektors (a-b)x(a-c)
	und teste ihr Vorzeichen.
	(a-b)1 * (a-c)2 - (a-b)2 * (a-c)1 */
	if((((a[0] - b[0]) * (a[1] - c[1])) - ((a[1] - b[1]) * (a[0] - c[0]))) <= 0.0)
		return;
	
	
	
	if(a[1] > b[1])
	{
		vtemp=b;
		b=a;
		a=vtemp;
		
		vtemp=colorb;
		colorb=colora;
		colora=vtemp;
	}
	
	if(b[1] > c[1])
	{
		vtemp=c;
		c=b;
		b=vtemp;
		
		vtemp=colorc;
		colorc=colorb;
		colorb=vtemp;
		
		if(a[1] > b[1])
		{
			vtemp=b;
			b=a;
			a=vtemp;
			
			vtemp=colorb;
			colorb=colora;
			colora=vtemp;
		}
	}
	

	
	ax=(int)a[0];
	ay=(int)a[1];
	
	bx=(int)b[0];
	by=(int)b[1];
	
	cx=(int)c[0];
	cy=(int)c[1];
	
	
	PlaneCoefficients(a,b,c,zcoeff);
	
	a[2] = colora[0];
	b[2] = colorb[0];
	c[2] = colorc[0];
	PlaneCoefficients(a,b,c,rcoeff);
	
	a[2] = colora[1];
	b[2] = colorb[1];
	c[2] = colorc[1];
	PlaneCoefficients(a,b,c,gcoeff);
	
	a[2] = colora[2];
	b[2] = colorb[2];
	c[2] = colorc[2];
	PlaneCoefficients(a,b,c,bcoeff);
	
	
	if(ay!=by)
	{
		for(i=(ay<=0 ? 0 : ay); i<=by && i<surface_height; i++)
		{
			/*Berechne linken Punkt auf Linie ab */
			p1x = ax + (int)(((real)(i - ay)/(real)(by - ay))*(real)(bx-ax));
			p2x = ax + (int)(((real)(i - ay)/(real)(cy - ay))*(real)(cx-ax));
			if(p2x < p1x)
			{
				itemp=p1x;
				p1x=p2x;
				p2x=itemp;
			}
			
			if(p1x<0)
				p1x=0;
			if(p2x>=surface_width)
				p2x=surface_width-1;
			
			for(; p1x<p2x; p1x++)
			{
				aglSetPixel(p1x, i, EvalPlane((real)p1x, (real)i, zcoeff),
							EvalPlane((real)p1x, (real)i, rcoeff), EvalPlane((real)p1x, (real)i, gcoeff), EvalPlane((real)p1x, (real)i, bcoeff));
				
			}
		}
	}
	
	if(by!=cy)
	{
		for(i=(by<=0 ? 0 : by); i<=cy && i<surface_height; i++)
		{
			p1x = bx + (int)(((real)(i - by)/(real)(cy - by))*(real)(cx-bx));
			p2x = ax + (int)(((real)(i - ay)/(real)(cy - ay))*(real)(cx-ax));
			if(p2x < p1x)
			{
				itemp=p1x;
				p1x=p2x;
				p2x=itemp;
			}
			
			if(p1x<0)
				p1x=0;
			if(p2x>=surface_width)
				p2x=surface_width-1;
			
			for(; p1x<p2x; p1x++)
			{
				aglSetPixel(p1x, i, EvalPlane((real)p1x, (real)i, zcoeff),
							EvalPlane((real)p1x, (real)i, rcoeff), EvalPlane((real)p1x, (real)i, gcoeff), EvalPlane((real)p1x, (real)i, bcoeff));
			}
		}
	}
}



void aglSetPixel(int x, int y, real z, real r, real g, real b)
{
	if(x<0 || x>=surface_width || y<0 || y>=surface_height)
		return;
	
	//EnterCriticalSection(&CriticalSection);
	
	if(zbuffer[(surface_width*y) + x]<=z)
	{
		//LeaveCriticalSection(&CriticalSection);
		return;
	}
	
	if(r<0.0)
		r=0.0;
	else if(r>1.0)
		r=1.0;

	if(g<0.0)
		g=0.0;
	else if(g>1.0)
		g=1.0;
	
	
	if(b<0.0)
		b=0.0;
	else if(b>1.0)
		b=1.0;

	pixels[(surface_width*y) + x] = ((int)(255.0*r))<<16 | ((int)(255.0*g))<<8 | ((int)(255.0*b));
	zbuffer[(surface_width*y) + x] = z;
	
	//LeaveCriticalSection(&CriticalSection);
}



pMesh Create_Mesh(int num_pts, int num_faces)
{
	pMesh mesh = (pMesh)malloc(sizeof(Mesh));


	mesh->num_pts=num_pts;
	mesh->num_faces=num_faces;
	
	mesh->points=Vector_Create(num_pts*3);
	mesh->hom_points=Vector_Create(num_pts*4);
	
	mesh->normals=Vector_Create(num_pts*3);
	mesh->transf_normals=Vector_Create(num_pts*3);
	
	mesh->faces_list=(int *)malloc(num_faces*3*sizeof(int));
	
	
	return mesh;
}

void Destroy_Mesh(pMesh mesh)
{
	free(mesh->faces_list);
	free(mesh->transf_normals);
	free(mesh->normals);
	free(mesh->hom_points);
	free(mesh->points);
	free(mesh);
}





pTriangleStrip Create_TriangleStrip(int len, int num_pts)
{
	pTriangleStrip strip = (pTriangleStrip)malloc(sizeof(TriangleStrip));
	
	strip->len=len;
	strip->num_pts=num_pts;
	strip->points=Vector_Create(num_pts*3);
	strip->hom_points=Vector_Create(num_pts*4);
	strip->action=(int *)malloc(len*sizeof(int));
	strip->a_point=(int *)malloc(len*sizeof(int));
	
	return strip;
}

void Destroy_TriangleStrip(pTriangleStrip strip)
{
	free(strip->a_point);
	free(strip->action);
	Vector_Destroy(strip->hom_points);
	Vector_Destroy(strip->points);
	free(strip);
}


pRenderTree Create_RenderTreeElement()
{
	pRenderTree tree = (pRenderTree)malloc(sizeof(RenderTree));

	tree->sibling=NULL;
	tree->child=NULL;
	
	tree->mesh=NULL;
	tree->strip=NULL;

	return tree;
}

void Destroy_RenderTreeElement(pRenderTree tree)
{
	free(tree);
}

pRenderTree Add_Tree_Child(pRenderTree tree)
{
	pRenderTree child = Create_RenderTreeElement();

	if(tree->child==NULL)
	{
		tree->child=child;
	}
	else
	{
		pRenderTree temptree=tree->child;
		while(temptree->sibling!=NULL)
		{
			temptree=temptree->sibling;
		}
		temptree->sibling=child;
	}
	
	return child;
}


void doRenderTree(pRenderTree tree)
{
	pRenderTree tempt=tree->child;
	int i;
	
	
	while(tempt!=NULL)
	{
		MatrixMult4x4_nocopy(tempt->m_temp, tree->m_temp, tempt->m_current);
		
		if(tempt->mesh!=NULL)
		{
			for(i=0; i<tempt->mesh->num_pts; i++)
			{
				MatrixVectorMult4x3_Project(tempt->mesh->hom_points+(i*4), tempt->m_temp, tempt->mesh->points+(i*3));
				MatrixVectorMult3x3(tempt->mesh->transf_normals+(i*3), tempt->m_temp, tempt->mesh->normals+(i*3));
				
				tempt->mesh->hom_points[i*4] = xres*0.5 + tempt->mesh->hom_points[i*4] * ((xres)/(2.0*tan(fov2)*tempt->mesh->hom_points[(i*4)+2]));
				tempt->mesh->hom_points[(i*4)+1] = yres*0.5 + tempt->mesh->hom_points[(i*4)+1] * ((yres)/(2.0*tan(fov2*aspect_ratio)*tempt->mesh->hom_points[(i*4)+2]));
			}
			
			Render_Mesh(tempt->mesh, tempt->colors);
		}
		else if(tempt->strip!=NULL)
		{
			for(i=0; i<tempt->strip->num_pts; i++)
			{
				MatrixVectorMult4x3_Project(tempt->strip->hom_points+(i*4), tempt->m_temp, tempt->strip->points+(i*3));
				
				tempt->strip->hom_points[i*4] = xres*0.5 + tempt->strip->hom_points[i*4] * ((xres)/(2.0*tan(fov2)*tempt->strip->hom_points[(i*4)+2]));
				tempt->strip->hom_points[(i*4)+1] = yres*0.5 + tempt->strip->hom_points[(i*4)+1] * ((yres)/(2.0*tan(fov2*aspect_ratio)*tempt->strip->hom_points[(i*4)+2]));
			}
			Render_TStrip(tempt->strip, tempt->colors);
		}
		
		doRenderTree(tempt);
		tempt=tempt->sibling;
	}
}


void Render_TStrip(pTriangleStrip strip, pVector colors)
{
	int i;
	int old, middle=0, young=0;
	
	for(i=0; i<strip->len; i++)
	{
		switch(strip->action[i])
		{
			case 0:
				old=middle;
				middle=young;
				young=strip->a_point[i];
			break;
			
			case 1:
				old=middle;
				middle=young;
				young=strip->a_point[i];
				AssignTask_RasterTriangle(strip->hom_points+(4*old), strip->hom_points+(4*middle), strip->hom_points+(4*young),
											colors+(3*old), colors+(3*middle), colors+(3*young));
			break;
			
			case 2:
				old=young;
				young=strip->a_point[i];
				AssignTask_RasterTriangle(strip->hom_points+(4*old), strip->hom_points+(4*middle), strip->hom_points+(4*young),
											colors+(3*old), colors+(3*middle), colors+(3*young));
			break;
			
			case 3:
				middle=young;
				young=strip->a_point[i];
				AssignTask_RasterTriangle(strip->hom_points+(4*old), strip->hom_points+(4*middle), strip->hom_points+(4*young),
											colors+(3*old), colors+(3*middle), colors+(3*young));
			break;
		}
	}
}


void Render_Mesh(pMesh mesh, pVector colors)
{
	int i;
	
	for(i=0; i<mesh->num_faces; i++)
	{
		AssignTask_RasterTriangle(mesh->hom_points+(4*mesh->faces_list[i*3]), mesh->hom_points+(4*mesh->faces_list[1+i*3]), mesh->hom_points+(4*mesh->faces_list[2+i*3]),
									colors+(3*mesh->faces_list[i*3]), colors+(3*mesh->faces_list[1+i*3]), colors+(3*mesh->faces_list[2+i*3]));
	}
}
