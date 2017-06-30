#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Inizio Costanti
#define MAX_RES_NAME 255
#define MAX_TREE_HEIGHT 255
#define MAX_RES_NUM 255
// Fine Costanti


typedef struct {
    
    char name_dir[MAX_RES_NAME];
    struct dentry *resources[MAX_RES_NUM];
    int free_resources;
    
    } dentry;

typedef struct {

    char name[MAX_RES_NAME];
    char data[];
    
    } file;



// Variabili globali:
dentry root;


//Inizio prototipi funzioni {

int create(char);

void create_dir(char*);

// } Fine prototipi funzioni

int main(){
    
    root.name_dir[0]='/';
    root.free_resources=MAX_RES_NUM;
    
    
    return 0;
}


void create_dir(char path[]){
    dentry curr_dir = root;
    int tree_height=0;
    int res_num;
    
    /*
     Parte di codice in cui curr_dir avanza nel percorso
     e 
     tree_height++;
     */
    
    /*Controllo possibili errori*/
    
    if (tree_height>=MAX_TREE_HEIGHT)
        printf("no");
    
    else{
        
        res_num=MAX_RES_NUM-curr_dir.free_resources;
    /*
     Parte di codice in cui alloco una struct dentry per la nuova directory
     
     curr_dir.resources[res_num]= allocazione dinamica;
     curr_dir.resources[res_num].free_resources=MAX_RES_NUM;
     
     */
        
        curr_dir.free_resources--;
    
    }
    
}










