#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_NODES 512
#define MAX_FILENAME 256

typedef struct Node {
    unsigned char byte;
    unsigned int freq;
    struct Node *left, *right;
} Node;

typedef struct {
    unsigned char *data;
    unsigned int size;
} FileData;

Node* nodes[MAX_TREE_NODES];
int num_nodes = 0;

void insert_node(Node* n) {
    int i = num_nodes++;
    while (i && n->freq < nodes[(i-1)/2]->freq) {
        nodes[i] = nodes[(i-1)/2];
        i = (i-1)/2;
    }
    nodes[i] = n;
}

Node* remove_min() {
    Node* min = nodes[0];
    Node* last = nodes[--num_nodes];
    int i = 0, child;
    while ((i*2+1) < num_nodes) {
        child = (i*2+2 < num_nodes && nodes[i*2+2]->freq < nodes[i*2+1]->freq) ? i*2+2 : i*2+1;
        if (last->freq <= nodes[child]->freq) break;
        nodes[i] = nodes[child];
        i = child;
    }
    nodes[i] = last;
    return min;
}

void build_freq_table(unsigned char *data, int size, unsigned int *freq) {
    for (int i = 0; i < size; i++) freq[data[i]]++;
}

Node* build_tree(unsigned int *freq) {
    for (int i = 0; i < 256; i++)
        if (freq[i]) {
            Node* n = malloc(sizeof(Node));
            n->byte = i; n->freq = freq[i]; n->left = n->right = NULL;
            insert_node(n);
        }
    while (num_nodes > 1) {
        Node* l = remove_min();
        Node* r = remove_min();
        Node* m = malloc(sizeof(Node));
        m->byte = 0; m->freq = l->freq + r->freq;
        m->left = l; m->right = r;
        insert_node(m);
    }
    return remove_min();
}

void build_codes(Node *node, char **codes, char *code, int depth) {
    if (!node->left && !node->right) {
        code[depth] = '\0';
        codes[node->byte] = strdup(code);
        return;
    }
    if (node->left) { code[depth]='0'; build_codes(node->left, codes, code, depth+1); }
    if (node->right){ code[depth]='1'; build_codes(node->right, codes, code, depth+1); }
}

void write_tree(Node *node, FILE *fout) {
    if (!node->left && !node->right) {
        fputc('1', fout);
        fputc(node->byte, fout);
    } else {
        fputc('0', fout);
        write_tree(node->left, fout);
        write_tree(node->right, fout);
    }
}

Node* read_tree(FILE *fin) {
    int flag = fgetc(fin);
    if (flag == '1') {
        Node* n = malloc(sizeof(Node));
        n->byte = fgetc(fin);
        n->left = n->right = NULL;
        return n;
    } else {
        Node* n = malloc(sizeof(Node));
        n->left = read_tree(fin);
        n->right = read_tree(fin);
        return n;
    }
}

void encode_file(const char *filename, FILE *fout, char **codes) {
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    rewind(f);
    unsigned char *data = malloc(size);
    fread(data, 1, size, f);
    fclose(f);

    fwrite(&size, sizeof(int), 1, fout);
    unsigned char name_len = strlen(filename);
    fwrite(&name_len, 1, 1, fout);
    fwrite(filename, 1, name_len, fout);

    unsigned char byte = 0, bit_pos = 0;
    for (int i = 0; i < size; i++) {
        char *code = codes[data[i]];
        for (int j = 0; code[j]; j++) {
            if (code[j] == '1') byte |= (1 << (7 - bit_pos));
            bit_pos++;
            if (bit_pos == 8) {
                fputc(byte, fout);
                byte = 0; bit_pos = 0;
            }
        }
    }
    if (bit_pos) fputc(byte, fout);

    free(data);
}

void decode_file(FILE *fin, const char *outdir, Node *tree) {
    int size;
    fread(&size, sizeof(int), 1, fin);
    unsigned char name_len;
    fread(&name_len, 1, 1, fin);
    char name[MAX_FILENAME];
    fread(name, 1, name_len, fin);
    name[name_len] = '\0';

    char outpath[MAX_FILENAME];
    snprintf(outpath, MAX_FILENAME, "%s/%s", outdir, name);
    FILE *fout = fopen(outpath, "wb");

    Node *node = tree;
    int total = 0;
    int c;
    unsigned char byte;
    int bit;

    while (total < size) {
        c = fgetc(fin);
        for (int i = 0; i < 8; i++) {
            bit = (c >> (7-i)) & 1;
            node = bit ? node->right : node->left;
            if (!node->left && !node->right) {
                fputc(node->byte, fout);
                total++;
                if (total == size) break;
                node = tree;
            }
        }
    }
    fclose(fout);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage:\n Compress: %s c output.haf file1 file2 ...\n Decompress: %s d archive.haf outdir\n", argv[0], argv[0]);
        return 1;
    }

    if (argv[1][0] == 'c') {
        unsigned int freq[256] = {0};
        for (int i = 3; i < argc; i++) {
            FILE *f = fopen(argv[i], "rb");
            fseek(f, 0, SEEK_END);
            int size = ftell(f);
            rewind(f);
            unsigned char *data = malloc(size);
            fread(data, 1, size, f);
            fclose(f);
            build_freq_table(data, size, freq);
            free(data);
        }

        Node *tree = build_tree(freq);
        char *codes[256] = {0};
        char code[256];
        build_codes(tree, codes, code, 0);

        FILE *fout = fopen(argv[2], "wb");
        write_tree(tree, fout);
        fputc('\n', fout);
        for (int i = 3; i < argc; i++)
            encode_file(argv[i], fout, codes);
        fclose(fout);
        printf("Archive created: %s\n", argv[2]);

    } else if (argv[1][0] == 'd') {
        FILE *fin = fopen(argv[2], "rb");
        Node *tree = read_tree(fin);
        fgetc(fin); // skip newline
        while (!feof(fin)) {
            int c = fgetc(fin);
            if (c == EOF) break;
            ungetc(c, fin);
            decode_file(fin, argv[3], tree);
        }
        fclose(fin);
        printf("Archive extracted to %s\n", argv[3]);
    }

    return 0;
}

