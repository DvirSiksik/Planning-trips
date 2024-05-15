#pragma once



// a value not supposed in stack and queue
#define		EMPTY	0x7FFFFFFF

// new types
typedef void* DATA;					// a type for data (easy to change)
typedef enum { False, True } BOOL;	// a boolean type


typedef struct node
{
	DATA			key;
	struct node* next;
}NODE;

