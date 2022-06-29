/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:
 * Email:
 */

#include <stdlib.h>
#include <stdio.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */

struct node {
  int priority;
  void* val;
};

struct pq {
  struct dynarray* arr;
};



/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
  struct pq* new = malloc(sizeof(struct pq));
  new->arr = dynarray_create();
  return new;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
  dynarray_free(pq->arr);
  free(pq);
  return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
  if(dynarray_size(pq->arr) <= 0) {
    return 1;
  }
  return 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
  struct node* newnode = malloc(sizeof(struct node));
  newnode->val = value;
  newnode->priority = priority;
  dynarray_insert(pq->arr, newnode);
  int idx = dynarray_size(pq->arr)-1;
  // printf("Inserting %d\n", priority);

  while(idx>0){
    int parentidx = (idx-1)/2;
    struct node* parentnode = dynarray_get(pq->arr, parentidx);
    //printf("Checking Parent Node %d\n", parentidx);
    if(parentnode->priority > priority){
      //swap
      dynarray_set(pq->arr, parentidx, dynarray_get(pq->arr, idx));
      dynarray_set(pq->arr, idx, parentnode);

      idx = parentidx;
    } else {
      return;
    }
  }
  return;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
  struct node* node = dynarray_get(pq->arr, 0);
  return node->val;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
  struct node* node = dynarray_get(pq->arr, 0);
  return node->priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
  if(dynarray_size(pq->arr) == 1) {
    //SPECIAL CASE: PQ has one node left
    struct node* node = dynarray_get(pq->arr, 0);
    void* value = node->val;
    free(dynarray_get(pq->arr, 0));
    dynarray_remove(pq->arr, 0);
    return value;
  }
  struct node* root = dynarray_get(pq->arr, 0);
  struct node* last = dynarray_get(pq->arr, dynarray_size(pq->arr)-1);
  void* value = root->val;
  dynarray_set(pq->arr, 0, last);
  dynarray_remove(pq->arr, dynarray_size(pq->arr)-1);
  free(root);
  int idx = 0;
  int size = dynarray_size(pq->arr);

  while(idx < size){
    int left = 2*idx+1;
    int right = 2*idx+2;
    struct node* node = dynarray_get(pq->arr, idx);
    int priority = node->priority;
    if(left >= size){
      //CASE: No Children
      return value;
    } else if(right >= size){
      //CASE: Left Child
      struct node* leftchild = dynarray_get(pq->arr, left);
      if(priority > leftchild->priority){
        //swap
        struct node* temp = dynarray_get(pq->arr, idx);
        dynarray_set(pq->arr, idx, dynarray_get(pq->arr, left));
        dynarray_set(pq->arr, left, temp);
        idx = left;
      } else {
        return value;
      }
    } else {
      //CASE: Two Children
      struct node* leftchild = dynarray_get(pq->arr, left);
      struct node* rightchild = dynarray_get(pq->arr, right);
      if(priority <= leftchild->priority && priority <= rightchild->priority) {
        //CASE: Have reached correct position
        return value;
      }

      if(leftchild->priority <= rightchild->priority){
        //Swap with left
        struct node* temp = dynarray_get(pq->arr, idx);
        dynarray_set(pq->arr, idx, dynarray_get(pq->arr, left));
        dynarray_set(pq->arr, left, temp);
        idx = left;
      } else {
        //swap with right
        struct node* temp = dynarray_get(pq->arr, idx);
        dynarray_set(pq->arr, idx, dynarray_get(pq->arr, right));
        dynarray_set(pq->arr, right, temp);
        idx = right;
      }
    }
  }
  return NULL;
}
