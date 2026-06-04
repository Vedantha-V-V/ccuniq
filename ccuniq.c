#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#define SIZE 4096

char* trim(char *str) {
    int length = strlen(str);
    while (length > 0 && isspace((unsigned char)str[length - 1])) {
        length--;
    }
    str[length] = '\0';
    return str;
}

struct node {
    char* key;
    int value;
    bool visited;
    struct node* next;
};

struct node* get_node(char* key, int value){
    struct node*node = (struct node*)malloc(sizeof(struct node));
    node->key = key;
    node->value = value;
    node->visited = false;
    node->next = NULL;
    return node;
};

struct hash_map {
    int num, capacity;
    struct node** arr;
};

int hash(struct hash_map* map, char* key)
{
    int index;
    int sum = 0, factor = 31;
    for (int i = 0; i < strlen(key); i++) {
        sum = ((sum % map->capacity) + (((int)key[i]) * factor) % map->capacity) % map->capacity;
        factor = ((factor % __INT16_MAX__) * (31 % __INT16_MAX__)) % __INT16_MAX__;
    }

    index = sum;
    return index;
}

void insert(struct hash_map* map, char* key){
    int bucket_index = hash(map, key);
    if (map->arr[bucket_index] == NULL) {
        struct node* new_node = get_node(key, 1);
        map->arr[bucket_index] = new_node;
    }else {
        struct node*temp = map->arr[bucket_index];
        while(temp!=NULL && strcmp(temp->key,key)){
            temp = temp->next;
        }
        if(temp==NULL){
            struct node* new_node = get_node(key, 1);
            new_node->next = map->arr[bucket_index];
            map->arr[bucket_index] = new_node;
        }else{
            temp->value++;
        }
    }
    return;
}

int find(struct hash_map* map, char* key){
    int bucket_index = hash(map, key);
    struct node* head = map->arr[bucket_index];
    while (head != NULL) {
        if (!strcmp(head->key,key) && !(head->visited)) {
            head->visited = true;
            return head->value;
        }
        head = head->next;
    }
    return -1;
}

