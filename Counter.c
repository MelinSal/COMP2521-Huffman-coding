// Implementation of the Counter ADT

// Author:
// Melina Salardini (z5393518)
//
// written on 7/7/2024

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Counter.h"


struct counter {
  struct counter *right;
  struct counter *left;
  char character[MAX_CHARACTER_LEN + 1];
  int count;
};

// function prototypes
Counter CounterNew(void);
void CounterFree(Counter c);
void CounterAdd(Counter c, char *character);
int CounterNumItems(Counter c);
int CounterGet(Counter c, char *character);
void CounterItemsHelper(Counter c, struct item *item, int *index);
struct item *CounterItems(Counter c, int *numItems);


// create a new counter object
Counter CounterNew(void) {
  Counter newCounter = malloc(sizeof(struct counter));
  if (newCounter == NULL) {
    fprintf(stderr, "error: out of memory\n");
    exit(EXIT_FAILURE);
  }

  newCounter->right = NULL;
  newCounter->left = NULL;
  newCounter->character[0] = '\0';
  newCounter->count = 0;

  return newCounter;
}


// free memory for counter and all its nodes
void CounterFree(Counter c) {
  if (c == NULL) {
    return;
  }
  CounterFree(c->right);
  CounterFree(c->left);
  free(c);
}


//add a counter or increment its count if it already exists
void CounterAdd(Counter c, char *character) {
  // if the count is empty
  if (c->count == 0) {
    strcpy(c->character, character);
    c->count = 1;
    return;
  }

  Counter current = c;
  Counter prev = NULL;

  // traverse the tree to find the right position to add the character
  while (current != NULL) {
    if (strcmp(character, current->character) > 0) {
      prev = current;
      current = current->right;
    } else if (strcmp(character, current->character) < 0) {
      prev = current;
      current = current->left;
    } else {
      current->count++;
      return;
    }
  }

  // add a new counter
  Counter newCounter = CounterNew();
  strcpy(newCounter->character, character);
  newCounter->count = 1;

  if (strcmp(character, prev->character) > 0) {
    prev->right = newCounter;
  } else {
    prev->left = newCounter;
  }
}


// return the number of unique items in the counter
int CounterNumItems(Counter c) {
  // if the counter is empty
  if (c == NULL) {
    return 0;
  }

  int leftCount = CounterNumItems(c->left);
  int rightCount = CounterNumItems(c->right);
  return 1 + leftCount + rightCount;
}


// return the number of a specific character in the counter
int CounterGet(Counter c, char *character) {
  // if the counter is empty
  if (c == NULL) {
    return 0;
  }

  Counter current = c;
  while (current != NULL) {
    if (strcmp(character, current->character) > 0) {
      current = current->right;
    } else if (strcmp(character, current->character) < 0) {
      current = current->left;
    } else {
      return current->count;
    }
  }

  return 0;
}


// returns an array of items from the counter
struct item *CounterItems(Counter c, int *numItems) {
  *numItems = CounterNumItems(c);
  // if no Item exists
  if (*numItems == 0) {
    return NULL;
  }

  struct item *item = malloc(*numItems * sizeof(struct item));
  if (item == NULL) {
    fprintf(stderr, "error: out of memeory\n");
    exit(EXIT_FAILURE);
  }

  int index = 0;
  CounterItemsHelper(c, item, &index);
  return item;
}


// helper function to traverse the counter and collect the items into n array
void CounterItemsHelper(Counter c, struct item *item, int *index) {
  if (c == NULL) {
    return;
  }

  // use in order traverse to add the nodes
  CounterItemsHelper(c->left, item, index);
  // proccess the current code
  if (c->count > 0) {
    strcpy(item[*index].character, c->character);
    item[*index].freq = c->count;
    (*index)++;
  }

  CounterItemsHelper(c->right, item, index);
}
