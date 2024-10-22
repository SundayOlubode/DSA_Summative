#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_s
{
        char word[50];
        char definition[256];
        struct node_s *left, *right;
} node_t;

node_t *root = NULL;

node_t *create_node(char *word, char *definition);
node_t *insert_word(node_t *root, char *word, char *definition);
node_t *min_value_node(node_t *node);
node_t *delete_word(node_t *root, char *word);
node_t *search_word(node_t *root, char *word);
void inorder_traversal(node_t *root);

int main()
{
        int choice;
        char word[50], definition[256];

        root = insert_word(root, "banana", "An elongated fruit");
        root = insert_word(root, "apple", "A fruit");
        root = insert_word(root, "date", "A sweet fruit");
        root = insert_word(root, "cherry", "A small, round fruit");

        while (1)
        {
                printf("\n1. Add Word\n2. Delete Word\n3. Search Word\n4. Display All Words\n5. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
                getchar();

                switch (choice)
                {
                case 1:
                        printf("Enter word(word must be in lowercase): ");
                        fgets(word, sizeof(word), stdin);
                        word[strcspn(word, "\n")] = 0;

                        printf("Enter definition: ");
                        fgets(definition, sizeof(definition), stdin);
                        definition[strcspn(definition, "\n")] = 0;

                        root = insert_word(root, word, definition);
                        printf("Word added successfully.\n");
                        break;

                case 2:
                        printf("Enter word to delete(word must be in lowercase): ");
                        fgets(word, sizeof(word), stdin);
                        word[strcspn(word, "\n")] = 0;

                        root = delete_word(root, word);
                        printf("Word deleted successfully.\n");
                        break;

                case 3:
                        printf("Enter word to search(word must be in lowercase): ");
                        fgets(word, sizeof(word), stdin);
                        word[strcspn(word, "\n")] = 0;

                        node_t *search_result = search_word(root, word);
                        if (search_result)
                                printf("Definition: %s\n", search_result->definition);
                        else
                                printf("Word not found.\n");
                        break;

                case 4:
                        printf("\nDictionary (Alphabetical Order):\n");
                        inorder_traversal(root);
                        break;

                case 5:
                        exit(0);

                default:
                        printf("Invalid choice.\n");
                }
        }

        return 0;
}

void inorder_traversal(node_t *root)
{
        if (root != NULL)
        {
                inorder_traversal(root->left);
                printf("%s: %s\n", root->word, root->definition);
                inorder_traversal(root->right);
        }
}

node_t *search_word(node_t *root, char *word)
{
        if (root == NULL || strcmp(word, root->word) == 0)
                return root;

        if (strcmp(word, root->word) < 0)
                return search_word(root->left, word);

        return search_word(root->right, word);
}

node_t *delete_word(node_t *root, char *word)
{
        if (root == NULL)
                return root;

        if (strcmp(word, root->word) < 0)
                root->left = delete_word(root->left, word);
        else if (strcmp(word, root->word) > 0)
                root->right = delete_word(root->right, word);
        else
        {
                if (root->left == NULL)
                {
                        node_t *temp = root->right;
                        free(root);
                        return temp;
                }
                else if (root->right == NULL)
                {
                        node_t *temp = root->left;
                        free(root);
                        return temp;
                }

                node_t *temp = min_value_node(root->right);
                strcpy(root->word, temp->word);
                strcpy(root->definition, temp->definition);
                root->right = delete_word(root->right, temp->word);
        }

        return root;
}

node_t *min_value_node(node_t *node)
{
        node_t *current = node;

        while (current && current->left != NULL)
                current = current->left;

        return current;
}

node_t *insert_word(node_t *root, char *word, char *definition)
{
        if (root == NULL)
                return create_node(word, definition);

        if (strcmp(word, root->word) < 0)
                root->left = insert_word(root->left, word, definition);
        else if (strcmp(word, root->word) > 0)
                root->right = insert_word(root->right, word, definition);

        return root;
}

node_t *create_node(char *word, char *definition)
{
        node_t *new_node = (node_t *)malloc(sizeof(node_t));
        strcpy(new_node->word, word);
        strcpy(new_node->definition, definition);
        new_node->left = new_node->right = NULL;
        return new_node;
}