int main(int argc,char* argv[]){
    char* input;
    char* output = NULL;
    bool count = false,uniq = false,repeat = false;
    FILE *file;
    if(argc == 1||argc == 2 && !strcmp(argv[1], "-")){
        file = stdin;
    }else if(!strcmp(argv[argc-2], "-")&&!strncmp(argv[argc-1]+strlen(argv[argc-1])-4,".txt",4)){
        output = argv[argc-1];
        file = stdin;
    }else if(!strncmp(argv[argc-1]+strlen(argv[argc-1])-4,".txt",4) && !strncmp(argv[argc-2]+strlen(argv[argc-2])-4,".txt",4)){
        input = argv[argc-2];
        output = argv[argc-1];
        file = fopen(input,"r");
    }else if(!strncmp(argv[argc-1]+strlen(argv[argc-1])-4,".txt",4)){
        input = argv[argc-1];
        file = fopen(input,"r");
    }

    if(file == NULL){
        printf("uniq: %s No such file or directory",input);
        return 1;
    }
    char c;
    int i = 0,size=0;
    char* data = (char*)malloc(sizeof(char) * SIZE);
    int capacity = SIZE;

    while((c=fgetc(file))!=EOF){
        if(i >= capacity - 1){
            capacity *= 2;
            data = realloc(data, capacity);
        }
        if(c=='\n') size++;
        data[i] = c;
        i++;
    }
    size++;
    data[i] = '\0';

    struct hash_map* map = (struct hash_map*)malloc(sizeof(struct hash_map));
    map->capacity = size;
    map->num = 0;
    map->arr = (struct node**)malloc(sizeof(struct node*) * map->capacity);

    i=0;
    char* arr[size];
    char* token = strtok(data, "\n");
    while (token != NULL) {
        token = trim(token);
        if(token!="\0"){
            arr[i]=token;
        }
        i++;
        token = strtok(NULL, "\n");
    }

    for(int i=0;i<size;i++){
        insert(map, arr[i]);
    }

    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i],"-c")||!strcmp(argv[i],"--count")){
            count = true;
        }else if(!strcmp(argv[i],"-d")||!strcmp(argv[i],"--repeated")){
            repeat = true;
        }else if(!strcmp(argv[i],"-u")){
            uniq = true;
        }else if(!strcmp(argv[i],"-h")||!strcmp(argv[i],"--help")){
            printf("uniq [OPTION]... [INPUT] [OUTPUT]\n");
            printf("Filters matching lines from INPUT (or standard input),writing to OUTPUT (or standard output).\n");
            printf("OPTIONS:\n");
            printf("With no FILE, or when FILE is -, read standard input\n");
            printf("-c, --count show lines with number of occurences\n");
            printf("-d, --repeated Only print duplicate lines, one each\n");
            printf("-u Only print unique lines, one each\n");
            return 0;
        }
    }

    char flag[10];
    char* uniq_data = (char*)calloc(10,sizeof(char));
    int n = 10;
    for(int i=0;i<size;i++){
        int value = find(map,arr[i]);
        if(value == -1) continue;
        if(count){
            if(repeat){
                if(value>1){
                    if(output){
                        if(!uniq_data){
                            n = strlen(arr[i])+3;
                            uniq_data = realloc(uniq_data, n);
                            strcpy(uniq_data,arr[i]);
                            strcat(uniq_data,"\n");
                        }else{
                            n += strlen(arr[i])+3;
                            uniq_data = realloc(uniq_data, n);
                            strcat(uniq_data,arr[i]);
                            strcat(uniq_data,"\n");
                        }
                    }else{
                        printf("%d %s\n",value, arr[i]);
                    }
                }
            }else if(uniq){
                if(value==1){
                    if(output){
                        if(!uniq_data){
                            n = strlen(arr[i])+3;
                            uniq_data = realloc(uniq_data, n);
                            strcpy(uniq_data,arr[i]);
                            strcat(uniq_data,"\n");
                        }else{
                            n += strlen(arr[i])+3;
                            uniq_data = realloc(uniq_data, n);
                            strcat(uniq_data,arr[i]);
                            strcat(uniq_data,"\n");
                        }
                    }else{
                        printf("%d %s\n",value, arr[i]);
                    }
                }
            }else{
                if(output){
                    if(!uniq_data){
                        n = strlen(arr[i])+3;
                        uniq_data = realloc(uniq_data, n);
                        strcpy(uniq_data,arr[i]);
                        strcat(uniq_data,"\n");
                    }else{
                        n += strlen(arr[i])+3;
                        uniq_data = realloc(uniq_data, n);
                        strcat(uniq_data,arr[i]);
                        strcat(uniq_data,"\n");
                    }
                }else{
                    printf("%d %s\n",value, arr[i]);
                }
            }
        }else{
            if(repeat){
                if(value>1){
                    if(output){
                        if(!uniq_data){
                            n = strlen(arr[i])+3;
                            uniq_data = realloc(uniq_data, n);
                            strcpy(uniq_data,arr[i]);
                            strcat(uniq_data,"\n");
                        }else{
                            n += strlen(arr[i])+3;
                            uniq_data = realloc(uniq_data, n);
                            strcat(uniq_data,arr[i]);
                            strcat(uniq_data,"\n");
                        }
                    }else{
                        printf("%s\n", arr[i]);
                    }   
                }
            }else if(uniq){
                if(value==1){
                    if(output){
                        if(!uniq_data){
                            n = strlen(arr[i])+3;
                            uniq_data = realloc(uniq_data, n);
                            strcpy(uniq_data,arr[i]);
                            strcat(uniq_data,"\n");
                        }else{
                            n += strlen(arr[i])+3;
                            uniq_data = realloc(uniq_data, n);
                            strcat(uniq_data,arr[i]);
                            strcat(uniq_data,"\n");
                        }
                    }else{
                        printf("%s\n", arr[i]);
                    }
                }
            }else{
                if(output){
                    if(!uniq_data){
                        n = strlen(arr[i])+3;
                        uniq_data = realloc(uniq_data, n);
                        strcpy(uniq_data,arr[i]);
                        strcat(uniq_data,"\n");
                    }else{
                        n += strlen(arr[i])+3;
                        uniq_data = realloc(uniq_data, n);
                        strcat(uniq_data,arr[i]);
                        strcat(uniq_data,"\n");
                    }
                }else{
                    printf("%s\n", arr[i]);
                }
            }
        }
    }
    uniq_data[n]='\0';

    if(output){
        FILE* op;
        op = fopen(output, "w");
        fprintf(op,"%s",uniq_data);
        fclose(op);
    }
    fclose(file);
    return 0;
}