#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PEOPLE 100
#define NAME_LEN 50
#define PHONE_LEN 15

typedef struct person_s
{
        char name[NAME_LEN];
        char phone_number[PHONE_LEN];
} person_t;

typedef struct graph_s
{
        person_t people[MAX_PEOPLE];
        int adj_matrix[MAX_PEOPLE][MAX_PEOPLE];
        int num_people;
} graph_t;

void init_graph(graph_t *g);
int find_person_index(graph_t *g, const char *name);
void bfs_trace_contacts(graph_t *g, const char *criminal_name);
int add_connection(graph_t *g, const char *name1, const char *name2);
void add_person(graph_t *g, const char *name, const char *phone_number);
void list_contacts(graph_t *g);
void add_new_connection(graph_t *g);

int main()
{
        graph_t network;
        init_graph(&network);
        char criminal[NAME_LEN];
        int choice;

        add_person(&network, "Pius", "123456789");
        add_person(&network, "David", "987654321");
        add_person(&network, "Magot", "555666777");
        add_person(&network, "Joshua", "444555666");

        add_connection(&network, "Pius", "David");
        add_connection(&network, "Pius", "Magot");
        add_connection(&network, "Magot", "Joshua");

        while (1)
        {
                printf("\n--- Contact Network Menu ---\n");
                printf("1. List existing contacts\n");
                printf("2. Add a new connection\n");
                printf("3. Trace criminal's contacts\n");
                printf("4. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                        list_contacts(&network);
                        break;

                case 2:
                        add_new_connection(&network);
                        break;

                case 3:
                        printf("Enter the name of the criminal (e.g Pius): ");
                        scanf("%s", criminal);
                        printf("\nTracing contacts for: %s\n", criminal);
                        bfs_trace_contacts(&network, criminal);
                        break;

                case 4:
                        exit(0);
                        break;

                default:
                        printf("Invalid choice. Please enter a number between 1 and 4.\n");
                }
        }

        return 0;
}

void init_graph(graph_t *g)
{
        g->num_people = 0;
        for (int i = 0; i < MAX_PEOPLE; i++)
        {
                for (int j = 0; j < MAX_PEOPLE; j++)
                {
                        g->adj_matrix[i][j] = 0;
                }
        }
}

void add_person(graph_t *g, const char *name, const char *phone_number)
{
        if (g->num_people >= MAX_PEOPLE)
        {
                printf("Graph is full. Cannot add more people.\n");
                return;
        }
        strcpy(g->people[g->num_people].name, name);
        strcpy(g->people[g->num_people].phone_number, phone_number);
        g->num_people++;
}

int find_person_index(graph_t *g, const char *name)
{
        for (int i = 0; i < g->num_people; i++)
        {
                if (strcmp(g->people[i].name, name) == 0)
                {
                        return i;
                }
        }
        return -1;
}

int add_connection(graph_t *g, const char *name1, const char *name2)
{
        int index1 = find_person_index(g, name1);
        int index2 = find_person_index(g, name2);

        if (index1 == -1)
        {
                printf("\n%s not found in the network!\n", name1);
                return (-1);
        }
        if (index2 == -1)
        {
                printf("\n%s not found in the network!\n", name2);
                return (-1);
        }

        g->adj_matrix[index1][index2] = 1;
        g->adj_matrix[index2][index1] = 1;
        return (0);
}

void list_contacts(graph_t *g)
{
        printf("\n--- Contact List ---\n");
        for (int i = 0; i < g->num_people; i++)
        {
                printf("%d. %s, Phone: %s\n", i + 1, g->people[i].name, g->people[i].phone_number);
                printf("   Direct Connections: ");
                for (int j = 0; j < g->num_people; j++)
                {
                        if (g->adj_matrix[i][j] == 1)
                        {
                                if (j != 0)
                                {
                                        printf(", ");
                                }
                                printf("%s", g->people[j].name);
                        }
                }
                printf("\n");
        }
}

void add_new_connection(graph_t *g)
{
        char name1[NAME_LEN], name2[NAME_LEN];
        int response;

        list_contacts(g);

        printf("\nEnter the name of the first person: ");
        scanf("%s", name1);
        printf("Enter the name of the second person: ");
        scanf("%s", name2);

        response = add_connection(g, name1, name2);
        if (response == -1)
        {
                "Connection not added.\n";
                return;
        }
        printf("Connection added between %s and %s.\n", name1, name2);
}

void bfs_trace_contacts(graph_t *g, const char *criminal_name)
{
        int criminal_index = find_person_index(g, criminal_name);
        if (criminal_index == -1)
        {
                printf("%s not found in the network!\n", criminal_name);
                return;
        }

        bool visited[MAX_PEOPLE] = {false};
        int level[MAX_PEOPLE] = {0};
        int queue[MAX_PEOPLE];
        int front = 0, rear = 0;

        visited[criminal_index] = true;
        queue[rear++] = criminal_index;

        while (front < rear)
        {
                int current_index = queue[front++];
                int current_level = level[current_index];

                printf("Level %d: %s\n", current_level, g->people[current_index].name);

                for (int i = 0; i < g->num_people; i++)
                {
                        if (g->adj_matrix[current_index][i] == 1 && !visited[i])
                        {
                                visited[i] = true;
                                queue[rear++] = i;
                                level[i] = current_level + 1;
                        }
                }
        }
}
