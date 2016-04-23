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

//#include <stdio.h>
#include <math.h>
#include "vector_matrix.h"
#include "3d_helper.h"




void MatrixLoadIdentity(real * M)
{
	M[0]= 1.0; M[1]= 0.0; M[2]= 0.0; M[3]= 0.0;
	M[4]= 0.0; M[5]= 1.0; M[6]= 0.0; M[7]= 0.0;
	M[8]= 0.0; M[9]= 0.0; M[10]=1.0; M[11]=0.0;
	M[12]=0.0; M[13]=0.0; M[14]=0.0; M[15]=1.0;
}


void MatrixLoadScaling(real * M, real x, real y, real z)
{
	M[0]= x; M[1]= 0.0; M[2]= 0.0; M[3]= 0.0;
	M[4]= 0.0; M[5]= y; M[6]= 0.0; M[7]= 0.0;
	M[8]= 0.0; M[9]= 0.0; M[10]=z; M[11]=0.0;
	M[12]=0.0; M[13]=0.0; M[14]=0.0; M[15]=1.0;
}

void MatrixLoadTranslation(real * M, real x, real y, real z)
{
	M[0]= 1.0; M[1]= 0.0; M[2]= 0.0; M[3]= x;
	M[4]= 0.0; M[5]= 1.0; M[6]= 0.0; M[7]= y;
	M[8]= 0.0; M[9]= 0.0; M[10]=1.0; M[11]=z;
	M[12]=0.0; M[13]=0.0; M[14]=0.0; M[15]=1.0;
}


void MatrixLoadXRotation(real * M, real a)
{
	M[0]= 1.0; M[1]= 0.0; M[2]=    0.0; M[3]=           0.0;
	M[4]= 0.0; M[5]= cos(a); M[6]= (-1.0)*sin(a); M[7]= 0.0;
	M[8]= 0.0; M[9]= sin(a); M[10]=cos(a); M[11]=       0.0;
	M[12]=0.0; M[13]=0.0; M[14]=   0.0; M[15]=          1.0;
}

void MatrixLoadYRotation(real * M, real a)
{
	M[0]= cos(a); M[1]= 0.0; M[2]= sin(a); M[3]= 0.0;
	M[4]= 0.0; M[5]= 1.0; M[6]= 0.0; M[7]= 0.0;
	M[8]= (-1.0)*sin(a); M[9]= 0.0; M[10]=cos(a); M[11]=0.0;
	M[12]=0.0; M[13]=0.0; M[14]=0.0; M[15]=1.0;
}


void MatrixLoadZRotation(real * M, real a)
{
	M[0]= cos(a); M[1]= (-1.0)*sin(a); M[2]= 0.0; M[3]= 0.0;
	M[4]= sin(a); M[5]= cos(a); M[6]= 0.0; M[7]= 0.0;
	M[8]= 0.0; M[9]= 0.0; M[10]=1.0; M[11]=0.0;
	M[12]=0.0; M[13]=0.0; M[14]=0.0; M[15]=1.0;
}




void MatrixMult4x4(real * target, real * left, real * right)
{
	int i;
	real tempm[16];
	
	for(i=0; i<16; i++)
	{
		tempm[i] = left[(i&0xFFFFFFFC)] * right[(i&0x3)] + left[(i&0xFFFFFFFC)+1] * right[(i&0x3)+4]
					 + left[(i&0xFFFFFFFC)+2] * right[(i&0x3)+8] + left[(i&0xFFFFFFFC)+3] * right[(i&0x3)+12];
	}
	
	for(i=0; i<16; i++)
	{
		target[i] = tempm[i];
	}
}

void MatrixMult4x4_nocopy(real * target, real * left, real * right)
{
	int i;
	
	for(i=0; i<16; i++)
	{
		target[i] = left[(i&0xFFFFFFFC)] * right[(i&0x3)] + left[(i&0xFFFFFFFC)+1] * right[(i&0x3)+4]
					 + left[(i&0xFFFFFFFC)+2] * right[(i&0x3)+8] + left[(i&0xFFFFFFFC)+3] * right[(i&0x3)+12];
	}
}


