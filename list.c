#include "list.h"
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
static int num_List_create = 0;

static struct List_s heads_array [LIST_MAX_NUM_HEADS];
static int num_heads_used;
static struct List_s * l_free_head;
static struct List_s * l_free_tail;

static struct Node_s nodes_array [LIST_MAX_NUM_NODES];
static int num_nodes_used;
static struct Node_s * n_free_head;
static struct Node_s * n_free_tail;

static void setup_list(){//setting up the link list of unused heads
  l_free_head = heads_array;
  struct List_s * current = l_free_head;

  for (int i = 1; i < LIST_MAX_NUM_HEADS; i++){
    current->next_free = (heads_array+i);
    current = current->next_free;
  }

  current->next_free = NULL;
  l_free_tail = current;
}

static void setup_node(){// setting up link list of unused nodes
  n_free_head = nodes_array;
  struct Node_s * current = n_free_head;

  for (int i = 1; i < LIST_MAX_NUM_NODES; i++){
    current->next_free = (nodes_array+i);
    current = current->next_free;
  }

  current->next_free = NULL;
  n_free_tail = current;
}

static struct Node_s * allocate_new_node(){//return a node to be used (does not set the item)
  if (n_free_head == n_free_tail){
    struct Node_s * new = n_free_head;
    n_free_head = NULL;
    n_free_tail = NULL;
    new->next_free = NULL;
    num_nodes_used++;
    return new;
  }
  struct Node_s * new = n_free_head;
  n_free_head = n_free_head->next_free;
  new->next_free = NULL;
  num_nodes_used++;
  return new;
}

static void free_head(List * list){//freeing up the head pointer for later use (does not manage the list)
  if (list == NULL)
    return;
  if (l_free_head == NULL){
    list->next_free = NULL;
    list->current = NULL;
    list->head = NULL;
    list-> tail = NULL;
    list->count = 0;
    l_free_head = list;
    l_free_tail = list;
    list = NULL;
    num_heads_used--;
    return;
  }
  l_free_tail ->next_free = list;
  l_free_tail = list;
  l_free_tail->next_free = NULL;
  l_free_tail->current = NULL;
  l_free_tail->head = NULL;
  l_free_tail-> tail = NULL;
  l_free_tail->count = 0;
  list = NULL;
  num_heads_used--;
}

static void free_node(Node * current){// freeing up the node for later use (does not manage the list)
  if (current == NULL)
    return;
  if (n_free_head == NULL){
    current->next_free = NULL;
    current->prev = NULL;
    current->next = NULL;
    current->item = NULL;
    n_free_head = current;
    n_free_tail = current;
    current = NULL;
    num_nodes_used--;
    return;
  }
  n_free_tail ->next_free = current;
  n_free_tail = current;
  n_free_tail->next_free  = NULL;
  current->prev = NULL;
  current->next = NULL;
  current->item = NULL;
  current = NULL;
  num_nodes_used--;
}

//start assignment functions
List* List_create(){

  if (num_heads_used == LIST_MAX_NUM_HEADS){// Max number of heads acheived
    return NULL;
  }

  if (num_List_create == 0){//O(n) set up for the nodes
    setup_list();
    setup_node();
    num_List_create++;
  }
  if (l_free_head == l_free_tail){// if there is only one head left
    struct List_s * new = l_free_head;
    l_free_head = NULL;
    l_free_tail = NULL;
    new->next_free = NULL;
    num_heads_used++;
    return new;
  }
  //getting an unused head.
  struct List_s * new = l_free_head;
  l_free_head = l_free_head->next_free;
  new->next_free = NULL;
  num_heads_used++;
  return new;
}
int List_count(List* pList){//returns the pList->count
  assert(pList != NULL);

  return pList->count;
}

void* List_first(List* pList){
  assert(pList != NULL);
  if (pList->head == NULL){//base case if list is empty
    pList->tail = NULL;
    pList->current = NULL;
    return NULL;
  }
  pList->current = pList->head;//set current as head
  return pList->head->item;
}

void* List_last(List* pList){
  assert(pList != NULL);
  if (pList->head == NULL){//base case if list is empty
    pList->tail = NULL;
    pList->current = NULL;
    return NULL;
  }
  pList->current = pList->tail;//set current as tail
  return pList->tail->item;
}

