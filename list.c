/**************/
/*   list.c   */
/**************/

#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <crtdbg.h>
#include "list.h"
#include "fileHelper.h"
#include  "General.h"




//////////////////////////////////////////
// Init
// Aim:		create new list
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
//////////////////////////////////////////
BOOL L_init(LIST* pList)
{
	if (pList == NULL)
		return False;	// no list to initialize

	pList->head.next = NULL;
	return True;
}


/////////////////////////////////////////////////////////////////
// Insert
// Aim:		add new node
// Input:	pointer to the node BEFORE the place for the new one
//			a value to be stored in the new node
// Output:	pointer to the new node
/////////////////////////////////////////////////////////////////
NODE* L_insert(NODE* pNode, DATA Value)
{
	NODE* tmp;

	if (!pNode)
		return NULL;

	tmp = (NODE*)malloc(sizeof(NODE));	// new node

	if (tmp != NULL) {
		tmp->key = Value;
		tmp->next = pNode->next;
		pNode->next = tmp;
	}
	return tmp;
}


//////////////////////////////////////////////////////////////
// Delete
// Aim:		erase node
// Input:	pointer to the node BEFORE the node to be deleted 
// Output:	TRUE if succeeded
//////////////////////////////////////////////////////////////
BOOL L_delete(NODE* pNode, void (*freeFunc)(void*))
{
	NODE* tmp;
	if (!pNode)
		return False;
	tmp = pNode->next;
	if (!tmp)
		return False;

	pNode->next = tmp->next;
	if (freeFunc != NULL)
		freeFunc(tmp->key);
	free(tmp);
	return True;
}


/////////////////////////////////////////////////////////
// Find
// Aim:		search for a value
// Input:	pointer to the node to start with 
//			a value to be found
// Output:	pointer to the node containing the Value
/////////////////////////////////////////////////////////
const NODE* L_find(const NODE* pNode, DATA Value)
{
	const NODE* tmp = pNode;

	while (tmp != NULL)
	{
		if (tmp->key == Value)
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}


////////////////////////////////////////////////
// Free (additional function)
// Aim:		free the list memory
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
////////////////////////////////////////////////
BOOL L_free(LIST* pList, void (*freeFunc)(void*))
{
	NODE* tmp;
	BOOL cont = True;
	if (!pList)
		return False;

	tmp = &(pList->head);
	while (cont)
		cont = L_delete(tmp, freeFunc);

	return True;
}


////////////////////////////////////////////////
// Print (additional function)
// Aim:		print the list content (assume the DATA is int)
// Input:	pointer to the list structure
// Output:	a number of the printed elements
////////////////////////////////////////////////
int L_print(const LIST* pList, void(*print)(const void*))
{
	NODE* tmp;
	int		c = 0;

	if (!pList)
		return False;

	printf("\n");
	tmp = pList->head.next;
	while (tmp != NULL)
	{
		//printf(" %d ---> ");
		print(tmp->key);
		c++;
		tmp = tmp->next;
	}
	printf("\n");
	return c;
}

int L_WriteBFile(const LIST* pList, FILE* f)
{
	NODE* tmp;
	tmp = pList->head.next;
	while (tmp != NULL)
	{
		if (!writeStringToFile(tmp->key, f, "ERROR LIST"))
			return 0;
		tmp = tmp->next;
	}
	return 1;
}


int L_readBFile(LIST* pList,int count, FILE* f)
{
	L_init(pList);
	char* temp;

	for (int i = 0; i < count; i++)
	{
		temp=readStringFromFile(f,"ERROR LIST");
		if (!temp)
			return 0;
		L_insert(&pList->head, temp);
	}

	return 1;
}

int L_WriteTFile(const LIST* pList, FILE* f)
{
	NODE* tmp;
	tmp = pList->head.next;
	while (tmp != NULL)
	{
		fprintf(f, "%s\n", (char*)tmp->key);
		tmp = tmp->next;
	}
	return 1;
}

int L_readTFile(LIST* pList, int count, FILE* f)
{
	L_init(pList);
	char temp[MAX_STR_LEN];
	for (int i = 0; i < count; i++)
	{
		myGets(temp, MAX_STR_LEN, f);
		L_insert(&pList->head, getDynStr(temp));
	}
	return 1;
}

