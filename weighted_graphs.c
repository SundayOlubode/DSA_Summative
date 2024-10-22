#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INF INT_MAX

typedef struct edge_s
{
	int src, dest, weight;
} edge_t;

typedef struct graph_s
{
	int num_vertices, num_edges;
	edge_t *edges;
	char **vertex_names;
} graph_t;

graph_t *create_graph(int num_vertices, int num_edges);
void dijkstra(graph_t *graph, int src);
void bellman_ford(graph_t *graph, int src);
void print_solution(int dist[], int V, char **vertex_names);
int find_vertex_index(graph_t *graph, char *name);

int main()
{
	int num_vertices, num_edges, choice;
	char src_name[50], dest_name[50];

	printf("Enter the number of vertices: ");
	scanf("%d", &num_vertices);

	printf("Enter the number of edges: ");
	scanf("%d", &num_edges);

	graph_t *graph = create_graph(num_vertices, num_edges);

	printf("Enter the edges with source, destination, and weight:\n");
	for (int i = 0; i < num_edges; i++)
	{
		int u, v, w;

		printf("Edge %d - Source Vertex: ", i + 1);
		scanf("%s", src_name);

		printf("Edge %d - Destination Vertex: ", i + 1);
		scanf("%s", dest_name);

		printf("Edge %d - Weight: ", i + 1);
		scanf("%d", &w);

		u = find_vertex_index(graph, src_name);
		v = find_vertex_index(graph, dest_name);

		if (u == -1 || v == -1)
		{
			printf("Invalid vertex names!\n");
			i--;
			continue;
		}

		graph->edges[i].src = u;
		graph->edges[i].dest = v;
		graph->edges[i].weight = w;
	}

	while (1)
	{
		printf("\n");
		printf("Find the shortest distance for a source vertex.\n");
		printf("Enter the source vertex: ");
		scanf("%s", src_name);
		int src = find_vertex_index(graph, src_name);
		if (src == -1)
		{
			printf("Invalid source vertex!\n");
			free(graph->edges);
			free(graph);
			return 1;
		}
		printf("\n");
		printf("Choose Algorithm:\n");
		printf("1. Dijkstra(positive weights)\n");
		printf("2. Bellman-Ford (negative weights)\n");
		printf("3. to exit: ");
		scanf("%d", &choice);

		if (choice == 1)
		{
			dijkstra(graph, src);
			continue;
		}
		else if (choice == 2)
		{
			bellman_ford(graph, src);
			continue;
		}
		else
		{
			break;
		}
	}

	for (int i = 0; i < num_vertices; i++)
	{
		free(graph->vertex_names[i]);
	}
	free(graph->vertex_names);
	free(graph->edges);
	free(graph);

	return 0;
}

graph_t *create_graph(int num_vertices, int num_edges)
{
	graph_t *graph = (graph_t *)malloc(sizeof(graph_t));
	graph->num_vertices = num_vertices;
	graph->num_edges = num_edges;
	graph->edges = (edge_t *)malloc(num_edges * sizeof(edge_t));
	graph->vertex_names = (char **)malloc(num_vertices * sizeof(char *));

	printf("Enter the names for the vertices:\n");
	for (int i = 0; i < num_vertices; i++)
	{
		graph->vertex_names[i] = (char *)malloc(50 * sizeof(char));
		printf("Vertex %d name(lowercase): ", i + 1);
		scanf("%s", graph->vertex_names[i]);
	}

	return graph;
}

int find_vertex_index(graph_t *graph, char *name)
{
	for (int i = 0; i < graph->num_vertices; i++)
	{
		if (strcmp(graph->vertex_names[i], name) == 0)
			return i;
	}
	return -1;
}

void print_solution(int dist[], int V, char **vertex_names)
{
	printf("Vertex Distance from Source\n");
	for (int i = 0; i < V; i++)
	{
		if (dist[i] == INF)
		{
			printf("%s \t\t INF\n", vertex_names[i]);
		}
		else
		{
			printf("%s \t\t %d\n", vertex_names[i], dist[i]);
		}
	}
}

void dijkstra(graph_t *graph, int src)
{
	int V = graph->num_vertices;
	int dist[V];
	int visited[V];

	for (int i = 0; i < V; i++)
	{
		dist[i] = INF;
		visited[i] = 0;
	}

	dist[src] = 0;

	for (int count = 0; count < V - 1; count++)
	{
		int u = -1;
		for (int i = 0; i < V; i++)
		{
			if (!visited[i] && (u == -1 || dist[i] < dist[u]))
			{
				u = i;
			}
		}

		if (u == -1)
			break;

		visited[u] = 1;

		for (int i = 0; i < graph->num_edges; i++)
		{
			int u_src = graph->edges[i].src;
			int dest = graph->edges[i].dest;
			int weight = graph->edges[i].weight;

			if (u_src == u && !visited[dest] && dist[u_src] != INF && dist[u_src] + weight < dist[dest])
			{
				dist[dest] = dist[u_src] + weight;
			}
		}
	}

	print_solution(dist, V, graph->vertex_names);
}

void bellman_ford(graph_t *graph, int src)
{
	int V = graph->num_vertices;
	int E = graph->num_edges;
	int dist[V];

	for (int i = 0; i < V; i++)
	{
		dist[i] = INF;
	}
	dist[src] = 0;

	for (int i = 1; i <= V - 1; i++)
	{
		for (int j = 0; j < E; j++)
		{
			int u = graph->edges[j].src;
			int v = graph->edges[j].dest;
			int weight = graph->edges[j].weight;
			if (dist[u] != INF && dist[u] + weight < dist[v])
			{
				dist[v] = dist[u] + weight;
			}
		}
	}

	for (int i = 0; i < E; i++)
	{
		int u = graph->edges[i].src;
		int v = graph->edges[i].dest;
		int weight = graph->edges[i].weight;
		if (dist[u] != INF && dist[u] + weight < dist[v])
		{
			printf("Graph contains negative weight cycle\n");
			return;
		}
	}

	print_solution(dist, V, graph->vertex_names);
}
