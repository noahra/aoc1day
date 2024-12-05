#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_MAP_SIZE 12000

typedef struct Node {
  int64_t key;
  int value;
  struct Node *next;
} Node;
Node *hashMap[HASH_MAP_SIZE];

unsigned int hash(int key) { return key % HASH_MAP_SIZE; }

void insertOrUpdate(int key, int value) {
  unsigned int index = hash(key);
  Node *current = hashMap[index];

  while (current != NULL) {
    if (current->key == key) {
      current->value = value; // Update value if the key is found
      return;
    }
    current = current->next; // Move to the next node if key is not found
  }

  // If key is not found, create a new node
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->key = key;
  newNode->value = value;
  newNode->next = hashMap[index];
  hashMap[index] = newNode;
}

int search(int key) {
  unsigned int index = hash(key);
  Node *current = hashMap[index];

  while (current != NULL) {
    if (current->key == key) {
      return current->value;
    }

    current = current->next;
  }
  return -1;
}

void freeHashMap() {
  for (int i = 0; i < HASH_MAP_SIZE; i++) {
    Node *current = hashMap[i];
    while (current != NULL) {
      Node *temp = current;
      current = current->next;
      free(temp);
    }
    hashMap[i] = NULL;
  }
}
int read_pairs(FILE *ptr, int64_t **input_numbers, int64_t **input_numbers2,
               int max_size) {
  int i = 0;
  int64_t num, num2;
  *input_numbers = malloc(max_size * sizeof(int64_t));
  *input_numbers2 = malloc(max_size * sizeof(int64_t));

  // Read pairs of numbers from the file and store them in the arrays
  while (i < max_size && fscanf(ptr, "%lld %lld", &num, &num2) == 2) {
    (*input_numbers)[i] = num;
    (*input_numbers2)[i] = num2;
    i++;
  }
  return i; // Return the number of pairs read
}
int compare(const void *a, const void *b) {
  return (*(int64_t *)a - *(int64_t *)b);
}

int64_t calculate_accumulator(int64_t *input_numbers, int64_t *input_numbers2,
                              int n) {
  int64_t accumulator = 0;
  for (int i = 0; i < n; i++) {
    int64_t diff = llabs(input_numbers[i] - input_numbers2[i]);
    accumulator += diff;
  }
  return accumulator;
}
int main() {
  FILE *ptr;

  int64_t *input_numbers; // Pointers to the dynamically allocated arrays
  int64_t *input_numbers2;

  ptr = fopen("input.txt", "r");

  if (NULL == ptr) {
    printf("file can't be opened \n");
  }

  int n = read_pairs(ptr, &input_numbers, &input_numbers2, 1000003);

  qsort(input_numbers, n, sizeof(int64_t), compare);
  qsort(input_numbers2, n, sizeof(int64_t), compare);

  // Iterate through both lists
  int64_t accumulator = calculate_accumulator(input_numbers, input_numbers2, n);
  printf("Accumulator: %lld\n", accumulator);

  for (int i = 0; i < n; i++) {

    int current_value = search(input_numbers2[i]);
    if (current_value != -1) {
      insertOrUpdate(input_numbers2[i], current_value + 1);
    } else {
      insertOrUpdate(input_numbers2[i], 1);
    }
  }

  int similarity_score = 0;

  for (int i = 0; i < n; i++) {
    int current_value = search(input_numbers[i]);
    if (current_value != -1) {
      similarity_score = similarity_score + (current_value * input_numbers[i]);
    }
  }

  printf("Similarity score: %d\n", similarity_score);

  free(input_numbers);
  free(input_numbers2);

  freeHashMap();

  fclose(ptr);
  return 0;
}