/*void MatrixCopy4x4(real * target, real * dest)
{
	int i;
	
	for(i=0; i<16; i++)
	{
		target[i] = dest[i];
	}
}*/


void MatrixVectorMult4x3_Project(pVector target, real * M, pVector v)
{
	target[3] = M[12] * v[0] + M[13] * v[1]
				+ M[14] * v[2] + M[15];
	target[0] = (M[0] * v[0] + M[1] * v[1]
				+ M[2] * v[2] + M[3])/target[3];
	target[1] = (M[4] * v[0] + M[5] * v[1]
				+ M[6] * v[2] + M[7])/target[3];
	target[2] = (M[8] * v[0] + M[9] * v[1]
				+ M[10] * v[2] + M[11])/target[3];
	
	target[3]=1.0;
}

void MatrixVectorMult3x3(pVector target, real * M, pVector v)
{
	target[0] = M[0] * v[0] + M[1] * v[1] + M[2] * v[2];
	target[1] = M[4] * v[0] + M[5] * v[1] + M[6] * v[2];
	target[2] = M[8] * v[0] + M[9] * v[1] + M[10] * v[2];
}





void Create_WC_to_CC_Matrix(real * M, pVector cam, pVector lookat, pVector vup)
{
	real rtemp;
	
	/* Compute n */
	M[8]=lookat[0]-cam[0];
	M[9]=lookat[1]-cam[1];
	M[10]=lookat[2]-cam[2];
	
	rtemp = sqrt(M[8]*M[8] + M[9]*M[9] + M[10]*M[10]);
	
	M[8]=M[8]/rtemp;
	M[9]=M[9]/rtemp;
	M[10]=M[10]/rtemp;
	
	//printf("n=(%f, %f, %f)\r\n",M[8],M[9],M[10]);
	
	/* Compute v */
	rtemp = M[8]*vup[0] + M[9]*vup[1] + M[10]*vup[2]; /* n*vup */
	
	M[4] = vup[0] - rtemp*M[8];
	M[5] = vup[1] - rtemp*M[9];
	M[6] = vup[2] - rtemp*M[10];
	
	rtemp = sqrt(M[4]*M[4] + M[5]*M[5] + M[6]*M[6]);
	M[4]=M[4]/rtemp;
	M[5]=M[5]/rtemp;
	M[6]=M[6]/rtemp;
	
	//printf("v=(%f, %f, %f)\r\n",M[4],M[5],M[6]);
	
	
	/* Compute b */
	M[0]= M[9]*M[6] - M[10]*M[5];
	M[1]= M[10]*M[4] - M[8]*M[6];
	M[2]= M[8]*M[5] - M[9]*M[4];
	
	
	//printf("b=(%f, %f, %f)\r\n",M[0],M[1],M[2]);
	
	
	M[3] = (-1.0)*(cam[0]*M[0] + cam[1]*M[1] + cam[2]*M[2]);
	M[7] = (-1.0)*(cam[0]*M[4] + cam[1]*M[5] + cam[2]*M[6]);
	M[11] = (-1.0)*(cam[0]*M[8] + cam[1]*M[9] + cam[2]*M[10]);
	
	//printf("t=(%f, %f, %f)\r\n",M[3],M[7],M[11]);
	/*M[3]=0.0;
	M[7]=0.0;
	M[11]=0.0;*/
	
	M[12]=0.0;
	M[13]=0.0;
	M[14]=0.0;
	M[15]=1.0;
}




real EvalPlane(real x, real y, pVector coeff)
{
	return (coeff[0]*x) + (coeff[1]*y) + coeff[2];
}

void PlaneCoefficients(pVector u1, pVector u2, pVector u3, pVector result)
{
	real C;
	
	C = (u2[1]-u1[1])*(u3[0]-u1[0]) - (u2[0]-u1[0])*(u3[1]-u1[1]);
	result[0] = ((u2[1]-u1[1])*(u3[2]-u1[2]) - (u2[2]-u1[2])*(u3[1]-u1[1])) / C;
	result[1] = ((u2[2]-u1[2])*(u3[0]-u1[0]) - (u2[0]-u1[0])*(u3[2]-u1[2])) / C;
	result[2] = u1[2] - result[0]*u1[0] - result[1]*u1[1];
}
