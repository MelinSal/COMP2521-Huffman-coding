// Implementation of the Huffman module

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
#include "File.h"
#include "character.h"
#include "huffman.h"

// function prototypes
void decode(struct huffmanTree *tree, char *encoding, char *outputFilename);
struct huffmanTree *createHuffmanTree(char *inputFilename);
struct huffmanTree *newHuffmanTreeNode(char *character, int freq);
struct huffmanTree *min(struct huffmanTree **queue, int size);
int deleteNode(struct huffmanTree **queue, int size, struct huffmanTree *node);
int insertNode(struct huffmanTree **queue, int size, struct huffmanTree *node);



// Task 1
//
// decodes a string using the provided huffmanTree and writes
// the decoded character to a output file that is given
void decode(struct huffmanTree *tree, char *encoding, char *outputFilename) {
  File outputFile = FileOpenToWrite(outputFilename);

  struct huffmanTree *current = tree;
  for (int i = 0; encoding[i] != '\0'; i++) {
    if (encoding[i] == '0') {
      current = current->left;
    } else if (encoding[i] == '1') {
      current = current->right;
    }

    // if its a leaf node write the character in the outputFile
    if (current->right == NULL && current->left == NULL) {
      FileWrite(outputFile, current->character);

      // update the current pointer to start from the root of the tree
      current = tree;
    }
  }

  FileClose(outputFile);
}


// Task 3
//
// create a huffman tree from the characters read from the input file
struct huffmanTree *createHuffmanTree(char *inputFilename) {
  File inputFile = FileOpenToRead(inputFilename);
  Counter counter = CounterNew();
  char buffer[MAX_CHARACTER_LEN + 1];

  // read the char and add them to the counter
  while (FileReadCharacter(inputFile, buffer)) {
    CounterAdd(counter, buffer);
  }
  FileClose(inputFile);

  // create a leaf node for each one and put them in a queue
  int numItems = CounterNumItems(counter);
  struct item *items = CounterItems(counter, &numItems);
  struct huffmanTree **queue = malloc(numItems * sizeof(struct huffmanTree *));
  if (queue == NULL) {
    fprintf(stderr, "error: out of memory\n");
    exit(EXIT_FAILURE);
  }

  // initilize the new leaf and put them into a array
  for (int i = 0; i < numItems; i++) {
    queue[i] = newHuffmanTreeNode(items[i].character, items[i].freq);
  }
  free(items);

  int queueSize = numItems;

  while (queueSize > 1) {
    // take the two trees with the smallest freq
    struct huffmanTree *left = min(queue, queueSize);
    queueSize = deleteNode(queue, queueSize, left);
    struct huffmanTree *right = min(queue, queueSize);
    queueSize = deleteNode(queue, queueSize, right);

    // create a new node with the two trees as children and sum of their freq
    struct huffmanTree *newNode =
        newHuffmanTreeNode(NULL, right->freq + left->freq);
    newNode->right = right;
    newNode->left = left;

    // add the newNode to the array
    queueSize = insertNode(queue, queueSize, newNode);
  }

  // the last node is the root of the huffmanTree
  struct huffmanTree *root = queue[0];
  free(queue);
  CounterFree(counter);
  return root;
}


// create a new huffman tree node given character and frequency
struct huffmanTree *newHuffmanTreeNode(char *character, int freq) {
  struct huffmanTree *node = malloc(sizeof(struct huffmanTree));
  if (node == NULL) {
    fprintf(stderr, "error: out of memory\n");
    exit(EXIT_FAILURE);
  }

  if (character == NULL) {
    node->character = NULL;
  } else {
    node->character = malloc(strlen(character) + 1);
    strcpy(node->character, character);
  }

  node->freq = freq;
  node->left = NULL;
  node->right = NULL;

  return node;
}


// find and return the node with the minimum frequency in the queue
struct huffmanTree *min(struct huffmanTree **queue, int size) {
  int i = 1;
  struct huffmanTree *min = queue[0];
  while (i < size && queue[i] != NULL) {
    if (queue[i]->freq < min->freq) {
      min = queue[i];
    }
    i++;
  }
  return min;
}


// delete the node from the queue by shifting the elements left
int deleteNode(struct huffmanTree **queue, int size, struct huffmanTree *node) {
  int i;
  for (i = 0; i < size; i++) {
    if (queue[i] == node) {
      break;
    }
  }
  for (; i < size - 1; i++) {
    queue[i] = queue[i + 1];
  }
  queue[--(size)] = NULL;
  return size;
}


// insert a node into the queue
int insertNode(struct huffmanTree **queue, int size, struct huffmanTree *node) {
  queue[(size)++] = node;
  return size;
}




// Task 4
char *encode(struct huffmanTree *tree, char *inputFilename) { return NULL; }
