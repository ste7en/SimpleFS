#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Inizio Costanti
#define MAX_RES_NAME 255
#define MAX_TREE_HEIGHT 255
#define MAX_RES_NUM 1024
// Fine Costanti


typedef struct {
    
    char name_dir[MAX_RES_NAME];
    union resource **resources; //puntatore a vettore di puntatori delle risorse
    int free_resources;
    
    
    } inode;

typedef struct {

    char name[MAX_RES_NAME];
    char data[];
    
    } file;

union resource{
    
    file* f;    //una directory può avere come risorse o un file o una directory
    inode* d;
    
    };

//Inizio prototipi funzioni {

int create(char);

void create_dir(char*);

void initialize (inode*, char[], char[]); //nome e dati dell'inode

// } Fine prototipi funzioni



inode root;

int main(){
    
    //root=(inode*)malloc(sizeof(inode));

    initialize(&root, "/", NULL);
    
    
    
    
    
    
    
    
    
    
    return 0;
}








void initialize(inode* p,char name[], char data[]){
    
    if (data==NULL){
        // e' una directory
        strcpy(p->name_dir, name);
        p->free_resources=MAX_RES_NUM;
        p->resources=NULL;
    }
    
    else{
        // è un file
        //strcpy(p->resources[MAX_RES_NUM-p->free_resources]->f->name, name);
        
    }
    
    }


void create_dir(char path[]){
    inode *curr_dir = &root;
    int tree_height=0;
    int res_num;
    char sep[2]="/";
    char *token;
    int count=0;
    
    /* Segue:
     Parte di codice in cui curr_dir avanza nel percorso
     e 
     tree_height++;
     */
        token = strtok(path, sep);
        
        while((curr_dir->resources!=NULL)&&(token!=NULL)){
            
            if (curr_dir->resources[count]==NULL)
                break; //non trova la directory
            
            else if (strcmp(curr_dir->resources[count]->d->name_dir, token)){
                tree_height++;
                token=strtok(NULL, sep);
                curr_dir=curr_dir->resources[count]->d;
                count=0;
            }
            
            else
                count++;
            
        }
    /*Controllo possibili errori*/
    
    if (token==NULL || curr_dir->resources[count]==NULL || tree_height>=MAX_TREE_HEIGHT) //cartella gia' esistente o non trova la directory o massima altezza nell'albero
        printf("no");
    
    
    else{
        
        res_num=MAX_RES_NUM-(curr_dir->free_resources);
    /*
     Parte di codice in cui alloco una struct inode per la nuova directory
     
     curr_dir.resources[res_num]= allocazione dinamica;
     curr_dir.resources[res_num].free_resources=MAX_RES_NUM;
     
     */
        
        curr_dir->free_resources--;
    
    }
    
}










