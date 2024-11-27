/*******************************************************************************
 * Tipos implementados:
 *  - Pilha estatica
 *  - Pilha dinamica
 *  - fila/lista estatica
 *  - fila/lista dinamica
 *  - pilha circular estatica
 *  - Pilha circular dinamica
 *  - fila/lista circular estatica
 *  - fila/lista circular dinamica
 *  - fila/lista encadeada simples estatica
 *  - fila/lista encadeada simples dinamica
 *  - fila/lista duplamente encadeada estatica
 *  - fila/lista duplamente encadeada dinamica
 *  - fila/lista circular encadeada simples estatica
 *  - fila/lista circular encadeada simples dinamica
 *  - fila/lista circular duplamente encadeada estatica
 *  - fila/lista circular duplamente encadeada dinamica
 *  - arvore
 *  - tabela hash
 *  - mapa hash
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct static_queue {
    int *data;
    size_t front;
    size_t rear;
    size_t capacity;
} static_queue_t;

typedef struct dynamic_queue {
    int *data;
    size_t front;
    size_t rear;
    size_t capacity;
} dynamic_queue_t;

typedef struct circular_static_queue {
    int *data;
    size_t front;
    size_t rear;
    size_t capacity;
} circular_static_queue_t;

typedef struct circular_dynamic_queue {
    int *data;
    size_t front;
    size_t rear;
    size_t capacity;
} circular_dynamic_queue_t;

typedef struct static_stack {
    int *data;
    size_t top;
    size_t capacity;
} static_stack_t;

typedef struct dynamic_stack {
    int *data;
    size_t top;
    size_t capacity;
} dynamic_stack_t;

typedef struct {
    int *data;
    size_t top;
    size_t capacity;
} circular_static_stack_t;

typedef struct circular_dynamic_stack {
    int *data;
    size_t top;
    size_t capacity;
} circular_dynamic_stack_t;

typedef struct one_way_linked_list_node {
    int data;
    struct one_way_linked_list_node *next;
} one_way_linked_list_node_t;

typedef struct one_way_linked_list {
    one_way_linked_list_node_t *head;
} one_way_linked_list_t;

typedef struct two_way_linked_list_node {
    int data;
    struct two_way_linked_list_node *next;
    struct two_way_linked_list_node *prev;
} two_way_linked_list_node_t;

typedef struct two_way_linked_list {
    two_way_linked_list_node_t *head;
} two_way_linked_list_t;

typedef struct circular_one_way_linked_list_node {
    int data;
    struct circular_one_way_linked_list_node *next;
} circular_one_way_linked_list_node_t;

typedef struct circular_one_way_linked_list {
    circular_one_way_linked_list_node_t *head;
} circular_one_way_linked_list_t;

typedef struct circular_two_way_linked_list_node {
    int data;
    struct circular_two_way_linked_list_node *next;
    struct circular_two_way_linked_list_node *prev;
} circular_two_way_linked_list_node_t;

typedef struct circular_two_way_linked_list {
    circular_two_way_linked_list_node_t *head;
} circular_two_way_linked_list_t;

typedef struct tree_node {
    int data;
    struct tree_node *left;
    struct tree_node *right;
} tree_node_t;

typedef struct tree {
    tree_node_t *root;
} tree_t;


typedef struct hash_map {
    int *keys;
    int *values;
    size_t capacity;
    size_t size;
} hash_map_t;

typedef struct hash_table {
    int *table;
    size_t size;
} hash_table_t;

typedef struct static_one_way_linked_list_node {
    int data;
} static_one_way_linked_list_node_t;

typedef struct static_one_way_linked_list {
    static_one_way_linked_list_node_t *nodes;
    size_t capacity;
    size_t size;
} static_one_way_linked_list_t;

typedef struct static_two_way_linked_list_node {
    int data;
    struct static_two_way_linked_list_node *prev;
} static_two_way_linked_list_node_t;

typedef struct static_two_way_linked_list {
    static_two_way_linked_list_node_t *nodes;
    size_t capacity;
    size_t size;
} static_two_way_linked_list_t;

typedef struct static_circular_one_way_linked_list_node {
    int data;
    struct static_circular_one_way_linked_list_node *next;
} static_circular_one_way_linked_list_node_t;

typedef struct static_circular_one_way_linked_list {
    static_circular_one_way_linked_list_node_t *head;
} static_circular_one_way_linked_list_t;

typedef struct static_circular_two_way_linked_list_node {
    int data;
    struct static_circular_two_way_linked_list_node *next;
    struct static_circular_two_way_linked_list_node *prev;
} static_circular_two_way_linked_list_node_t;

typedef struct static_circular_two_way_linked_list {
    static_circular_two_way_linked_list_node_t *head;
} static_circular_two_way_linked_list_t;

typedef struct dynamic_one_way_linked_list {
    one_way_linked_list_node_t *head;
} dynamic_one_way_linked_list_t;

typedef struct dynamic_two_way_linked_list {
    two_way_linked_list_node_t *head;
} dynamic_two_way_linked_list_t;

typedef struct dynamic_circular_one_way_linked_list {
    circular_one_way_linked_list_node_t *head;
} dynamic_circular_one_way_linked_list_t;

typedef struct dynamic_circular_two_way_linked_list {
    circular_two_way_linked_list_node_t *head;
} dynamic_circular_two_way_linked_list_t;

static_queue_t* create_static_queue(size_t capacity);
void destroy_static_queue(static_queue_t *queue);
int enqueue_static_queue(static_queue_t *queue, int value);
int dequeue_static_queue(static_queue_t *queue);
void display_static_queue(static_queue_t *queue);

dynamic_queue_t* create_dynamic_queue(size_t capacity);
void destroy_dynamic_queue(dynamic_queue_t *queue);
int enqueue_dynamic_queue(dynamic_queue_t *queue, int value);
int dequeue_dynamic_queue(dynamic_queue_t *queue);
void display_dynamic_queue(dynamic_queue_t *queue);

circular_static_queue_t* create_circular_static_queue(size_t capacity);
void destroy_circular_static_queue(circular_static_queue_t *queue);
int enqueue_circular_static_queue(circular_static_queue_t *queue, int value);
int dequeue_circular_static_queue(circular_static_queue_t *queue);
void display_circular_static_queue(circular_static_queue_t *queue);

circular_dynamic_queue_t* create_circular_dynamic_queue(size_t capacity);
void destroy_circular_dynamic_queue(circular_dynamic_queue_t *queue);
int enqueue_circular_dynamic_queue(circular_dynamic_queue_t *queue, int value);
int dequeue_circular_dynamic_queue(circular_dynamic_queue_t *queue);
void display_circular_dynamic_queue(circular_dynamic_queue_t *queue);

static_stack_t* create_static_stack(size_t capacity);
void destroy_static_stack(static_stack_t *stack);
int push_static_stack(static_stack_t *stack, int value);
int pop_static_stack(static_stack_t *stack);
void display_static_stack(static_stack_t *stack);

dynamic_stack_t* create_dynamic_stack(size_t capacity);
void destroy_dynamic_stack(dynamic_stack_t *stack);
int push_dynamic_stack(dynamic_stack_t *stack, int value);
int pop_dynamic_stack(dynamic_stack_t *stack);
void display_dynamic_stack(dynamic_stack_t *stack);

circular_static_stack_t* create_circular_static_stack(size_t capacity);
void destroy_circular_static_stack(circular_static_stack_t *stack);
int push_circular_static_stack(circular_static_stack_t *stack, int value);
int pop_circular_static_stack(circular_static_stack_t *stack);
void display_circular_static_stack(circular_static_stack_t *stack);

circular_dynamic_stack_t* create_circular_dynamic_stack(size_t capacity);
void destroy_circular_dynamic_stack(circular_dynamic_stack_t *stack);
int push_circular_dynamic_stack(circular_dynamic_stack_t *stack, int value);
int pop_circular_dynamic_stack(circular_dynamic_stack_t *stack);
void display_circular_dynamic_stack(circular_dynamic_stack_t *stack);

one_way_linked_list_t* create_one_way_linked_list(void);
void destroy_one_way_linked_list(one_way_linked_list_t *list);
void insert_one_way_linked_list(one_way_linked_list_t *list, int value);
void display_one_way_linked_list(one_way_linked_list_t *list);

two_way_linked_list_t* create_two_way_linked_list(void);
void destroy_two_way_linked_list(two_way_linked_list_t *list);
void insert_two_way_linked_list(two_way_linked_list_t *list, int value);
void display_two_way_linked_list(two_way_linked_list_t *list);

circular_one_way_linked_list_t* create_circular_one_way_linked_list(void);
void destroy_circular_one_way_linked_list(circular_one_way_linked_list_t *list);
void insert_circular_one_way_linked_list(circular_one_way_linked_list_t *list, int value);
void display_circular_one_way_linked_list(circular_one_way_linked_list_t *list);

circular_two_way_linked_list_t* create_circular_two_way_linked_list(void);
void destroy_circular_two_way_linked_list(circular_two_way_linked_list_t *list);
void insert_circular_two_way_linked_list(circular_two_way_linked_list_t *list, int value);
void display_circular_two_way_linked_list(circular_two_way_linked_list_t *list);

tree_t* create_tree(void);
void destroy_tree(tree_t *tree);
tree_node_t* insert_tree(tree_t *tree, int value);
void display_tree(tree_t *tree, tree_node_t *node, int level);

hash_map_t* create_hash_map(size_t capacity);
void destroy_hash_map(hash_map_t *map);
int hash_map_insert(hash_map_t *map, int key, int value);
int hash_map_get(hash_map_t *map, int key);
void display_hash_map(hash_map_t *map);

hash_table_t* create_hash_table(size_t size);
void destroy_hash_table(hash_table_t *hash_table);
void insert_hash_table(hash_table_t *hash_table, int key);
void display_hash_table(hash_table_t *hash_table);


static_one_way_linked_list_t* create_static_one_way_linked_list(size_t capacity);
void destroy_static_one_way_linked_list(static_one_way_linked_list_t *list);
void insert_static_one_way_linked_list(static_one_way_linked_list_t *list, int value);
void display_static_one_way_linked_list(static_one_way_linked_list_t *list);

static_two_way_linked_list_t* create_static_two_way_linked_list(size_t capacity);
void destroy_static_two_way_linked_list(static_two_way_linked_list_t *list);
void insert_static_two_way_linked_list(static_two_way_linked_list_t *list, int value);
void display_static_two_way_linked_list(static_two_way_linked_list_t *list);

static_circular_one_way_linked_list_t* create_static_circular_one_way_linked_list(void);
void destroy_static_circular_one_way_linked_list(static_circular_one_way_linked_list_t *list);
void insert_static_circular_one_way_linked_list(static_circular_one_way_linked_list_t *list, int value);
void display_static_circular_one_way_linked_list(static_circular_one_way_linked_list_t *list);

static_circular_two_way_linked_list_t* create_static_circular_two_way_linked_list(void);
void destroy_static_circular_two_way_linked_list(static_circular_two_way_linked_list_t *list);
void insert_static_circular_two_way_linked_list(static_circular_two_way_linked_list_t *list, int value);
void display_static_circular_two_way_linked_list(static_circular_two_way_linked_list_t *list);

dynamic_one_way_linked_list_t* create_dynamic_one_way_linked_list(void);
void destroy_dynamic_one_way_linked_list(dynamic_one_way_linked_list_t *list);
void insert_dynamic_one_way_linked_list(dynamic_one_way_linked_list_t *list, int value);
void display_dynamic_one_way_linked_list(dynamic_one_way_linked_list_t *list);

dynamic_two_way_linked_list_t* create_dynamic_two_way_linked_list(void);
void destroy_dynamic_two_way_linked_list(dynamic_two_way_linked_list_t *list);
void insert_dynamic_two_way_linked_list(dynamic_two_way_linked_list_t *list, int value);
void display_dynamic_two_way_linked_list(dynamic_two_way_linked_list_t *list);

dynamic_circular_one_way_linked_list_t* create_dynamic_circular_one_way_linked_list(void);
void destroy_dynamic_circular_one_way_linked_list(dynamic_circular_one_way_linked_list_t *list);
void insert_dynamic_circular_one_way_linked_list(dynamic_circular_one_way_linked_list_t *list, int value);
void display_dynamic_circular_one_way_linked_list(dynamic_circular_one_way_linked_list_t *list);

dynamic_circular_two_way_linked_list_t* create_dynamic_circular_two_way_linked_list(void);
void destroy_dynamic_circular_two_way_linked_list(dynamic_circular_two_way_linked_list_t *list);
void insert_dynamic_circular_two_way_linked_list(dynamic_circular_two_way_linked_list_t *list, int value);
void display_dynamic_circular_two_way_linked_list(dynamic_circular_two_way_linked_list_t *list);

static_queue_t* create_static_queue(size_t capacity) {
    static_queue_t *queue = malloc(sizeof(static_queue_t));
    queue->data = malloc(sizeof(int) * capacity);
    queue->front = 0;
    queue->rear = 0;
    queue->capacity = capacity;
    return queue;
}

void destroy_static_queue(static_queue_t *queue) {
    free(queue->data);
    free(queue);
}

int enqueue_static_queue(static_queue_t *queue, int value) {
    if ((queue->rear + 1) % queue->capacity == queue->front) {
        return -1; // Queue is full
    }
    queue->data[queue->rear] = value;
    queue->rear = (queue->rear + 1) % queue->capacity;
    return 0; // Success
}

int dequeue_static_queue(static_queue_t *queue) {
    if (queue->front == queue->rear) {
        return -1; // Queue is empty
    }
    int value = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    return value;
}

void display_static_queue(static_queue_t *queue) {
    if (queue->front == queue->rear) {
        printf("Queue is empty\n");
        return;
    }
    size_t i = queue->front;
    while (i != queue->rear) {
        printf("%d ", queue->data[i]);
        i = (i + 1) % queue->capacity;
    }
    printf("\n");
}

dynamic_queue_t* create_dynamic_queue(size_t capacity) {
    dynamic_queue_t *queue = malloc(sizeof(dynamic_queue_t));
    queue->data = calloc(capacity, sizeof(int)); // Use calloc to initialize to zero
    queue->front = 0;
    queue->rear = 0;
    queue->capacity = capacity;
    return queue;
}

void destroy_dynamic_queue(dynamic_queue_t *queue) {
    free(queue->data);
    free(queue);
}

int enqueue_dynamic_queue(dynamic_queue_t *queue, int value) {
    if ((queue->rear + 1) % queue->capacity == queue->front) {
        // Resize the queue
        size_t new_capacity = queue->capacity * 2;
        int *new_data = calloc(new_capacity, sizeof(int));
        for (size_t i = 0; i < queue->capacity; i++) {
            new_data[i] = queue->data[(queue->front + i) % queue->capacity];
        }
        free(queue->data);
        queue->data = new_data;
        queue->front = 0;
        queue->rear = queue->capacity;
        queue->capacity = new_capacity;
    }
    queue->data[queue->rear] = value;
    queue->rear = (queue->rear + 1) % queue->capacity;
    return 0; // Success
}

int dequeue_dynamic_queue(dynamic_queue_t *queue) {
    if (queue->front == queue->rear) {
        return -1; // Queue is empty
    }
    int value = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    return value;
}

void display_dynamic_queue(dynamic_queue_t *queue) {
    if (queue->front == queue->rear) {
        printf("Queue is empty\n");
        return;
    }
    size_t i = queue->front;
    while (i != queue->rear) {
        printf("%d ", queue->data[i]);
        i = (i + 1) % queue->capacity;
    }
    printf("\n");
}
circular_static_queue_t* create_circular_static_queue(size_t capacity) {
    circular_static_queue_t *queue = malloc(sizeof(circular_static_queue_t));
    queue->data = malloc(sizeof(int) * capacity);
    queue->front = 0;
    queue->rear = 0;
    queue->capacity = capacity;
    return queue;
}

void destroy_circular_static_queue(circular_static_queue_t *queue) {
    free(queue->data);
    free(queue);
}

int enqueue_circular_static_queue(circular_static_queue_t *queue, int value) {
    if ((queue->rear + 1) % queue->capacity == queue->front) {
        return -1; // Queue is full
    }
    queue->data[queue->rear] = value;
    queue->rear = (queue->rear + 1) % queue->capacity;
    return 0; // Success
}

int dequeue_circular_static_queue(circular_static_queue_t *queue) {
    if (queue->front == queue->rear) {
        return -1; // Queue is empty
    }
    int value = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    return value;
}

void display_circular_static_queue(circular_static_queue_t *queue) {
    if (queue->front == queue->rear) {
        printf("Queue is empty\n");
        return;
    }
    size_t i = queue->front;
    while (i != queue->rear) {
        printf("%d ", queue->data[i]);
        i = (i + 1) % queue->capacity;
    }
    printf("\n");
}


circular_dynamic_queue_t* create_circular_dynamic_queue(size_t capacity) {
    circular_dynamic_queue_t *queue = malloc(sizeof(circular_dynamic_queue_t));
    queue->data = malloc(sizeof(int) * capacity);
    queue->front = 0;
    queue->rear = 0;
    queue->capacity = capacity;
    return queue;
}

void destroy_circular_dynamic_queue(circular_dynamic_queue_t *queue) {
    free(queue->data);
    free(queue);
}

int enqueue_circular_dynamic_queue(circular_dynamic_queue_t *queue, int value) {
    if ((queue->rear + 1) % queue->capacity == queue->front) {
        // Resize the queue
        size_t new_capacity = queue->capacity * 2;
        int *new_data = malloc(sizeof(int) * new_capacity);
        for (size_t i = 0; i < queue->capacity; i++) {
            new_data[i] = queue->data[(queue->front + i) % queue->capacity];
        }
        free(queue->data);
        queue->data = new_data;
        queue->front = 0;
        queue->rear = queue->capacity;
        queue->capacity = new_capacity;
    }
    queue->data[queue->rear] = value;
    queue->rear = (queue->rear + 1) % queue->capacity;
    return 0; // Success
}

int dequeue_circular_dynamic_queue(circular_dynamic_queue_t *queue) {
    if (queue->front == queue->rear) {
        return -1; // Queue is empty
    }
    int value = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    return value;
}

void display_circular_dynamic_queue(circular_dynamic_queue_t *queue) {
    if (queue->front == queue->rear) {
        printf("Queue is empty\n");
        return;
    }
    size_t i = queue->front;
    while (i != queue->rear) {
        printf("%d ", queue->data[i]);
        i = (i + 1) % queue->capacity;
    }
    printf("\n");
}
static_stack_t* create_static_stack(size_t capacity) {
    static_stack_t *stack = malloc(sizeof(static_stack_t));
    stack->data = malloc(sizeof(int) * capacity);
    stack->top = 0;
    stack->capacity = capacity;
    return stack;
}

void destroy_static_stack(static_stack_t *stack) {
    free(stack->data);
    free(stack);
}

int push_static_stack(static_stack_t *stack, int value) {
    if (stack->top >= stack->capacity) {
        return -1; // Stack is full
    }
    stack->data[stack->top++] = value;
    return 0; // Success
}

int pop_static_stack(static_stack_t *stack) {
    if (stack->top == 0) {
        return -1; // Stack is empty
    }
    return stack->data[--stack->top];
}

void display_static_stack(static_stack_t *stack) {
    if (stack->top == 0) {
        printf("Stack is empty\n");
        return;
    }
    for (size_t i = 0; i < stack->top; i++) {
        printf("%d ", stack->data[i]);
    }
    printf("\n");
}
dynamic_stack_t* create_dynamic_stack(size_t capacity) {
    dynamic_stack_t *stack = malloc(sizeof(dynamic_stack_t));
    stack->data = malloc(sizeof(int) * capacity);
    stack->top = 0;
    stack->capacity = capacity;
    return stack;
}

void destroy_dynamic_stack(dynamic_stack_t *stack) {
    free(stack->data);
    free(stack);
}

int push_dynamic_stack(dynamic_stack_t *stack, int value) {
    if (stack->top >= stack->capacity) {
        // Resize the stack
        size_t new_capacity = stack->capacity * 2;
        int *new_data = malloc(sizeof(int) * new_capacity);
        for (size_t i = 0; i < stack->capacity; i++) {
            new_data[i] = stack->data[i];
        }
        free(stack->data);
        stack->data = new_data;
        stack->capacity = new_capacity;
    }
    stack->data[stack->top++] = value;
    return 0; // Success
}

int pop_dynamic_stack(dynamic_stack_t *stack) {
    if (stack->top == 0) {
        return -1; // Stack is empty
    }
    return stack->data[--stack->top];
}

void display_dynamic_stack(dynamic_stack_t *stack) {
    if (stack->top == 0) {
        printf("Stack is empty\n");
        return;
    }
    for (size_t i = 0; i < stack->top; i++) {
        printf("%d ", stack->data[i]);
    }
    printf("\n");
}
circular_static_stack_t* create_circular_static_stack(size_t capacity) {
    circular_static_stack_t *stack = malloc(sizeof(circular_static_stack_t));
    stack->data = malloc(sizeof(int) * capacity);
    stack->top = 0;
    stack->capacity = capacity;
    return stack;
}

void destroy_circular_static_stack(circular_static_stack_t *stack) {
    free(stack->data);
    free(stack);
}

int push_circular_static_stack(circular_static_stack_t *stack, int value) {
    if (stack->top >= stack->capacity) {
        return -1; // Stack is full
    }
    stack->data[stack->top++] = value;
    return 0; // Success
}

int pop_circular_static_stack(circular_static_stack_t *stack) {
    if (stack->top == 0) {
        return -1; // Stack is empty
    }
    return stack->data[--stack->top];
}

void display_circular_static_stack(circular_static_stack_t *stack) {
    if (stack->top == 0) {
        printf("Stack is empty\n");
        return;
    }
    for (size_t i = 0; i < stack->top; i++) {
        printf("%d ", stack->data[i]);
    }
    printf("\n");
}
circular_dynamic_stack_t* create_circular_dynamic_stack(size_t capacity) {
    circular_dynamic_stack_t *stack = malloc(sizeof(circular_dynamic_stack_t));
    stack->data = malloc(sizeof(int) * capacity);
    stack->top = 0;
    stack->capacity = capacity;
    return stack;
}

void destroy_circular_dynamic_stack(circular_dynamic_stack_t *stack) {
    free(stack->data);
    free(stack);
}

int push_circular_dynamic_stack(circular_dynamic_stack_t *stack, int value) {
    if (stack->top >= stack->capacity) {
        // Resize the stack
        size_t new_capacity = stack->capacity * 2;
        int *new_data = malloc(sizeof(int) * new_capacity);
        for (size_t i = 0; i < stack->capacity; i++) {
            new_data[i] = stack->data[i];
        }
        free(stack->data);
        stack->data = new_data;
        stack->capacity = new_capacity;
    }
    stack->data[stack->top++] = value;
    return 0; // Success
}

int pop_circular_dynamic_stack(circular_dynamic_stack_t *stack) {
    if (stack->top == 0) {
        return -1; // Stack is empty
    }
    return stack->data[--stack->top];
}

void display_circular_dynamic_stack(circular_dynamic_stack_t *stack) {
    if (stack->top == 0) {
        printf("Stack is empty\n");
        return;
    }
    for (size_t i = 0; i < stack->top; i++) {
        printf("%d ", stack->data[i]);
    }
    printf("\n");
}

one_way_linked_list_t* create_one_way_linked_list(void) {
    one_way_linked_list_t *list = malloc(sizeof(one_way_linked_list_t));
    list->head = NULL;
    return list;
}

void destroy_one_way_linked_list(one_way_linked_list_t *list) {
    one_way_linked_list_node_t *current = list->head;
    while (current) {
        one_way_linked_list_node_t *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void insert_one_way_linked_list(one_way_linked_list_t *list, int value) {
    one_way_linked_list_node_t *new_node = malloc(sizeof(one_way_linked_list_node_t));
    new_node->data = value;
    new_node->next = list->head;
    list->head = new_node;
}

void display_one_way_linked_list(one_way_linked_list_t *list) {
    one_way_linked_list_node_t *current = list->head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}


two_way_linked_list_t* create_two_way_linked_list(void) {
    two_way_linked_list_t *list = malloc(sizeof(two_way_linked_list_t));
    list->head = NULL;
    return list;
}

void destroy_two_way_linked_list(two_way_linked_list_t *list) {
    two_way_linked_list_node_t *current = list->head;
    while (current) {
        two_way_linked_list_node_t *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void insert_two_way_linked_list(two_way_linked_list_t *list, int value) {
    two_way_linked_list_node_t *new_node = malloc(sizeof(two_way_linked_list_node_t));
    new_node->data = value;
    new_node->next = list->head;
    new_node->prev = NULL;
    if (list->head) {
        list->head->prev = new_node;
    }
    list->head = new_node;
}

void display_two_way_linked_list(two_way_linked_list_t *list) {
    two_way_linked_list_node_t *current = list->head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

circular_one_way_linked_list_t* create_circular_one_way_linked_list(void) {
    circular_one_way_linked_list_t *list = malloc(sizeof(circular_one_way_linked_list_t));
    list->head = NULL;
    return list;
}

void destroy_circular_one_way_linked_list(circular_one_way_linked_list_t *list) {
    if (!list->head) {
        free(list);
        return;
    }
    circular_one_way_linked_list_node_t *current = list->head;
    circular_one_way_linked_list_node_t *next;
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != list->head);
    free(list);
}

void insert_circular_one_way_linked_list(circular_one_way_linked_list_t *list, int value) {
    circular_one_way_linked_list_node_t *new_node = malloc(sizeof(circular_one_way_linked_list_node_t));
    new_node->data = value;
    if (!list->head) {
        list->head = new_node;
        new_node->next = new_node; // Point to itself
    } else {
        new_node->next = list->head->next;
        list->head->next = new_node;
        list->head = new_node; // New node becomes the new head
    }
}

void display_circular_one_way_linked_list(circular_one_way_linked_list_t *list) {
    if (!list->head) {
        printf("List is empty\n");
        return;
    }
    circular_one_way_linked_list_node_t *current = list->head;
    do {
        printf("%d ", current->data);
        current = current->next;
    } while (current != list->head);
    printf("\n");
}


circular_two_way_linked_list_t* create_circular_two_way_linked_list(void) {
    circular_two_way_linked_list_t *list = malloc(sizeof(circular_two_way_linked_list_t));
    list->head = NULL;
    return list;
}

void destroy_circular_two_way_linked_list(circular_two_way_linked_list_t *list) {
    if (!list->head) {
        free(list);
        return;
    }
    circular_two_way_linked_list_node_t *current = list->head;
    circular_two_way_linked_list_node_t *next;
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != list->head);
    free(list);
}

void insert_circular_two_way_linked_list(circular_two_way_linked_list_t *list, int value) {
    circular_two_way_linked_list_node_t *new_node = malloc(sizeof(circular_two_way_linked_list_node_t));
    new_node->data = value;
    if (!list->head) {
        list->head = new_node;
        new_node->next = new_node;
        new_node->prev = new_node; // Point to itself
    } else {
        new_node->next = list->head;
        new_node->prev = list->head->prev;
        list->head->prev->next = new_node;
        list->head->prev = new_node;
    }
}

void display_circular_two_way_linked_list(circular_two_way_linked_list_t *list) {
    if (!list->head) {
        printf("List is empty\n");
        return;
    }
    circular_two_way_linked_list_node_t *current = list->head;
    do {
        printf("%d ", current->data);
        current = current->next;
    } while (current != list->head);
    printf("\n");
}

tree_t* create_tree(void) {
    tree_t *tree = malloc(sizeof(tree_t));
    tree->root = NULL;
    return tree;
}

void destroy_tree_node(tree_node_t *node) {
    if (node) {
        destroy_tree_node(node->left);
        destroy_tree_node(node->right);
        free(node);
    }
}

void destroy_tree(tree_t *tree) {
    destroy_tree_node(tree->root);
    free(tree);
}

tree_node_t* insert_tree(tree_t *tree, int value) {
    tree_node_t *new_node = malloc(sizeof(tree_node_t));
    new_node->data = value;
    new_node->left = new_node->right = NULL;

    if (tree->root == NULL) {
        tree->root = new_node;
        return new_node;
    }

    tree_node_t *current = tree->root;
    tree_node_t *parent = NULL;

    while (1) {
        parent = current;
        if (value < current->data) {
            current = current->left;
            if (current == NULL) {
                parent->left = new_node;
                return new_node;
            }
        } else {
            current = current->right;
            if (current == NULL) {
                parent->right = new_node;
                return new_node;
            }
        }
    }
}

void display_tree(tree_t *tree, tree_node_t *node, int level) {
    if (node) {
        display_tree(tree, node->right, level + 1);
        for (int i = 0; i < level; i++) {
            printf("    ");
        }
        printf("%d\n", node->data);
        display_tree(tree, node->left, level + 1);
    }
}

// Implementations of the new types
hash_map_t* create_hash_map(size_t capacity) {
    hash_map_t *map = malloc(sizeof(hash_map_t));
    map->keys = malloc(sizeof(int) * capacity);
    map->values = malloc(sizeof(int) * capacity);
    map->capacity = capacity;
    map->size = 0;
    return map;
}

void destroy_hash_map(hash_map_t *map) {
    free(map->keys);
    free(map->values);
    free(map);
}

int hash_map_insert(hash_map_t *map, int key, int value) {
    if (map->size >= map->capacity) {
        return -1; // Hash map is full
    }
    map->keys[map->size] = key;
    map->values[map->size] = value;
    map->size++;
    return 0; // Success
}

int hash_map_get(hash_map_t *map, int key) {
    for (size_t i = 0; i < map->size; i++) {
        if (map->keys[i] == key) {
            return map->values[i];
        }
    }
    return -1; // Key not found
}

void display_hash_map(hash_map_t *map) {
    for (size_t i = 0; i < map->size; i++) {
        printf("Key: %d, Value: %d\n", map->keys[i], map->values[i]);
    }
}


hash_table_t* create_hash_table(size_t size) {
    hash_table_t *hash_table = malloc(sizeof(hash_table_t));
    hash_table->table = calloc(size, sizeof(int));
    hash_table->size = size;
    return hash_table;
}

void destroy_hash_table(hash_table_t *hash_table) {
    free(hash_table->table);
    free(hash_table);
}

void insert_hash_table(hash_table_t *hash_table, int key) {
    size_t index = key % hash_table->size;
    hash_table->table[index] = key; // Simple overwrite for demo purposes
}

void display_hash_table(hash_table_t *hash_table) {
    for (size_t i = 0; i < hash_table->size; i++) {
        printf("Index %zu: %d\n", i, hash_table->table[i]);
    }
}


// Implementations for static one-way linked list
static_one_way_linked_list_t* create_static_one_way_linked_list(size_t capacity) {
    static_one_way_linked_list_t *list = malloc(sizeof(static_one_way_linked_list_t));
    list->nodes = malloc(sizeof(static_one_way_linked_list_node_t) * capacity);
    list->capacity = capacity;
    list->size = 0;
    return list;
}

void destroy_static_one_way_linked_list(static_one_way_linked_list_t *list) {
    free(list->nodes);
    free(list);
}

void insert_static_one_way_linked_list(static_one_way_linked_list_t *list, int value) {
    if (list->size < list->capacity) {
        list->nodes[list->size++].data = value;
    }
}

void display_static_one_way_linked_list(static_one_way_linked_list_t *list) {
    for (size_t i = 0; i < list->size; i++) {
        printf("%d ", list->nodes[i].data);
    }
    printf("\n");
}

// Implementations for static two-way linked list
static_two_way_linked_list_t* create_static_two_way_linked_list(size_t capacity) {
    static_two_way_linked_list_t *list = malloc(sizeof(static_two_way_linked_list_t));
    list->nodes = malloc(sizeof(static_two_way_linked_list_node_t) * capacity);
    list->capacity = capacity;
    list->size = 0;
    return list;
}

void destroy_static_two_way_linked_list(static_two_way_linked_list_t *list) {
    free(list->nodes);
    free(list);
}

void insert_static_two_way_linked_list(static_two_way_linked_list_t *list, int value) {
    if (list->size < list->capacity) {
        list->nodes[list->size++].data = value;
    }
}

void display_static_two_way_linked_list(static_two_way_linked_list_t *list) {
    for (size_t i = 0; i < list->size; i++) {
        printf("%d ", list->nodes[i].data);
    }
    printf("\n");
}

// Implementations for static circular one-way linked list
static_circular_one_way_linked_list_t* create_static_circular_one_way_linked_list(void) {
    static_circular_one_way_linked_list_t *list = malloc(sizeof(static_circular_one_way_linked_list_t));
    list->head = NULL;
    return list;
}

void destroy_static_circular_one_way_linked_list(static_circular_one_way_linked_list_t *list) {
    if (!list->head) return;
    static_circular_one_way_linked_list_node_t *current = list->head;
    do {
        static_circular_one_way_linked_list_node_t *next = current->next;
        free(current);
        current = next;
    } while (current != list->head);
    free(list);
}

void insert_static_circular_one_way_linked_list(static_circular_one_way_linked_list_t *list, int value) {
    static_circular_one_way_linked_list_node_t *new_node = malloc(sizeof(static_circular_one_way_linked_list_node_t));
    new_node->data = value;
    if (!list->head) {
        list->head = new_node;
        new_node->next = new_node; // Point to itself
    } else {
        static_circular_one_way_linked_list_node_t *temp = list->head;
        while (temp->next != list->head) {
            temp = temp->next;
        }
        temp->next = new_node;
        new_node->next = list->head; // Make it circular
    }
}

void display_static_circular_one_way_linked_list(static_circular_one_way_linked_list_t *list) {
    if (!list->head) {
        printf("List is empty\n");
        return;
    }
    static_circular_one_way_linked_list_node_t *current = list->head;
    do {
        printf("%d ", current->data);
        current = current->next;
    } while (current != list->head);
    printf("\n");
}

// Implementations for static circular two-way linked list
static_circular_two_way_linked_list_t* create_static_circular_two_way_linked_list(void) {
    static_circular_two_way_linked_list_t *list = malloc(sizeof(static_circular_two_way_linked_list_t));
    list->head = NULL;
    return list;
}

void destroy_static_circular_two_way_linked_list(static_circular_two_way_linked_list_t *list) {
    if (!list->head) return;
    static_circular_two_way_linked_list_node_t *current = list->head;
    do {
        static_circular_two_way_linked_list_node_t *next = current->next;
        free(current);
        current = next;
    } while (current != list->head);
    free(list);
}

void insert_static_circular_two_way_linked_list(static_circular_two_way_linked_list_t *list, int value) {
    static_circular_two_way_linked_list_node_t *new_node = malloc(sizeof(static_circular_two_way_linked_list_node_t));
    new_node->data = value;
    if (!list->head) {
        list->head = new_node;
        new_node->next = new_node; // Point to itself
        new_node->prev = new_node; // Point to itself
    } else {
        static_circular_two_way_linked_list_node_t *tail = list->head->prev;
        tail->next = new_node;
        new_node->prev = tail;
        new_node->next = list->head;
        list->head->prev = new_node; // Make it circular
    }
}

void display_static_circular_two_way_linked_list(static_circular_two_way_linked_list_t *list) {
    if (!list->head) {
        printf("List is empty\n");
        return;
    }
    static_circular_two_way_linked_list_node_t *current = list->head;
    do {
        printf("%d ", current->data);
        current = current->next;
    } while (current != list->head);
    printf("\n");
}

// Implementations for dynamic one-way linked list
dynamic_one_way_linked_list_t* create_dynamic_one_way_linked_list(void) {
    dynamic_one_way_linked_list_t *list = malloc(sizeof(dynamic_one_way_linked_list_t));
    list->head = NULL;
    return list;
}

void destroy_dynamic_one_way_linked_list(dynamic_one_way_linked_list_t *list) {
    one_way_linked_list_node_t *current = list->head;
    while (current) {
        one_way_linked_list_node_t *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void insert_dynamic_one_way_linked_list(dynamic_one_way_linked_list_t *list, int value) {
    one_way_linked_list_node_t *new_node = malloc(sizeof(one_way_linked_list_node_t));
    new_node->data = value;
    new_node->next = list->head;
    list->head = new_node;
}

void display_dynamic_one_way_linked_list(dynamic_one_way_linked_list_t *list) {
    one_way_linked_list_node_t *current = list->head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

// Implementations for dynamic two-way linked list
dynamic_two_way_linked_list_t* create_dynamic_two_way_linked_list(void) {
    dynamic_two_way_linked_list_t *list = malloc(sizeof(dynamic_two_way_linked_list_t));
    list->head = NULL;
    return list;
}

void destroy_dynamic_two_way_linked_list(dynamic_two_way_linked_list_t *list) {
    two_way_linked_list_node_t *current = list->head;
    while (current) {
        two_way_linked_list_node_t *next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void insert_dynamic_two_way_linked_list(dynamic_two_way_linked_list_t *list, int value) {
    two_way_linked_list_node_t *new_node = malloc(sizeof(two_way_linked_list_node_t));
    new_node->data = value;
    new_node->next = list->head;
    new_node->prev = NULL;

    if (list->head) {
        list->head->prev = new_node;
    }
    list->head = new_node;
}

void display_dynamic_two_way_linked_list(dynamic_two_way_linked_list_t *list) {
    two_way_linked_list_node_t *current = list->head;
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}


// Implementations for dynamic circular one-way linked list
dynamic_circular_one_way_linked_list_t* create_dynamic_circular_one_way_linked_list(void) {
    dynamic_circular_one_way_linked_list_t *list = malloc(sizeof(dynamic_circular_one_way_linked_list_t));
    list->head = NULL;
    return list;
}

void destroy_dynamic_circular_one_way_linked_list(dynamic_circular_one_way_linked_list_t *list) {
    if (!list->head) return;
    circular_one_way_linked_list_node_t *current = list->head;
    do {
        circular_one_way_linked_list_node_t *next = current->next;
        free(current);
        current = next;
    } while (current != list->head);
    free(list);
}

void insert_dynamic_circular_one_way_linked_list(dynamic_circular_one_way_linked_list_t *list, int value) {
    circular_one_way_linked_list_node_t *new_node = malloc(sizeof(circular_one_way_linked_list_node_t));
    new_node->data = value;

    if (!list->head) {
        list->head = new_node;
        new_node->next = new_node; // Point to itself
    } else {
        circular_one_way_linked_list_node_t *temp = list->head;
        while (temp->next != list->head) {
            temp = temp->next;
        }
        temp->next = new_node;
        new_node->next = list->head; // Make it circular
    }
}

void display_dynamic_circular_one_way_linked_list(dynamic_circular_one_way_linked_list_t *list) {
    if (!list->head) {
        printf("List is empty\n");
        return;
    }
    circular_one_way_linked_list_node_t *current = list->head;
    do {
        printf("%d ", current->data);
        current = current->next;
    } while (current != list->head);
    printf("\n");
}

// Implementations for dynamic circular two-way linked list
dynamic_circular_two_way_linked_list_t* create_dynamic_circular_two_way_linked_list(void) {
    dynamic_circular_two_way_linked_list_t *list = malloc(sizeof(dynamic_circular_two_way_linked_list_t));
    list->head = NULL;
    return list;
}

void destroy_dynamic_circular_two_way_linked_list(dynamic_circular_two_way_linked_list_t *list) {
    if (!list->head) return;
    circular_two_way_linked_list_node_t *current = list->head;
    do {
        circular_two_way_linked_list_node_t *next = current->next;
        free(current);
        current = next;
    } while (current != list->head);
    free(list);
}

void insert_dynamic_circular_two_way_linked_list(dynamic_circular_two_way_linked_list_t *list, int value) {
    circular_two_way_linked_list_node_t *new_node = malloc(sizeof(circular_two_way_linked_list_node_t));
    new_node->data = value;

    if (!list->head) {
        list->head = new_node;
        new_node->next = new_node; // Point to itself
        new_node->prev = new_node; // Point to itself
    } else {
        circular_two_way_linked_list_node_t *tail = list->head->prev;
        tail->next = new_node;
        new_node->prev = tail;
        new_node->next = list->head;
        list->head->prev = new_node; // Make it circular
    }
}

void display_dynamic_circular_two_way_linked_list(dynamic_circular_two_way_linked_list_t *list) {
    if (!list->head) {
        printf("List is empty\n");
        return;
    }
    circular_two_way_linked_list_node_t *current = list->head;
    do {
        printf("%d ", current->data);
        current = current->next;
    } while (current != list->head);
    printf("\n");
}

int main(void) {
    static_queue_t *sq = create_static_queue(5);
    enqueue_static_queue(sq, 10);
    enqueue_static_queue(sq, 20);
    printf("static queue\n");
    display_static_queue(sq);
    dequeue_static_queue(sq);
    printf("static queue\n");
    display_static_queue(sq);
    destroy_static_queue(sq);

    dynamic_queue_t *dq = create_dynamic_queue(2);
    enqueue_dynamic_queue(dq, 30);
    enqueue_dynamic_queue(dq, 40);
    enqueue_dynamic_queue(dq, 50); // Triggers resize
    printf("dynamic queue\n");
    display_dynamic_queue(dq);
    dequeue_dynamic_queue(dq);
    printf("dynamic queue\n");
    display_dynamic_queue(dq);
    destroy_dynamic_queue(dq);

    static_stack_t *ss = create_static_stack(3);
    push_static_stack(ss, 1);
    push_static_stack(ss, 2);
    printf("static stack\n");
    display_static_stack(ss);
    pop_static_stack(ss);
    printf("static stack\n");
    display_static_stack(ss);
    destroy_static_stack(ss);

    dynamic_stack_t *ds = create_dynamic_stack(2);
    push_dynamic_stack(ds, 1);
    push_dynamic_stack(ds, 2);
    push_dynamic_stack(ds, 3); // Triggers resize
    printf("dynamic stack\n");
    display_dynamic_stack(ds);
    pop_dynamic_stack(ds);
    printf("dynamic stack\n");
    display_dynamic_stack(ds);
    destroy_dynamic_stack(ds);

    one_way_linked_list_t *oll = create_one_way_linked_list();
    insert_one_way_linked_list(oll, 10);
    insert_one_way_linked_list(oll, 20);
    printf("one way linked list\n");
    display_one_way_linked_list(oll);
    destroy_one_way_linked_list(oll);

    two_way_linked_list_t *twll = create_two_way_linked_list();
    insert_two_way_linked_list(twll, 30);
    insert_two_way_linked_list(twll, 40);
    printf("two way linked list\n");
    display_two_way_linked_list(twll);
    destroy_two_way_linked_list(twll);

    circular_one_way_linked_list_t *cowll = create_circular_one_way_linked_list();
    insert_circular_one_way_linked_list(cowll, 50);
    insert_circular_one_way_linked_list(cowll, 60);
    printf("circular one way linked list\n");
    display_circular_one_way_linked_list(cowll);
    destroy_circular_one_way_linked_list(cowll);

    circular_two_way_linked_list_t *ctwll = create_circular_two_way_linked_list();
    insert_circular_two_way_linked_list(ctwll, 70);
    insert_circular_two_way_linked_list(ctwll, 80);
    printf("circular two way linked list\n");
    display_circular_two_way_linked_list(ctwll);
    destroy_circular_two_way_linked_list(ctwll);

    tree_t *tree = create_tree();
    insert_tree(tree, 5);
    insert_tree(tree, 3);
    insert_tree(tree, 7);
    insert_tree(tree, 2);
    insert_tree(tree, 4);
    insert_tree(tree, 6);
    insert_tree(tree, 8);
    printf("tree\n");
    display_tree(tree, tree->root, 0);
    destroy_tree(tree);

    static_circular_one_way_linked_list_t *sc_ow_list = create_static_circular_one_way_linked_list();
    insert_static_circular_one_way_linked_list(sc_ow_list, 1);
    insert_static_circular_one_way_linked_list(sc_ow_list, 2);
    insert_static_circular_one_way_linked_list(sc_ow_list, 3);
    printf("Static Circular One-Way Linked List:\n");
    display_static_circular_one_way_linked_list(sc_ow_list);
    destroy_static_circular_one_way_linked_list(sc_ow_list);

    static_circular_two_way_linked_list_t *sc_tw_list = create_static_circular_two_way_linked_list();
    insert_static_circular_two_way_linked_list(sc_tw_list, 1);
    insert_static_circular_two_way_linked_list(sc_tw_list, 2);
    insert_static_circular_two_way_linked_list(sc_tw_list, 3);
    printf("Static Circular Two-Way Linked List:\n");
    display_static_circular_two_way_linked_list(sc_tw_list);
    destroy_static_circular_two_way_linked_list(sc_tw_list);

    dynamic_one_way_linked_list_t *d_ow_list = create_dynamic_one_way_linked_list();
    insert_dynamic_one_way_linked_list(d_ow_list, 1);
    insert_dynamic_one_way_linked_list(d_ow_list, 2);
    insert_dynamic_one_way_linked_list(d_ow_list, 3);
    printf("Dynamic One-Way Linked List:\n");
    display_dynamic_one_way_linked_list(d_ow_list);
    destroy_dynamic_one_way_linked_list(d_ow_list);

    dynamic_two_way_linked_list_t *d_tw_list = create_dynamic_two_way_linked_list();
    insert_dynamic_two_way_linked_list(d_tw_list, 1);
    insert_dynamic_two_way_linked_list(d_tw_list, 2);
    insert_dynamic_two_way_linked_list(d_tw_list, 3);
    printf("Dynamic Two-Way Linked List:\n");
    display_dynamic_two_way_linked_list(d_tw_list);
    destroy_dynamic_two_way_linked_list(d_tw_list);

    dynamic_circular_one_way_linked_list_t *dc_ow_list = create_dynamic_circular_one_way_linked_list();
    insert_dynamic_circular_one_way_linked_list(dc_ow_list, 1);
    insert_dynamic_circular_one_way_linked_list(dc_ow_list, 2);
    insert_dynamic_circular_one_way_linked_list(dc_ow_list, 3);
    printf("Dynamic Circular One-Way Linked List:\n");
    display_dynamic_circular_one_way_linked_list(dc_ow_list);
    destroy_dynamic_circular_one_way_linked_list(dc_ow_list);

    dynamic_circular_two_way_linked_list_t *dc_tw_list = create_dynamic_circular_two_way_linked_list();
    insert_dynamic_circular_two_way_linked_list(dc_tw_list, 1);
    insert_dynamic_circular_two_way_linked_list(dc_tw_list, 2);
    insert_dynamic_circular_two_way_linked_list(dc_tw_list, 3);
    printf("Dynamic Circular Two-Way Linked List:\n");
    display_dynamic_circular_two_way_linked_list(dc_tw_list);
    destroy_dynamic_circular_two_way_linked_list(dc_tw_list);

    hash_table_t *hash_table = create_hash_table(10);
    insert_hash_table(hash_table, 1);
    insert_hash_table(hash_table, 2);
    insert_hash_table(hash_table, 12); // Collides with 2 if size is 10
    printf("Hash Table:\n");
    display_hash_table(hash_table);
    destroy_hash_table(hash_table);

    hash_map_t *hash_map = create_hash_map(10);
    hash_map_insert(hash_map, 1, 100);
    hash_map_insert(hash_map, 2, 200);
    hash_map_insert(hash_map, 3, 300);
    hash_map_insert(hash_map, 12, 400); // Collision with key 2 (if using a simple mod-based hash)

    printf("Hash Map:\n");
    display_hash_map(hash_map);

    printf("Value for key 2: %d\n", hash_map_get(hash_map, 2));
    printf("Value for key 12: %d\n", hash_map_get(hash_map, 12)); // Should show 400

    destroy_hash_map(hash_map);

    return 0;
}
