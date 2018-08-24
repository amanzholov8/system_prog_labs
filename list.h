struct list_node {
	struct list_node *next;
	struct list_node *prev;
	int key;
	void *value;
};

// Basic list functions
int insert_node (struct list_node *node, struct list_node *new_node);
int del_node (struct list_node *node);
struct list_node *search_list (int search_key);

int count_list_length (struct list_node *head);
int is_list_empty (struct list_node *head);
void iterate_print_keys (struct list_node *head);

// Sorted list functions
int insert_sorted_by_key (struct list_node *head, struct list_node *new_node);
