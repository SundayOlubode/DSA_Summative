#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct task_s
{
        int id;
        char description[256];
        struct task_s *next;
} task_t;

task_t *head = NULL;
int task_id = 1;

void add_task(char *description);
void remove_task(int id);
void display_tasks();
void mark_task_completed(int id);

int main()
{
        int choice, id;
        char description[256];

        while (1)
        {
                printf("\n1. Add Task\n2. Remove Task\n3. Mark Task as Completed\n4. Display All Tasks\n5. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                        getchar();
                        printf("Enter task description: ");
                        fgets(description, sizeof(description), stdin);
                        description[strcspn(description, "\n")] = 0;
                        add_task(description);
                        printf("Task added successfully.\n");
                        break;

                case 2:
                        printf("Enter task ID to remove: ");
                        scanf("%d", &id);
                        remove_task(id);
                        break;

                case 3:
                        printf("Enter task ID to mark as completed: ");
                        scanf("%d", &id);
                        mark_task_completed(id);
                        break;

                case 4:
                        display_tasks();
                        break;

                case 5:
                        exit(0);

                default:
                        printf("Invalid choice.\n");
                }
        }

        return 0;
}

void add_task(char *description)
{
        task_t *new_task = (task_t *)malloc(sizeof(task_t));
        new_task->id = task_id++;
        strcpy(new_task->description, description);
        new_task->next = NULL;

        if (head == NULL)
        {
                head = new_task;
        }
        else
        {
                task_t *temp = head;
                while (temp->next != NULL)
                {
                        temp = temp->next;
                }
                temp->next = new_task;
        }
}

void remove_task(int id)
{
        task_t *temp = head, *prev = NULL;

        while (temp != NULL && temp->id != id)
        {
                prev = temp;
                temp = temp->next;
        }

        if (temp == NULL)
        {
                printf("Task not found.\n");
                return;
        }

        if (prev == NULL)
                head = temp->next;
        else
                prev->next = temp->next;

        free(temp);
        printf("Task removed successfully.\n");
}

void display_tasks()
{
        task_t *temp = head;

        if (temp == NULL)
        {
                printf("No tasks available.\n");
                return;
        }

        while (temp != NULL)
        {
                printf("ID: %d, Description: %s\n", temp->id, temp->description);
                temp = temp->next;
        }
}

void mark_task_completed(int id)
{
        remove_task(id);
        printf("Task marked as completed and removed.\n");
}
