#include "ccheap.h"
#include "common.h"
#include <stdlib.h>

static int CmpMax(int a, int b) {
	return a > b;
}
static int CmpMin(int a, int b) {
	return a < b;
}


static int HeapifyUp(CC_HEAP* Heap,int Pos) {
	int i = Pos;
	while (i > 0 && Heap->Cmp(Heap->Elems[i],Heap->Elems[(i-1)/2])) {
		int aux = Heap->Elems[i];
		Heap->Elems[i] = Heap->Elems[(i - 1) / 2];
		Heap->Elems[(i - 1) / 2] = aux;
		i = (i - 1) / 2;
	}
	return 0;
}

static int HeapifyDown(CC_HEAP* Heap,int pos) {
	int i = pos;
	while (i < Heap->Size && (Heap->Cmp(Heap->Elems[2*i + 1],Heap->Elems[i]) || Heap->Cmp(Heap->Elems[2 * i + 2], Heap->Elems[i]))) {
		if (Heap->Cmp(Heap->Elems[2 * i + 1], Heap->Elems[2 * i + 2])) {
			int aux = Heap->Elems[i];
			Heap->Elems[i] = Heap->Elems[2 * i + 1];
			Heap->Elems[2 * i + 1] = aux;
			i = 2 * i + 1;
		}
		else {
			int aux = Heap->Elems[i];
			Heap->Elems[i] = Heap->Elems[2 * i + 2];
			Heap->Elems[2 * i + 2] = aux;
			i = 2 * i + 2;
		}
	}
	return 0;
}

static int Resize(CC_HEAP* Heap) {
	int* ElemsAux = (int*)malloc(Heap->Capacity * 2 * sizeof(int));
	if (ElemsAux == NULL) {
		return -1;
	}
	int i;
	for (i = 0; i < Heap->Size; i++) {
		ElemsAux[i] = Heap->Elems[i];
	}
	free(Heap->Elems);
	Heap->Capacity *= 2;
	Heap->Elems = ElemsAux;
	return 0;
}
int HpCreateMaxHeap(CC_HEAP **MaxHeap, CC_VECTOR* InitialElements)
{
	if (MaxHeap == NULL) {
		return -1;
	}
	*MaxHeap = (CC_HEAP*)malloc(sizeof(CC_HEAP));
	if (*MaxHeap == NULL) {
		return -1;
	}
	if (InitialElements == NULL) {
		(*MaxHeap)->Elems = (int*)malloc(20 * sizeof(int));
		if ((*MaxHeap)->Elems == NULL) {
			free(*MaxHeap);
			return -1;
		}
		(*MaxHeap)->Capacity = 20;
		(*MaxHeap)->Cmp = CmpMax;
		(*MaxHeap)->Size = 0;
	}
	else {
		
		(*MaxHeap)->Elems = (int*)malloc(VecGetCount(InitialElements)*2* sizeof(int));
		if ((*MaxHeap)->Elems == NULL) {
			free(*MaxHeap);
			return -1;
		}
		//TODO Add Elems from vector
		(*MaxHeap)->Capacity = 20;
		(*MaxHeap)->Cmp = CmpMax;
		(*MaxHeap)->Size = 0;

	}
	return 0;
}

int HpCreateMinHeap(CC_HEAP **MinHeap, CC_VECTOR* InitialElements)
{
	if (MinHeap == NULL) {
		return -1;
	}
	*MinHeap = (CC_HEAP*)malloc(sizeof(CC_HEAP));
	if (*MinHeap == NULL) {
		return -1;
	}
	if (InitialElements == NULL) {
		(*MinHeap)->Elems = (int*)malloc(20 * sizeof(int));
		if ((*MinHeap)->Elems == NULL) {
			free(*MinHeap);
			return -1;
		}
		(*MinHeap)->Capacity = 20;
		(*MinHeap)->Cmp = CmpMin;
		(*MinHeap)->Size = 0;
	}
	else {
		(*MinHeap)->Elems = (int*)malloc(VecGetCount(InitialElements)*2 * sizeof(int));
		if ((*MinHeap)->Elems == NULL) {
			free(*MinHeap);
			return -1;
		}
		//TODO add element from vector;
		(*MinHeap)->Capacity = 20;
		(*MinHeap)->Cmp = CmpMin;
		(*MinHeap)->Size = 0;

	}
	return 0;
}

int HpDestroy(CC_HEAP **Heap)
{
	if (Heap == NULL || *Heap == NULL) {
		return -1;
	}
	free((*Heap)->Elems);
	free(*Heap);
    return 0;
}

int HpInsert(CC_HEAP *Heap, int Value)
{
	if (Heap == NULL) {
		return -1;
	}
	if (Heap->Size == Heap->Capacity) {
		if (Resize(Heap) == -1) {
			return -1;
		}
	}
	Heap->Elems[Heap->Size] = Value;
	HeapifyUp(Heap,Heap->Size);
	Heap->Size++;
	return 0;
}

int HpRemove(CC_HEAP *Heap, int Value)
{
	if (Heap == NULL) {
		return -1;
	}
	int i = 0;
	for (i = 0; i < Heap->Size; i++) {
		if (Heap->Elems[i] == Value) {
			Heap->Elems[i] = Heap->Elems[Heap->Size - 1];
			HeapifyDown(Heap, i);
			HeapifyUp(Heap, i);
		}
	}
    return 0;
}

int HpGetExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
	if (Heap == NULL || ExtremeValue == NULL) {
		return -1;
	}
	else {
		*ExtremeValue = Heap->Elems[0];
	}
	return 0;
}

int HpPopExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
	if (Heap == NULL || ExtremeValue == NULL) {
		return -1;
	}
	else {
		*ExtremeValue = Heap->Elems[0];
		while (Heap->Elems[0] == *ExtremeValue) {
			Heap->Elems[0] = Heap->Elems[Heap->Size - 1];
			Heap->Size--;
			HeapifyDown(Heap,0);
		}
	}
    return 0;
}

int HpGetElementCount(CC_HEAP *Heap)
{
	if (Heap == NULL) {
		return -1;
	}
    return Heap->Size;
}

static int CopyHeap(CC_HEAP* Heap, CC_HEAP** newHeap) {
	*newHeap = (CC_HEAP*)malloc(sizeof(CC_HEAP));
	if (*newHeap == NULL) {
		return 1;
	}
	(*newHeap)->Elems = (int*)malloc(sizeof(int));
	if ((*newHeap)->Elems == NULL) {
		return -1;
	}
	(*newHeap)->Capacity = Heap->Size;
	(*newHeap)->Size = Heap->Size;
	(*newHeap)->Cmp = Heap->Cmp;
	int i = 0;
	for (i = 0; i < Heap->Size; i++) {
		(*newHeap)->Elems[i] = Heap->Elems[i];
	}
	return 0;
}

int HpSortToVector(CC_HEAP *Heap, CC_VECTOR* SortedVector)
{
	if (Heap == NULL || SortedVector == NULL) {
		return -1;
	}
	CC_HEAP* heapCopy;
	if (CopyHeap(Heap, &heapCopy)==-1) {
		return -1;
	}
	//find if the heap is a minHeap or a maxHeap
	//if it's a minHeap var min will be equal to 1
	// 0 otherwise
	int min = 0;
	if (Heap->Cmp(1, 2)) {
		min = 1;
	}
	int aux;
	while (heapCopy->Size > 0) {
		HpPopExtreme(heapCopy, &aux);
		if (min == 1) {
			VecInsertHead(SortedVector, aux);
		}
		else {
			VecInsertTail(SortedVector, aux);
		}
	}
	free(heapCopy);
	return 0;
}
