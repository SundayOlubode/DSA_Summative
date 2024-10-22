#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100

typedef struct node_s
{
        char data;
        unsigned freq;
        struct node_s *left, *right;
} node_t;

typedef struct minheap_s
{
        unsigned size;
        unsigned capacity;
        node_t **array;
} minheap_t;

node_t *root = NULL;

node_t *create_node(char data, unsigned freq);
minheap_t *create_minheap(unsigned capacity);
void swap_nodes(node_t **a, node_t **b);
void minheapify(minheap_t *minHeap, int idx);
int is_size_one(minheap_t *minHeap);
node_t *extract_min(minheap_t *minHeap);
void insert_minheap(minheap_t *minHeap, node_t *node);
minheap_t *build_minheap(char data[], int freq[], int size);
node_t *build_huffman_tree(char data[], int freq[], int size);
void store_codes(node_t *root, int arr[], int top, char huffman_codes[256][MAX_TREE_HT]);
void calculate_freq(FILE *fp, int freq[256], char **text, long *size);
void write_bits(FILE *fp, unsigned char *bit_buffer, int *bit_count);
void compress_file(char *input_file, char *output_file);
void decompress_file(char *input_file, char *output_file, node_t *root);
long get_file_size(char *filename);

int main()
{
        char input_file[100], compressed_file[100], decompressed_file[100];

        printf("Enter the name of the input file: ");
        scanf("%s", input_file);
        printf("You inputted ./%s\n", input_file);

        printf("Enter the name of the compressed output file: ");
        scanf("%s", compressed_file);

        compress_file(input_file, compressed_file);

        printf("Enter a name for the decompressed output file (including .[extension]): ");
        scanf("%s", decompressed_file);

        decompress_file(compressed_file, decompressed_file, root);

        printf("\n");
        printf("Compression and decompression complete.\n");
        printf("Original file size before compression: %ld bytes\n", get_file_size(input_file));
        printf("Size of the compressed file: %ld bytes\n", get_file_size(compressed_file));
        printf("Size of the decompressed file: %ld bytes\n", get_file_size(decompressed_file));

        return 0;
}

node_t *create_node(char data, unsigned freq)
{
        node_t *node = (node_t *)malloc(sizeof(node_t));
        node->left = node->right = NULL;
        node->data = data;
        node->freq = freq;
        return node;
}

minheap_t *create_minheap(unsigned capacity)
{
        minheap_t *minHeap = (minheap_t *)malloc(sizeof(minheap_t));
        minHeap->size = 0;
        minHeap->capacity = capacity;
        minHeap->array = (node_t **)malloc(minHeap->capacity * sizeof(node_t *));
        return minHeap;
}

void swap_nodes(node_t **a, node_t **b)
{
        node_t *t = *a;
        *a = *b;
        *b = t;
}

void minheapify(minheap_t *minHeap, int idx)
{
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        {
                smallest = left;
        }

        if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        {
                smallest = right;
        }

        if (smallest != idx)
        {
                swap_nodes(&minHeap->array[smallest], &minHeap->array[idx]);
                minheapify(minHeap, smallest);
        }
}

int is_size_one(minheap_t *minHeap)
{
        return (minHeap->size == 1);
}

node_t *extract_min(minheap_t *minHeap)
{
        node_t *temp = minHeap->array[0];
        minHeap->array[0] = minHeap->array[minHeap->size - 1];
        --minHeap->size;
        minheapify(minHeap, 0);
        return temp;
}

void insert_minheap(minheap_t *minHeap, node_t *node)
{
        ++minHeap->size;
        int i = minHeap->size - 1;
        while (i && node->freq < minHeap->array[(i - 1) / 2]->freq)
        {
                minHeap->array[i] = minHeap->array[(i - 1) / 2];
                i = (i - 1) / 2;
        }
        minHeap->array[i] = node;
}

minheap_t *build_minheap(char data[], int freq[], int size)
{
        minheap_t *minHeap = create_minheap(size);
        for (int i = 0; i < size; ++i)
        {
                minHeap->array[i] = create_node(data[i], freq[i]);
        }
        minHeap->size = size;
        for (int i = (minHeap->size - 1) / 2; i >= 0; --i)
        {
                minheapify(minHeap, i);
        }
        return minHeap;
}

node_t *build_huffman_tree(char data[], int freq[], int size)
{
        node_t *left, *right, *top;

        minheap_t *minHeap = build_minheap(data, freq, size);

        while (!is_size_one(minHeap))
        {
                left = extract_min(minHeap);
                right = extract_min(minHeap);
                top = create_node('$', left->freq + right->freq);
                top->left = left;
                top->right = right;
                insert_minheap(minHeap, top);
        }

        return extract_min(minHeap);
}