void* List_next(List* pList){
  assert(pList != NULL);

  if (pList->current == NULL){//base case if current is out of bounds
    if (pList->state_curret == LIST_OOB_END)//if current is end out of bounds do nothing
      return NULL;
    //else return the head
    pList->current = pList->head;
    return pList->head->item;
  }

  if (pList->current == pList->tail){//set the current as out of bounds end
    pList->state_curret = LIST_OOB_END;
    pList->current = NULL;
    return NULL;
  }
  pList->current = pList->current->next;
  return pList->current->item;
}
void* List_prev(List* pList){
  assert(pList != NULL);
  if (pList->current == NULL){//base case if current is out of bounds
      if (pList->state_curret == LIST_OOB_START)//if the current is start out of bounds then do nothing
        return NULL;
      //else set the current as tail
      pList->current = pList->tail;
      return pList->tail->item;
  }
  if (pList->current == pList->head){//if the current goes out of bounds
    pList->state_curret = LIST_OOB_START;
    pList->current = NULL;
    return NULL;
  }
  pList->current = pList->current->prev;
  return pList->current->item;
}
void* List_curr(List* pList){
  assert(pList != NULL);
  if (pList->current == NULL)//base case if current is not available
    return NULL;
  return pList->current->item;
}



int List_add(List* pList, void* pItem){
  assert(pList != NULL);

  if (num_nodes_used == LIST_MAX_NUM_NODES)//base case if there are no nodes left
    return -1;
  if (pList->head == NULL){// if the list is empty
    struct Node_s * new = allocate_new_node();
    new->item = pItem;
    new->next = NULL;
    new->prev = NULL;
    pList->head = new;
    pList->tail = new;
    pList->current = new;
    pList->count++;
    return 0;
  }
  if (pList->current == NULL){// if current is out of bounds
    struct Node_s * new = allocate_new_node();
    new->item = pItem;

    if (pList->state_curret == LIST_OOB_END){//if end out of bounds add to the end of list

      new->next = NULL;
      new->prev = pList->tail;
      pList->tail->next = new;
      pList->tail = new;
    }
    else{// if start out of bounds add to the begining of the list
      new->next = pList->head;
      new->prev = NULL;
      pList->head->prev = new;
      pList->head = new;
    }

    pList->current = new;//set current as the new list
    pList->count++;
    return 0;
  }
  if (pList->current == pList->tail){//if current is tail, you need to make the set the new current as tail
    struct Node_s * new = allocate_new_node();
    new->item = pItem;
    new->prev = pList->tail;
    new->next = NULL;
    pList->tail->next = new;
    pList->current = new;
    pList->tail = new;
    return 0;
  }
  struct Node_s * new = allocate_new_node();
  new->item = pItem;

  new->prev = pList->current;
  new->next = pList->current->next;
  pList->current->next->prev = new;
  pList->current->next = new;
  pList->current = new;
  return 0;
}
int List_insert(List* pList, void* pItem){
  assert(pList != NULL);

  if (num_nodes_used == LIST_MAX_NUM_NODES)//base case all nodes used
    return -1;
  if (pList->head == NULL){//if the list is empty
    struct Node_s * new = allocate_new_node();
    new->item = pItem;
    new->next = NULL;
    new->prev = NULL;
    pList->head = new;
    pList->tail = new;
    pList->current = new;
    pList->count++;
    return 0;
  }
  if (pList->current == NULL){//if the current is out of bounds
    struct Node_s * new = allocate_new_node();
    new->item = pItem;

    if (pList->state_curret == LIST_OOB_END){// if the current is end out of bounds

      new->next = NULL;
      new->prev = pList->tail;
      pList->tail->next = new;
      pList->tail = new;

    }
    else{// if the current is start out of bounds
      new->next = pList->head;
      new->prev = NULL;
      pList->head->prev = new;
      pList->head = new;
    }

    pList->current = new;
    pList->count++;
    return 0;
  }

  if (pList->current == pList->head){//if current is head, you need to make the set the new current as head
    struct Node_s * new = allocate_new_node();
    new->item = pItem;
    new->prev = pList->current->prev;
    new->next = pList->current;
    pList->current->prev = new;
    pList->current = new;
    pList->count++;
    pList->head = pList->current;
    return 0;
  }

  struct Node_s * new = allocate_new_node();
  new->item = pItem;
  new->prev = pList->current->prev;
  new->next = pList->current;
  pList->current->prev->next = new;
  pList->current->prev = new;

  pList->current = new;
  pList->count++;
  return 0;
}
int List_append(List* pList, void* pItem){
  assert(pList != NULL);

  if (num_nodes_used == LIST_MAX_NUM_NODES)//base case if max number of nodes are used
    return -1;
  if (pList->head == NULL){//if the list are empty
    struct Node_s * new = allocate_new_node();
    new->item = pItem;
    new->next = NULL;
    new->prev = NULL;
    pList->head = new;
    pList->tail = new;
    pList->current = new;
    pList->count++;
    return 0;
  }

  struct Node_s * new = allocate_new_node();
  new->item = pItem;
  new->next = NULL;
  new->prev = pList->tail;
  pList->tail->next = new;
  pList->tail = new;
  pList->current = new;
  pList->count++;
  return 0;
}
int List_prepend(List* pList, void* pItem){
  assert(pList != NULL);

  if (num_nodes_used == LIST_MAX_NUM_NODES)//base case if max number of nodes are used
    return -1;
  if (pList->head == NULL){//if list is empty
    struct Node_s * new = allocate_new_node();
    new->item = pItem;
    new->next = NULL;
    new->prev = NULL;
    pList->head = new;
    pList->tail = new;
    pList->current = new;
    pList->count++;
    return 0;
  }
  struct Node_s * new = allocate_new_node();
  new->item = pItem;
  new->next = pList->head;
  new->prev = NULL;
  pList->head->prev = new;
  pList->head = new;
  pList->current = new;
  pList->count++;
  return 0;
}

