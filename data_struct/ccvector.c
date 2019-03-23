#include "ccvector.h"
#include "common.h"
#include <stdlib.h>
int VecCreate(CC_VECTOR **Vector)
{
	if (Vector == NULL) {
		return -1;
	}
	(*Vector) = (CC_VECTOR*)malloc(sizeof(CC_VECTOR));
	if ((*Vector) == NULL) goto cleanup;
	(*Vector)->size = 0;
	(*Vector)->capacity = 10;
	(*Vector)->elems = (int*)malloc(10 * sizeof(int));
	if ((*Vector)->elems == NULL) goto cleanup;
    //CC_UNREFERENCED_PARAMETER(Vector);
    return 0;
cleanup:
	if (*Vector != NULL) {
		if ((*Vector)->elems != NULL) {
			free((*Vector)->elems);
		}
		free(*Vector);
	}
	return -1;
}

int VecDestroy(CC_VECTOR **Vector)
{
    //CC_UNREFERENCED_PARAMETER(Vector);
	if (*Vector == NULL) {
		return -1;
	}	
	free((*Vector)->elems);
	free(*Vector);
	return 0;
}

static int resize(CC_VECTOR* Vector) {
	
	int *aux = (int*)malloc(Vector->capacity * 2 * sizeof(int));
	if (aux == NULL) {
		return -1;
	}
	else {
		int i = 0;
		for (i = 0; i < Vector->size; i++) {
			aux[i] = Vector->elems[i];
		}
	}
	Vector->capacity *= 2;
	free(Vector->elems);
	Vector->elems = aux;
	return 0;

}



int VecInsertTail(CC_VECTOR *Vector, int Value)
{
	if (Vector == NULL) {
		return -1;
	}
	if (Vector->size == Vector->capacity) {
		if (resize(Vector) != 0) return -1;
	}
	Vector->elems[Vector->size] = Value;
	Vector->size += 1;	
    return 0;
}

int VecInsertHead(CC_VECTOR *Vector, int Value)
{
	if (Vector == NULL) {
		return -1;
	}
	int i = 0;
	if (Vector->size == Vector->capacity) {
		if (resize(Vector) != 0) return -1;
	}
	for (i = Vector->size; i > 0; i--) {
		Vector->elems[i] = Vector->elems[i - 1];
	}
	Vector->elems[0] = Value;
	Vector->size += 1;
    return 0;
}


int VecInsertAfterIndex(CC_VECTOR *Vector, int Index, int Value)
{
	if (Vector == NULL) {
		return -1;
	}
	if (Index < 0 || Index > Vector->size) {
		return -1;
	}
	if (Vector->size == Vector->capacity) {
		if (resize(Vector) != 0) return -1;
	}
	int i = 0;
	for (i = Vector->size; i > Index; i--) {
		Vector->elems[i] = Vector->elems[i - 1];
	}
	Vector->elems[Index] = Value;
	Vector->size++;
	return 0;
}

int VecRemoveByIndex(CC_VECTOR *Vector, int Index)
{
	if (Vector == NULL) {
		return -1;
	}
	if (Index < 0 || Index >= Vector->size) {
		return -1;
	}

	int i = 0;
	for (i = Index; i < Vector->size; i++) {
		Vector->elems[i] = Vector->elems[i + 1];
	}
	Vector->size--;

    return 0;
}

int VecGetValueByIndex(CC_VECTOR *Vector, int Index, int *Value)
{
	if (Vector == NULL) {
		return -1;
	}
	if (Index < 0 || Index >= Vector->size) {
		return -1;
	}
	*Value = Vector->elems[Index];
    return 0;
}

int VecGetCount(CC_VECTOR *Vector)
{
	if (Vector == NULL) {
		return -1;
	}
	return Vector->size;
}

int VecClear(CC_VECTOR *Vector)
{
	if (Vector == NULL) {
		return -1;
	}
	Vector->size = 0;
    return 0;
}

int VecSort(CC_VECTOR *Vector)
{
	if (Vector == NULL) {
		return -1;
	}
	int i, j;
	for (i = 0; i < Vector->size; i++) {
		for (j = i; j < Vector->size; j++) {
			if (Vector->elems[i] > Vector->elems[j]) {
				int aux = Vector->elems[i];
				Vector->elems[i] = Vector->elems[j];
				Vector->elems[j] = aux;
			}
		}
	}
	return 0;
}
