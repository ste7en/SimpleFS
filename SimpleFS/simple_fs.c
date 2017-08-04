#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Inizio Costanti
#define MAX_RES_NAME 255
#define MAX_TREE_HEIGHT 255
#define MAX_RES_NUM 1024
#define HASHTABLE_DIM 128
#define MAX_DATA_LENGHT 3000

// Fine Costanti


typedef struct node {
    
    char name[MAX_RES_NAME];
    char data[MAX_RES_NAME];
    bool is_file;
    struct node **resources;    //puntatore a risorse, se è un file è NULL, se è una dir è un vettore di puntatori
    int free_resources;
    struct node *next_brother;
    struct node *last_son;      //ultimo figlio aggiunto
    
    } node;

//Inizio prototipi funzioni {

int command_to_number(char *);

unsigned hash(char *);

node *Luke_NodeWalker(char *, char *);

void create(char*);

void create_dir(char*);

void initialize (node*, char[], char[]); //nome e dati della directory

// } Fine prototipi funzioni




// Variabili globali

node root;

// fine variabili globali




int main(){
    
    int arg=1;  //iniziallizzato a 1
    char command[10];
    char path[MAX_RES_NAME*MAX_TREE_HEIGHT];
    char *data;
    char *token;
    strcpy(root.name, "/");
    root.free_resources=MAX_RES_NUM;
    root.is_file=false;
    root.resources=NULL;
    
    while(arg>0){
        
        scanf("%s %s", command, path);
        data = strtok(path, " ");
        arg=command_to_number(command);
        //token=strtok(path, "/");
        
        switch(arg){
            case 0:
                break;
            case 1:
                //create
                
                create(path);
                break;
            case 2:
                //create_dir
                
               // Luke_NodeWalker(path);
                break;
            case 3:
                //read
                break;
            case 4:
                //write
                break;
            case 5:
                //delete
                break;
            case 6:
                //delete_r
                break;
            case 7:
                //find
                break;
            default:
                printf("no\n");
                
        }
        
        
        
    }
    
    
    
    return 0;
}




node *Luke_NodeWalker(char *path, char *son){
    
    unsigned hashvalue;
    node *pointer=&root;
    
    while(path!=son&&path!=NULL){
            
        hashvalue=hash(path);
        
        if (pointer->resources!=NULL){
            
            pointer=pointer->resources[hashvalue];
            
            while(strcmp(pointer->name, path)!=0 && pointer!=NULL){
                pointer=pointer->next_brother;  //scorro la lista dei figli con lo stesso hashvalue
            }
            
            path=strtok(NULL, "/");
            
        }
        
        
    }
    
    return pointer;
    
}








void create(char *path){
    
    unsigned hashval;
    int slash='/';
    char *name;
    node *father=&root;
    name = strrchr(path, slash);
    path= strtok(path, "/");
    
    
    if (path==NULL) {   // caso in cui si cerca di creare la directory root, preesistente
        printf("no");
    }
    
    else{
        
        if (name!=NULL){
        name++;
        father = Luke_NodeWalker(path, name);
        }
        
        if (father==NULL){
            printf("no\n");
        }
        
        else {      //esiste il nodo padre, posso procedere a creare il file
            
            if(!father->is_file){
                
                hashval=hash(name);
                
                if (father->resources==NULL) {      //il padre non ha ancora figli
                    
                    father->resources = (node**) (malloc(sizeof(node*)*HASHTABLE_DIM));
                    
                    for (int i=0; i<HASHTABLE_DIM; i++) {
                        father->resources[i]=NULL;
                    }
                    
                    father->resources[hashval] = (node*) malloc(sizeof(node));
                    
                    strcpy(father->resources[hashval]->name, name);
                    
                    father->resources[hashval]->is_file=true;
                    father->resources[hashval]->free_resources=0;
                    father->resources[hashval]->resources=NULL;
                    father->resources[hashval]->next_brother=NULL;
                    father->resources[hashval]->last_son=NULL;
                    father->last_son=father->resources[hashval];
                    father->free_resources--;
                    
                    printf("ok\n");
                }
                
                else{       //il padre ha già altri figli
                    
                    bool res_exists=false;     //valore booleano se è già presente il file nella directory padre
                    
                    if (father->free_resources!=0){
                        
                        node *temp=father->resources[hashval];
                        
                        while(temp!=NULL&&!res_exists){
                            
                            if(strcmp(father->resources[hashval]->name, name)==0){ //file o directory già esistente
                                printf("no\n");
                                res_exists=true;
                            }
                            else{
                                temp=temp->next_brother;
                            }
                        }
                        if(!res_exists){
                            temp =(node*) malloc(sizeof(node));
                            strcpy(temp->name, name);
                            temp->next_brother= father->resources[hashval];
                            temp->is_file=true;
                            temp->free_resources=0;
                            temp->resources=NULL;
                            temp->last_son=NULL;
                            father->resources[hashval]=temp;
                            father->last_son=temp;
                            father->free_resources--;
                            printf("ok\n");
                        }
                    }
                    
                }
                
            }
        }
    }
    
    

}