void* List_remove(List* pList){
  assert(pList != NULL);

  if (pList->head == NULL || pList->current == NULL)//base case if list is empty or current is out of bounds
    return NULL;
  pList->count--;
  if (pList->head == pList->tail){//if there is only one element in the list we need to set head and tail to nULL
    pList->head = NULL;
    pList->tail = NULL;
    void * temp = pList -> current->item;
    free_node(pList->current);
    pList->current = NULL;
    return temp;
  }
  if (pList->current == pList->head){//if current is the head we need to set the head->prev as NULL
    pList->head = pList->head->next;
    pList->head->prev = NULL;

    void * temp = pList -> current->item;
    free_node(pList->current);
    pList->current = pList->head;
    return temp;
  }
  if (pList->current == pList->tail){//if current is the tail we need to set the tail->next as NULL
    pList->tail = pList->tail->prev;
    pList->tail->next = NULL;

    void * temp = pList -> current->item;
    free_node(pList->current);
    pList->current = pList->tail;
    return temp;
  }

  pList->current->prev->next = pList->current->next;
  pList->current->next->prev = pList->current->prev;
  void * temp = pList -> current->item;
  free_node(pList->current);
  pList->current = pList->tail;
  return temp;
}

void List_concat(List* pList1, List* pList2){
  assert(pList1 != NULL);
  assert(pList2 != NULL);

  if (pList1->head == NULL){//base case if plist1 is empty, just make plist2 as the node
    pList1->head = pList2->head;
    pList1->tail = pList2->tail;
    pList1->count += pList2->count;
    free_head(pList2);
    return;
  }
  if (pList2->head != NULL){//if the plist2 is not empty then set the correct things
    pList1->tail->next = pList2->head;
    pList2->head->prev = pList1->tail;
    pList1->tail = pList2->tail;
  }
  pList1->count += pList2->count;
  free_head(pList2);
  return;
}

void List_free(List* pList, FREE_FN pItemFreeFn){
  assert(pList != NULL);
  if (pList->head == NULL){//base case if list is empty then just free the head for later use
    free_head(pList);
    return;
  }
  struct Node_s * current = pList->head;
  while(current != NULL){
    current = current->next;
    (*pItemFreeFn)(pList->head->item);
    free_node(pList->head);
    pList->head = current;
  }
  free_head(pList);
}

void* List_trim(List* pList){
  assert(pList != NULL);
  if (pList->head == NULL)//base case if list is empty
    return NULL;
  if (pList->head == pList->tail){//if there is only one element left in the list than set both head and tail to NULL after freeing node
    pList->current = pList->tail;
    pList->count--;
    pList->head = NULL;
    pList->tail = NULL;
    void * temp = pList->current->item;
    free_node(pList->current);
    pList->current = NULL;
    return temp;
  }

  pList->current = pList->tail;
  pList->count--;
  pList->tail = pList->tail->prev;
  pList->tail->next = NULL;
  void * temp = pList->current->item;
  free_node(pList->current);
  pList->current = pList->tail;
  return temp;


}

void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){// when inputing pCompactor it's just node_struct
  assert(pList != NULL);
  if (pList->head == NULL)//base case if list is empty
    return NULL;
  if (pList->current == NULL){// if current is out of bounds
    if (pList-> state_curret == LIST_OOB_END)//if end of out of bounds do nothing
      return NULL;
    else //if start out of bounds then start searching from thea head
      pList->current = pList->head;

  }
  if ((*pComparator)(pList->current->item, pComparisonArg) == 1)//start comparing current item
    return pList->current->item;
  void* temp = List_next(pList);
  while (temp != NULL){
    if ((*pComparator)(temp, pComparisonArg) == 1)
      return temp;
    temp = List_next(pList);
  }
  return NULL;
}