void store_codes(node_t *root, int arr[], int top, char huffman_codes[256][MAX_TREE_HT])
{
        if (root->left)
        {
                arr[top] = 0;
                store_codes(root->left, arr, top + 1, huffman_codes);
        }

        if (root->right)
        {
                arr[top] = 1;
                store_codes(root->right, arr, top + 1, huffman_codes);
        }

        if (!(root->left) && !(root->right))
        {
                char code[MAX_TREE_HT];
                for (int i = 0; i < top; i++)
                {
                        code[i] = arr[i] + '0';
                }
                code[top] = '\0';
                strcpy(huffman_codes[(unsigned char)root->data], code);
        }
}

void calculate_freq(FILE *fp, int freq[256], char **text, long *size)
{
        *size = 0;
        unsigned char ch;
        while (fread(&ch, sizeof(unsigned char), 1, fp) == 1)
        {
                freq[ch]++;
                (*text)[(*size)++] = ch;
        }
        (*text)[*size] = '\0';
}

void write_bits(FILE *fp, unsigned char *bit_buffer, int *bit_count)
{
        if (*bit_count >= 8)
        {
                fwrite(bit_buffer, sizeof(unsigned char), 1, fp);
                *bit_buffer = 0;
                *bit_count = 0;
        }
}

void compress_file(char *input_file, char *output_file)
{
        int freq[256] = {0};
        char *text = (char *)malloc(100000);
        long text_size = 0;

        FILE *fp = fopen(input_file, "rb");
        if (!fp)
        {
                printf("Failed to open input file.\n");
                free(text);
                return;
        }

        calculate_freq(fp, freq, &text, &text_size);
        fclose(fp);

        char unique_chars[256];
        int unique_freq[256];
        int size = 0;

        for (int i = 0; i < 256; i++)
        {
                if (freq[i])
                {
                        unique_chars[size] = i;
                        unique_freq[size] = freq[i];
                        size++;
                }
        }

        root = build_huffman_tree(unique_chars, unique_freq, size);

        char huffman_codes[256][MAX_TREE_HT];
        int arr[MAX_TREE_HT], top = 0;
        store_codes(root, arr, top, huffman_codes);

        FILE *out_fp = fopen(output_file, "wb");
        if (!out_fp)
        {
                printf("Failed to open output file.\n");
                free(text);
                return;
        }

        unsigned char bit_buffer = 0;
        int bit_count = 0;

        for (long i = 0; i < text_size; i++)
        {
                char *code = huffman_codes[(unsigned char)text[i]];
                for (int j = 0; code[j] != '\0'; j++)
                {
                        bit_buffer = (bit_buffer << 1) | (code[j] - '0');
                        bit_count++;

                        write_bits(out_fp, &bit_buffer, &bit_count);
                }
        }

        if (bit_count > 0)
        {
                bit_buffer <<= (8 - bit_count);
                fwrite(&bit_buffer, sizeof(unsigned char), 1, out_fp);
        }

        fclose(out_fp);
        free(text);
}

void decompress_file(char *input_file, char *output_file, node_t *root)
{
        FILE *fp = fopen(input_file, "rb");
        if (!fp)
        {
                printf("Failed to open input file for decompression.\n");
                return;
        }

        FILE *out_fp = fopen(output_file, "wb");
        if (!out_fp)
        {
                printf("Failed to open output file for decompression.\n");
                fclose(fp);
                return;
        }

        node_t *current = root;
        unsigned char bit_buffer;
        int bits_read = 0;
        while (fread(&bit_buffer, sizeof(unsigned char), 1, fp) == 1)
        {
                for (int i = 0; i < 8; i++)
                {
                        if (current->left == NULL && current->right == NULL)
                        {
                                fwrite(&current->data, sizeof(char), 1, out_fp);
                                current = root;
                        }

                        int bit = (bit_buffer >> (7 - i)) & 1;
                        current = (bit == 0) ? current->left : current->right;
                }
        }

        if (current->left == NULL && current->right == NULL)
        {
                fwrite(&current->data, sizeof(char), 1, out_fp);
        }

        fclose(fp);
        fclose(out_fp);
}

long get_file_size(char *filename)
{
        FILE *fp = fopen(filename, "rb");
        if (!fp)
        {
                printf("Failed to open file to get size.\n");
                return -1;
        }

        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fclose(fp);
        return size;
}
