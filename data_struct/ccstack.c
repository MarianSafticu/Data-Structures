#include "ccstack.h"
#include "common.h"
#include <stdlib.h>

int StCreate(CC_STACK **Stack)
{
	if (Stack == NULL) {
		return -1;
	}
	*Stack = (CC_STACK*)malloc(sizeof(CC_STACK));
	if (*Stack == NULL) {
		return -1;
	}
	(*Stack)->First = NULL;
	(*Stack)->Last = NULL;
	(*Stack)->Size = 0;
    return 0;
}

int StDestroy(CC_STACK **Stack)
{
	if (Stack == NULL || *Stack == NULL) {
		return -1;
	}
	if ((*Stack)->First != NULL) {
		StClear(*Stack);
	}
	free(*Stack);
    return 0;
}

int StPush(CC_STACK *Stack, int Value)
{
	if (Stack == NULL) {
		return -1;
	}
	PSTACK_NODE nou = (PSTACK_NODE)malloc(sizeof(STACK_NODE));
	if (nou == NULL) {
		return -1;
	}
	nou->Elem = Value;
	nou->Next = NULL;
	nou->Prev = NULL;

	if (Stack->First == NULL) {
		Stack->First = nou;
		Stack->Last = nou;
	}
	else {
		//insert at the begining of the list
		nou->Next = Stack->First;
		Stack->First->Prev = nou;
		Stack->First = nou;
	}
	Stack->Size++;

    return 0;
}

int StPop(CC_STACK *Stack, int *Value)
{
	if (Stack == NULL || Value == NULL) {
		return -1;
	}
	*Value = Stack->First->Elem;
	if (Stack->First->Prev != NULL) {
		Stack->First = Stack->First->Prev;
		free(Stack->First->Next);
	}
	else {
		free(Stack->First);
		Stack->First = NULL;
	}
	Stack->Size--;
    return 0;
}

int StPeek(CC_STACK *Stack, int *Value)
{
	if (Stack == NULL || Value == NULL) {
		return -1;
	}
	*Value = Stack->First->Elem;
    return 0;
}

int StIsEmpty(CC_STACK *Stack)
{
	if (Stack == NULL) {
		return -1;
	}
    return Stack->Size == 0; 
}

int StGetCount(CC_STACK *Stack)
{
	if (Stack == NULL) {
		return -1;
	}
	return Stack->Size;
}

int StClear(CC_STACK *Stack)
{
	//If the pointer is invalid or the stack is already empty
	if (Stack == NULL || Stack->First == NULL) {
		return -1;
	}
	while (Stack->First != NULL) {
		if (Stack->First->Next == NULL) {
			free(Stack->First);
			Stack->First = NULL;
			Stack->Last = NULL;
			Stack->Size = 0;
		}
		else {
			Stack->First = Stack->First->Next;
			free(Stack->First->Prev);
			Stack->First->Prev = NULL;
		}
	}
	return 0;
}

int StPushStack(CC_STACK *Stack, CC_STACK *StackToPush)
{
	if (Stack == NULL || StackToPush == NULL) {
		return -1;
	}
	//In the list I allways add elements in front
	//I could append the second Stack to the first just by setting
	//the last node from the second stack to the first node of the first Stack ( and prev of the first 
	//element of fisrt stack to the last elem of the second
	//then just setting the fisrt elem of the first stack to the first element of the second
	//and setting second stack to an empty one.

	Stack->First->Prev = StackToPush->Last;
	StackToPush->Last->Next = Stack->First;
	Stack->First = StackToPush->First;
	Stack->Size += StackToPush->Size;

	StackToPush->First = NULL;
	StackToPush->Last = NULL;
	StackToPush->Size = 0;
	return 0;
}
