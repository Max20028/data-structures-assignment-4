/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name:
 * Email:
 */

#include <stdio.h>
#include <stdlib.h>

#include "pq.h"

#define DATA_FILE "airports.dat"
#define START_NODE 0

struct dat {
  int node;
  int prevnode;
};

int main(int argc, char const *argv[]) {
  //LOAD INITIAL DATA
  int numNodes = 0;
  int numEdges = 0;
  FILE* fdata = fopen(DATA_FILE, "r");
  fscanf(fdata, "%d", &numNodes);
  fscanf(fdata, "%d", &numEdges);
  
  //PREPARE DATA STRUCTURES
  int** edges = malloc(sizeof(int*) * numNodes);
  for(int i = 0; i < numNodes; i++) {
    edges[i] = malloc(sizeof(int) * numNodes);
  }

  //LOAD DATA
  for(int i = 0; i < numEdges; i++) {
    int startingNode = 0;
    int endingNode = 0;
    int weight = 0;

    fscanf(fdata, "%d", &startingNode);
    fscanf(fdata, "%d", &endingNode);
    fscanf(fdata, "%d", &weight);

    edges[startingNode][endingNode] = weight;
  }

  int* cost = malloc(sizeof(int) * numNodes);
  int* prev = malloc(sizeof(int) * numNodes);
  struct pq* pq = pq_create();

  //FINISH PREP
  fclose(fdata);


  //DIJKSTRA
  // for every node N:
  //   cost[N] = infinity
  //   prev[N] = undefined

  // Q = new priority queue
  // insert N_start into Q with priority 0

  // while Q is not empty:
  //   c = Q.first_priority()  // c is the total cost of the path to N
  //   {N, N_prev} = Q.remove_first()  // N_prev is the previous node on the path to N
  //   if e < cost[N]:
  //     cost[N] = c
  //     prev[N] = N_prev
  //     for each neighbor N_i of N:  // A neighbor is a node connected to N by an edge
  //       c_i = cost of edge from N to N_i
  //       insert {N_i, N} into Q with priority c + c_i  // N is the previous node on the path to N_i
  
  for(int i = 0; i < numNodes; i++) {
    cost[i] = 2147483647;
    prev[i] = -1;
  }

  struct dat* start = malloc(sizeof(struct dat));
  start->node = START_NODE;
  start->prevnode = -1;
  pq_insert(pq, start, 0);

  while(!pq_isempty) {
    int c = pq_first_priority(pq);
    struct dat* dat = pq_remove_first(pq);
    int node = dat->node;
    int prevnode = dat->node;
    free(dat);
    if(c < cost[node]) {
      cost[node] = c;
      prev[node] = prevnode;
      for(int i = 0; i < numNodes; i++) {
        if(edges[node][i] != 0) {
          int c_i = edges[node][i];
          struct dat* new = malloc(sizeof(struct dat));
          new->node = i;
          new->prevnode = node;
          pq_insert(pq, new, c+c_i);
        }
      }
    }
  }



  //FREE DATA
  for(int i = 0; i < numNodes; i++) {
    free(edges[i]);
  }
  free(edges);


  return 0;
}
