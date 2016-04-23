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
#include <stdlib.h>
#include "vector_matrix.h"


pVector Vector_Create(int n)
{
	return (pVector)malloc(n*sizeof(real));
}


void Vector_Destroy(pVector v)
{
	free(v);
}

/*void Vector_Report(pVector v, int n)
{
	int i;
	
	printf("( ");
	
	for(i=0; i<n; i++)
		printf("%12.6f ", v[i]);
	
	printf(")\r\n");
}*/

pMatrix Matrix_Create(int m, int n)
{
	pMatrix A;
	int i;
	
	A = (pMatrix)malloc(m*sizeof(pVector));
	
	for(i=0; i<m; i++)
		A[i] = Vector_Create(n);
	
	return A;
}


void Matrix_Destroy(pMatrix A, int m, int n)
{
	int i;
	
	for(i=0; i<m; i++)
		Vector_Destroy(A[i]);
	
	free(A);
}


/*void Matrix_Report(pMatrix A, int m, int n)
{
	int i;
	
	for(i=0; i<m; i++)
		Vector_Report(A[i], n);
}*/
