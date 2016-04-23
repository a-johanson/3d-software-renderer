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

#ifndef INC_ENGINE
#define INC_ENGINE


#include "vector_matrix.h"


#define WIDTH 1280
#define HEIGHT 720
#define SIZE WIDTH*HEIGHT



struct _TriangleStrip {
int len;
int num_pts;
pVector points;
pVector hom_points;
int * action;
int * a_point;
};

typedef struct _TriangleStrip TriangleStrip;
typedef TriangleStrip * pTriangleStrip;




struct _Mesh {
int num_pts;
int num_faces;

pVector points;
pVector hom_points;

//pVector normals; /*Eckennormalen*/
//pVector transf_normals;

int * faces_list;
};

typedef struct _Mesh Mesh;
typedef Mesh * pMesh;




struct _RenderTree {
real m_current[16];
real m_temp[16];
pTriangleStrip strip;
pMesh mesh;
pVector colors;
//pPhongMaterial phong_material;
real r[9];
int exponent;

struct _RenderTree * sibling;
struct _RenderTree * child;
};

typedef struct _RenderTree RenderTree;
typedef RenderTree * pRenderTree;







void RenderScene();
void PrepareScene(int bg_color);

pTriangleStrip LoadTriangleStrip(const char* filename);
pVector LoadColorFile(const char* filename);



void aglDrawTriangle(pVector a, pVector b, pVector c, pVector colora, pVector colorb, pVector colorc);
void aglSetPixel(int x, int y, real z, real r, real g, real b);

pMesh Create_Mesh(int num_pts, int num_faces);
void Destroy_Mesh(pMesh mesh);

pTriangleStrip Create_TriangleStrip(int len, int num_pts);
void Destroy_TriangleStrip(pTriangleStrip strip);


pRenderTree Create_RenderTreeElement();
void Destroy_RenderTreeElement(pRenderTree tree);
pRenderTree Add_Tree_Child(pRenderTree tree);
void doRenderTree(pRenderTree tree);


void Render_TStrip(pTriangleStrip strip, pVector colors);
void Render_Mesh(pMesh mesh, pVector colors);


#endif
