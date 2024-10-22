#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 0
#define BLACK 1

typedef struct student_s
{
        int id;
        char name[100];
        float grade;
        int color; /* RED | BLACK */
        struct student_s *left, *right, *parent;
} student_t;

void insert_student(char *name, float grade);
void delete_student(int id);
student_t *search_student(int id);
void inorder_traversal(student_t *node);
void update_student(int id, char *new_name, float new_grade);
student_t *create_node(int id, char *name, float grade);
void left_rotate(student_t *x);
void right_rotate(student_t *x);
void rb_insert_fixup(student_t *z);
void rb_transplant(student_t *u, student_t *v);
student_t *tree_minimum(student_t *x);
void rb_delete_fixup(student_t *x);

student_t *root = NULL;
student_t *NIL;
int unique_id = 1;

int main()
{
        NIL = (student_t *)malloc(sizeof(student_t));
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL->parent = NIL;

        root = NIL;

        int choice, id;
        float grade;
        char name[100];

        while (1)
        {
                printf("\n1. Add Student\n2. Delete Student\n3. Search Student\n4. Display All Students\n5. Update Student\n6. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                        getchar();
                        printf("Enter student name: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = 0;
                        printf("Enter student grade: ");
                        scanf("%f", &grade);

                        insert_student(name, grade);
                        printf("Student added successfully.\n");
                        break;

                case 2:
                        printf("Enter student ID to delete: ");
                        scanf("%d", &id);
                        delete_student(id);
                        break;

                case 3:
                        printf("Enter student ID to search: ");
                        scanf("%d", &id);
                        student_t *student = search_student(id);
                        if (student != NIL)
                                printf("ID: %d, Name: %s, Grade: %.2f\n", student->id, student->name, student->grade);
                        else
                                printf("Student not found.\n");
                        break;

                case 4:
                        printf("Student records in ascending order of names:\n");
                        inorder_traversal(root);
                        break;

                case 5:
                        printf("Enter student ID to update: ");
                        scanf("%d", &id);
                        getchar();
                        printf("Enter new student name: ");
                        fgets(name, sizeof(name), stdin);
                        name[strcspn(name, "\n")] = 0;
                        printf("Enter new student grade: ");
                        scanf("%f", &grade);

                        update_student(id, name, grade);
                        printf("Student updated successfully.\n");
                        break;

                case 6:
                        exit(0);

                default:
                        printf("Invalid choice.\n");
                }
        }

        return 0;
}

student_t *create_node(int id, char *name, float grade)
{
        student_t *new_node = (student_t *)malloc(sizeof(student_t));
        new_node->id = id;
        strcpy(new_node->name, name);
        new_node->grade = grade;
        new_node->color = RED;
        new_node->left = new_node->right = new_node->parent = NIL;
        return new_node;
}

void left_rotate(student_t *x)
{
        student_t *y = x->right;
        x->right = y->left;
        if (y->left != NIL)
                y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == NULL)
                root = y;
        else if (x == x->parent->left)
                x->parent->left = y;
        else
                x->parent->right = y;
        y->left = x;
        x->parent = y;
}

void right_rotate(student_t *x)
{
        student_t *y = x->left;
        x->left = y->right;
        if (y->right != NIL)
                y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == NULL)
                root = y;
        else if (x == x->parent->right)
                x->parent->right = y;
        else
                x->parent->left = y;
        y->right = x;
        x->parent = y;
}

void rb_insert_fixup(student_t *z)
{
        while (z->parent->color == RED)
        {
                if (z->parent == z->parent->parent->left)
                {
                        student_t *y = z->parent->parent->right;
                        if (y->color == RED)
                        {
                                z->parent->color = BLACK;
                                y->color = BLACK;
                                z->parent->parent->color = RED;
                                z = z->parent->parent;
                        }
                        else
                        {
                                if (z == z->parent->right)
                                {
                                        z = z->parent;
                                        left_rotate(z);
                                }
                                z->parent->color = BLACK;
                                z->parent->parent->color = RED;
                                right_rotate(z->parent->parent);
                        }
                }
                else
                {
                        student_t *y = z->parent->parent->left;
                        if (y->color == RED)
                        {
                                z->parent->color = BLACK;
                                y->color = BLACK;
                                z->parent->parent->color = RED;
                                z = z->parent->parent;
                        }
                        else
                        {
                                if (z == z->parent->left)
                                {
                                        z = z->parent;
                                        right_rotate(z);
                                }
                                z->parent->color = BLACK;
                                z->parent->parent->color = RED;
                                left_rotate(z->parent->parent);
                        }
                }
        }
        root->color = BLACK;
}

