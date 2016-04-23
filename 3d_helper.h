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

#ifndef INC_3DHELPER
#define INC_3DHELPER



 #define crossProduct(a,b,c) \
	(a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
	(a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
	(a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];


#define dotProduct(v,q) \
		((v)[0] * (q)[0] + \
		(v)[1] * (q)[1] + \
		(v)[2] * (q)[2])	



void MatrixLoadIdentity(real * M);
void MatrixLoadScaling(real * M, real x, real y, real z);
void MatrixLoadTranslation(real * M, real x, real y, real z);
void MatrixLoadXRotation(real * M, real a);
void MatrixLoadYRotation(real * M, real a);
void MatrixLoadZRotation(real * M, real a);

void MatrixMult4x4(real * target, real * left, real * right);
void MatrixMult4x4_nocopy(real * target, real * left, real * right);
void MatrixVectorMult4x3_Project(pVector target, real * M, pVector v);
void MatrixVectorMult3x3(pVector target, real * M, pVector v);
//void MatrixCopy4x4(real * target, real * dest);


void Create_WC_to_CC_Matrix(real * M, pVector cam, pVector lookat, pVector vup);

real EvalPlane(real x, real y, pVector coeff);
void PlaneCoefficients(pVector u1, pVector u2, pVector u3, pVector result);


#endif