//void create_dir(char path[]){
//    node *curr_dir = &root;
//    int tree_height=0;
//    int res_num;
//    char sep[2]="/";
//    char *token;
//    int count=0;
//    char name[MAX_RES_NAME];
//    
//    
//    /* Segue:
//     Parte di codice in cui curr_dir avanza nel percorso
//     e 
//     tree_height++;
//     */
//        token = strtok(path, sep);
//        
//        while((curr_dir->resources[count]!=NULL)&&(token!=NULL)){
//            
//            if (curr_dir->resources[count]==NULL)
//                break; //non trova la directory
//            
//            else if (strcmp(curr_dir->resources[count]->d->name, token)){
//                tree_height++;
//                token=strtok(NULL, sep);
//                curr_dir=curr_dir->resources[count]->d;
//                count=0;
//            }
//            
//            else
//                count++;
//            
//        }
//    /*Controllo possibili errori*/
//    
//    if (token==NULL || (curr_dir->resources[count]!=NULL&&curr_dir->resources[count]==NULL) || tree_height>=MAX_TREE_HEIGHT) //cartella gia' esistente o non trova la directory o massima altezza nell'albero
//        printf("no");
//    
//    
//    else{   // ha superato il controllo degli errori
//        
//        res_num=MAX_RES_NUM-(curr_dir->free_resources); // POSSO SOSTITUIRLO CON COUNT
//        strcpy(name, token);
//        if ((token=strtok(NULL, sep))==NULL)
//        {
//            // l'ultima stringa in token e' effettivamente il nome
//            // della directory da creare
//            if (curr_dir->resources[count]==NULL) {    // la directory non ha risorse -> creo vettore di risorse
//                curr_dir->resources[count] = (union resource*) malloc(sizeof(union resource));
//            }
//            curr_dir->resources[res_num]->d = (node*) malloc(sizeof(node));
//            curr_dir->resources[res_num]->d->free_resources=MAX_RES_NUM;
//            strcpy(curr_dir->resources[res_num]->d->name, name);
//            curr_dir->free_resources--;
//            }
//        
//        else{
//            // directory non esistente
//            printf("no");
//        }
//        
//        
//    
//    }
//    
//}
//


                   

int command_to_number(char *s){
    if(strcmp(s, "exit")==0)
        return 0;
    if(strcmp(s, "create")==0)
        return 1;
    if(strcmp(s, "create_dir")==0)
        return 2;
    if(strcmp(s, "read")==0)
        return 3;
    if(strcmp(s, "write")==0)
        return 4;
    if(strcmp(s, "delete")==0)
        return 5;
    if(strcmp(s, "delete_r")==0)
        return 6;
    if(strcmp(s, "find")==0)
        return 7;
    else return -1;
}




unsigned hash(char *c){
    
    unsigned hash_val;
    for(hash_val=0; *c!='\0'; c++){
        hash_val= *c+31*hash_val; //funzione hash dal libro C programming language
    }
    return hash_val % HASHTABLE_DIM;
    
}



