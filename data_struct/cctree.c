#include "cctree.h"
#include "common.h"
#include <stdlib.h>

int TreeCreate(CC_TREE **Tree)
{
	if (Tree == NULL) {
		return -1;
	}
	*Tree = (CC_TREE*)malloc(sizeof(CC_TREE));
	if (*Tree == NULL) {
		return -1;
	}
	(*Tree)->Root = NULL;
	(*Tree)->Size = 0;
	return 0;
}


int TreeDestroy(CC_TREE **Tree)
{
	if (Tree == NULL || *Tree == NULL) {
		return -1;
	}
	TreeClear(*Tree);
	free(*Tree);
    return 0;
}

int RecalculateHeight(PTREE_NODE Root) {
	if (Root == NULL) {
		return 0;
	}
	int hl, hr;
	if (Root->Left == NULL) {
		hl = -1;
	}
	else {
		hl = RecalculateHeight(Root->Left);
	}
	if (Root->Right == NULL) {
		hr = -1;
	}
	else {
		hr = RecalculateHeight(Root->Right);
	}

	if (hr > hl) {
		Root->Height = hr+1;
	}
	else {
		Root->Height = hl+1;
	}
	return Root->Height;
}

int RotatieStanga(CC_TREE *Tree) {
	if (Tree == NULL) {
		return -1;
	}
	PTREE_NODE parentR = Tree->Root->Parent;
	Tree->Root->Parent = Tree->Root->Right;
	Tree->Root->Right->Parent = parentR;

	Tree->Root->Right = Tree->Root->Right->Left;
	Tree->Root->Parent->Left = Tree->Root;
	Tree->Root = Tree->Root->Parent;
	RecalculateHeight(Tree->Root);
	return 0;
}
int RotatieDreapta(CC_TREE *Tree) {
	if (Tree == NULL) {
		return -1;
	}
	PTREE_NODE parentR = Tree->Root->Parent;
	Tree->Root->Parent = Tree->Root->Left;
	Tree->Root->Left->Parent = parentR;
	
	Tree->Root->Left = Tree->Root->Left->Right;
	Tree->Root->Parent->Right = Tree->Root;
	Tree->Root = Tree->Root->Parent;
	RecalculateHeight(Tree->Root);
	return 0;
}
int RotatieDublaStanga(CC_TREE *Tree) {
	CC_TREE* subDrept = (CC_TREE*)malloc (sizeof(CC_TREE));
	subDrept->Root = Tree->Root->Right;
	RotatieDreapta(subDrept);
	RotatieStanga(Tree);
	RecalculateHeight(Tree->Root);
	free(subDrept);
	return 0;
}
int RotatieDublaDreapta(CC_TREE *Tree) {
	CC_TREE* subStang = (CC_TREE*)malloc(sizeof(CC_TREE));
	subStang->Root = Tree->Root->Left;
	RotatieStanga(subStang);
	Tree->Root->Left = subStang->Root;
	RotatieDreapta(Tree);
	RecalculateHeight(Tree->Root);
	free(subStang);
	return 0;
}

