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

#ifndef INC_VECTOR_MATRIX
#define INC_VECTOR_MATRIX

typedef float real;

typedef real * pVector;

typedef pVector * pMatrix;


pVector Vector_Create(int);
void Vector_Destroy(pVector);
//void Vector_Report(pVector, int);

pMatrix Matrix_Create(int, int);
void Matrix_Destroy(pMatrix, int, int);
//void Matrix_Report(pMatrix, int, int);

#endif /* INC_VECTOR_MATRIX */
