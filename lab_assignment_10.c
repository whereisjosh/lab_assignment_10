#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct trie_node {
    struct trie_node* children[26];
    int count;
};

struct trie_node* createtrie_node() {
    struct trie_node* newNode = (struct trie_node*)malloc(sizeof(struct trie_node));
    if (newNode) {
        newNode->count = 0;
        for (int i = 0; i < 26; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

struct trie {
    struct trie_node* root;
};

struct trie* createtrie() {
    struct trie* newtrie = (struct trie*)malloc(sizeof(struct trie));
    if (newtrie) {
        newtrie->root = createtrie_node();
    }
    return newtrie;
}

void insert(struct trie* ptrie, char* word) {

    struct trie_node* currentNode = ptrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= 26) {
            return;
        }
        if (currentNode->children[index] == NULL) {
            currentNode->children[index] = createtrie_node();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->count++;
}

int numberOfOccurrences(struct trie* ptrie, char* word) {
    struct trie_node* currentNode = ptrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (currentNode->children[index] == NULL) {
            return 0;
        }
        currentNode = currentNode->children[index];
    }
    return currentNode->count;
}

struct trie_node* deallocatetrie_node(struct trie_node* node) {
    if (node) {
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                deallocatetrie_node(node->children[i]);
            }
        }
        free(node);
    }
    return NULL;
}

struct trie* deallocatetrie(struct trie* ptrie) {
    if (ptrie) {
        ptrie->root = deallocatetrie_node(ptrie->root);
        free(ptrie);
    }
    return NULL;
}

int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Can't open file%s\n", filename);
        exit(1);
    }

    int numWords = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        pInWords[numWords] = strdup(buffer);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char* inWords[256];

    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct trie* ptrie = createtrie();
    for (int i = 0; i < numWords; i++) {
        insert(ptrie, inWords[i]);
    }

    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(ptrie, pWords[i]));
    }

    // Deallocate the trie data structure
    ptrie = deallocatetrie(ptrie);
    if (ptrie != NULL) {
        printf("There is an error in this program\n");
    }

    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}