//this methods are static because I only use them in rotations and 
//because of that I don't calculate size of the subTree, so it could return false information
//if it's use external
static int GetSubLeft(CC_TREE** SubTree,CC_TREE* Tree) {
	if (SubTree == NULL) {
		return -1;
	}
	*SubTree = (CC_TREE*)malloc(sizeof(CC_TREE));
	if ((*SubTree) == NULL) {
		return -1;
	}
	(*SubTree)->Root = Tree->Root->Left;
	
}
static int GetSubRight(CC_TREE** SubTree,CC_TREE* Tree) {
	if (SubTree == NULL) {
		return -1;
	}
	*SubTree = (CC_TREE*)malloc(sizeof(CC_TREE));
	if ((*SubTree) == NULL) {
		return -1;
	}
	(*SubTree)->Root = Tree->Root->Right;
	
}
int Rebalance(CC_TREE *Tree) {
	if (Tree == NULL) {
		return -1;
	}
	int hlc,hrc; // height for left and right child
	if (Tree->Root == NULL) {
		return 0;
	}
	if (Tree->Root->Left == NULL) {
		hlc = -1;
	}
	else {
		hlc = Tree->Root->Left->Height;
	}
	if (Tree->Root->Right == NULL) {
		hrc = -1;
	}
	else {
		hrc = Tree->Root->Right->Height;
	}
	//int diffLeft=0, diffRight=0; //differences between left and right node heights for left and right childs
	if (hlc - hrc == 0 || hlc - hrc == 1 || hlc - hrc == -1) {
		return 0;
	}
	else {
		if (hlc - hrc == 2) {
			int hllc, hlrc;
			if (Tree->Root->Left->Left == NULL){
				hllc = 0;
			}
			else {
				hllc = Tree->Root->Left->Left->Height;
			}
			if (Tree->Root->Left->Right == NULL) {
				hlrc = 0;
			}
			else {
				hlrc = Tree->Root->Left->Right->Height;
			}
			if (hllc - hlrc == -1) {
				RotatieDublaDreapta(Tree);
			}
			else {
				RotatieDreapta(Tree);
			}
		}


		if (hlc - hrc == -2) {
			int hrlc, hrrc;
			if (Tree->Root->Right->Left == NULL) {
				hrlc = 0;
			}
			else {
				hrlc = Tree->Root->Right->Left->Height;
			}
			if (Tree->Root->Right->Right == NULL) {
				hrrc = 0;
			}
			else {
				hrrc = Tree->Root->Right->Right->Height;
			}
			if (hrlc - hrrc == 1) {
				RotatieDublaStanga(Tree);
			}
			else {
				RotatieStanga(Tree);
			}
		}
	}

	return 0;
}
int TreeInsert(CC_TREE *Tree, int Value)
{
	if (Tree == NULL) {
		return -1;
	}
	PTREE_NODE nou = (PTREE_NODE)malloc(sizeof(TREE_NODE));
	nou->Height = 0;
	nou->Left = NULL;
	nou->Right = NULL;
	nou->Parent = NULL;
	nou->Value = Value;
	
	if (Tree->Root == NULL) {
		Tree->Root = nou;
		Tree->Size = 1;
		return 0;
	}
	PTREE_NODE curent = Tree->Root;
	PTREE_NODE ant =NULL;
	int left = 0;
	//find previous node
	while (curent != NULL) {
		if (Value < curent->Value) {
			left = 1;
			ant = curent;
			curent = curent->Left;
		}
		else {
			left = 0;
			ant = curent;
			curent = curent->Right;
		}
	}
	if (left == 1) {
		ant->Left = nou;
		nou->Parent = ant;
	}
	else {
		ant->Right = nou;
	}
	nou->Parent = ant;
	Tree->Size++;
	//recalculate height for branches that contains the new node
	RecalculateHeight(Tree->Root);
	//rebalance tree ( in case it's already balanced , nothing will happen);
	
	Rebalance(Tree);
	

	return 0;
}
static int RemoveNode(PTREE_NODE node,int left) {
	if (node->Left == NULL && node->Right == NULL) {
		if (left == 1) {
			node->Parent->Left = NULL;
		}
		else {
			node->Parent->Right = NULL;
		}
		free(node);
		node = NULL;
	}
	else if (node->Left == NULL && node->Right != NULL) {
		if (left == 1) {
			node->Parent->Left = node->Right;
		}
		else {
			node->Parent->Right = node->Right;
		}
		node->Right->Parent = node->Parent;

		free(node);
		node = NULL;
	}
	else if (node->Left != NULL && node->Right == NULL) {
		if (left == 1) {
			node->Parent->Left = node->Left;
		}
		else {
			node->Parent->Right = node->Left;
		}
		node->Left->Parent = node->Parent;

		free(node);
		node = NULL;
	}
	else {
		if (left == 1) {
			PTREE_NODE curent = node->Right;
			if (curent->Left == NULL) {
				node->Value = curent->Value;
				RemoveNode(curent, 0);

			}
			else {
				while (curent->Left != NULL) {
					curent = curent->Left;
				}
				node->Value = curent->Value;
				RemoveNode(curent, 1);
			}
		}
		else {
			PTREE_NODE curent = node->Left;
			if (curent->Right == NULL) {
				node->Value = curent->Value;
				RemoveNode(curent, 1);
			}
			else {
				while (curent->Right != NULL) {
					curent = curent->Right;
				}
				node->Value = curent->Value;
				RemoveNode(curent, 0);
			}
		}
	}
	return 0;
}
int TreeRemove(CC_TREE *Tree, int Value)
{
	if (Tree == NULL) {
		return -1;
	}
	if (Tree->Root == NULL) {
		return 0;
	}
	if (Tree->Root->Value == Value && Tree->Root->Left == NULL && Tree->Root->Right == NULL) {
		free(Tree->Root);
		Tree->Root = NULL;
		Tree->Size--;

	}
	PTREE_NODE curent = Tree->Root;
	int left = 0;
	while (curent != NULL && Value != curent->Value) {
		if (Value > curent->Value) {
			curent = curent->Right;
			left = 0;
		}
		else {
			curent = curent->Left;
			left = 1;
		}
	}
	if (curent == NULL) {
		return 0;
	}
	else if (curent->Value == Value) {
		RemoveNode(curent, left);
		Tree->Size--;
		TreeRemove(Tree, Value);
	}	
	return 0;
}