void insert_student(char *name, float grade)
{
        student_t *z = create_node(unique_id++, name, grade);
        student_t *y = NIL;
        student_t *x = root;

        while (x != NIL)
        {
                y = x;
                if (strcmp(z->name, x->name) < 0)
                        x = x->left;
                else
                        x = x->right;
        }

        z->parent = y;
        if (y == NIL)
                root = z;
        else if (strcmp(z->name, y->name) < 0)
                y->left = z;
        else
                y->right = z;

        rb_insert_fixup(z);
}

void rb_transplant(student_t *u, student_t *v)
{
        if (u->parent == NIL)
                root = v;
        else if (u == u->parent->left)
                u->parent->left = v;
        else
                u->parent->right = v;
        v->parent = u->parent;
}

student_t *tree_minimum(student_t *x)
{
        while (x->left != NIL)
                x = x->left;
        return x;
}

void rb_delete_fixup(student_t *x)
{
        while (x != root && x->color == BLACK)
        {
                if (x == x->parent->left)
                {
                        student_t *w = x->parent->right;
                        if (w->color == RED)
                        {
                                w->color = BLACK;
                                x->parent->color = RED;
                                left_rotate(x->parent);
                                w = x->parent->right;
                        }
                        if (w->left->color == BLACK && w->right->color == BLACK)
                        {
                                w->color = RED;
                                x = x->parent;
                        }
                        else
                        {
                                if (w->right->color == BLACK)
                                {
                                        w->left->color = BLACK;
                                        w->color = RED;
                                        right_rotate(w);
                                        w = x->parent->right;
                                }
                                w->color = x->parent->color;
                                x->parent->color = BLACK;
                                w->right->color = BLACK;
                                left_rotate(x->parent);
                                x = root;
                        }
                }
                else
                {
                        student_t *w = x->parent->left;
                        if (w->color == RED)
                        {
                                w->color = BLACK;
                                x->parent->color = RED;
                                right_rotate(x->parent);
                                w = x->parent->left;
                        }
                        if (w->right->color == BLACK && w->left->color == BLACK)
                        {
                                w->color = RED;
                                x = x->parent;
                        }
                        else
                        {
                                if (w->left->color == BLACK)
                                {
                                        w->right->color = BLACK;
                                        w->color = RED;
                                        left_rotate(w);
                                        w = x->parent->left;
                                }
                                w->color = x->parent->color;
                                x->parent->color = BLACK;
                                w->left->color = BLACK;
                                right_rotate(x->parent);
                                x = root;
                        }
                }
        }
        x->color = BLACK;
}

void delete_student(int id)
{
        student_t *z = search_student(id);
        if (z == NIL)
        {
                printf("Student with ID %d not found.\n", id);
                return;
        }

        student_t *y = z;
        int y_original_color = y->color;
        student_t *x;

        if (z->left == NIL)
        {
                x = z->right;
                rb_transplant(z, z->right);
        }
        else if (z->right == NIL)
        {
                x = z->left;
                rb_transplant(z, z->left);
        }
        else
        {
                y = tree_minimum(z->right);
                y_original_color = y->color;
                x = y->right;
                if (y->parent == z)
                        x->parent = y;
                else
                {
                        rb_transplant(y, y->right);
                        y->right = z->right;
                        y->right->parent = y;
                }
                rb_transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
        }

        if (y_original_color == BLACK)
                rb_delete_fixup(x);

        free(z);
        printf("Student with ID %d deleted.\n", id);
}

student_t *search_student(int id)
{
        student_t *current = root;
        while (current != NIL && current->id != id)
        {
                if (id < current->id)
                        current = current->left;
                else
                        current = current->right;
        }
        return current;
}

void inorder_traversal(student_t *node)
{
        if (node != NIL)
        {
                inorder_traversal(node->left);
                printf("ID: %d, Name: %s, Grade: %.2f\n", node->id, node->name, node->grade);
                inorder_traversal(node->right);
        }
}

void update_student(int id, char *new_name, float new_grade)
{
        student_t *student = search_student(id);
        if (student != NIL)
        {
                strcpy(student->name, new_name);
                student->grade = new_grade;
        }
        else
        {
                printf("Student with ID %d not found.\n", id);
        }
}