int TreeContains(CC_TREE *Tree, int Value)
{
	if (Tree == NULL) {
		return -1;
	}
	PTREE_NODE curent = Tree->Root;
	while (curent != NULL) {
		if (Value == curent->Value) {
			return 1;
		}
		if (Value > curent->Value) {
			curent = curent->Right;
		}
		else {
			curent = curent->Left;
		}
	}
	if (curent == NULL) {
		return 0;
	}
	else {
		if (curent->Value == Value) {
			return 1;
		}
		else {
			return 0;
		}
	}
}

int TreeGetCount(CC_TREE *Tree)
{
	if (Tree == NULL){
		return -1;
	}
	else {
		return Tree->Size;
	}
}

int TreeGetHeight(CC_TREE *Tree)
{
	if (Tree == NULL) {
		return -1;
	}
    return Tree->Root->Height;
}
//remove all nodes under the node argument and itself
static int Clear(PTREE_NODE nod) {
	if (nod == NULL) {
		return 0;
	}
	if (nod->Left != NULL) {
		Clear(nod->Left);
	}
	if (nod->Right != NULL) {
		Clear(nod->Right);
	}
	free(nod);
	return 0;
}
int TreeClear(CC_TREE *Tree)
{
	if (Tree == NULL) {
		return -1;
	}
	Clear(Tree->Root);
    return 0;
}

void inorder(PTREE_NODE root,int index,int *Value,int *i) {
	if (root->Left != NULL) {
		inorder(root->Left, index, Value,i);
	}
	if (*i == index) {
		*Value = root->Value;
	}
	*i = *i + 1 ;
	if (root->Right != NULL) {
		inorder(root->Right,index,Value,i);
	}
}

void preorder(PTREE_NODE root, int index, int *Value, int *i) {
	if (*i == index) {
		*Value = root->Value;
	}
	*i = *i + 1;
	if (root->Left != NULL) {
		preorder(root->Left, index, Value, i);
	}
	if (root->Right != NULL) {
		preorder(root->Right, index, Value, i);
	}
}
void postorder(PTREE_NODE root, int index, int *Value, int *i) {
	if (root->Left != NULL) {
		preorder(root->Left, index, Value, i);
	}
	if (root->Right != NULL) {
		preorder(root->Right, index, Value, i);
	}
	if (*i == index) {
		*Value = root->Value;
	}
	*i = *i + 1;
}
int TreeGetNthPreorder(CC_TREE *Tree, int Index, int *Value)
{
	if (Tree == NULL || Value == NULL) {
		return -1;
	}
	int i = 0;
	preorder(Tree->Root, Index, Value, &i);
	return 0;
}

int TreeGetNthInorder(CC_TREE *Tree, int Index, int *Value)
{
	if (Tree == NULL || Value == NULL) {
		return -1;
	}
	int i = 0;
	inorder(Tree->Root, Index, Value,&i);
    return 0;
}

int TreeGetNthPostorder(CC_TREE *Tree, int Index, int *Value)
{
	if (Tree == NULL || Value == NULL) {
		return -1;
	}
	int i = 0;
	inorder( Tree->Root, Index, Value, &i);
	return 0;
}

