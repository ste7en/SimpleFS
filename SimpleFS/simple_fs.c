#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 Progetto di Algoritmi e Principi dell'informatica
 di Stefano Formicola
 */

// Inizio Costanti
#define MAX_RES_NAME 256            //ovvero 255 più il carattere terminatore della stringa
#define MAX_TREE_HEIGHT 255         //altezza massima dell'albero
#define MAX_RES_NUM 1024            //massimo numero di risorse per nodo (directory)
#define HASHTABLE_DIM 128           //dimensioni della tabella hash di ogni directory - l'hashing è risolto con concatenazione
#define MAX_PATH_LENGHT (255*256)+1 //tiene conto della presenza di 255 simboli '/' più il terminatore
#define BLOCK_SIZE 512              //dimensioni di default per un blocco nella write o array di puntatori nella find
// Fine Costanti


typedef struct node{
    
    char name[MAX_RES_NAME];    //nome del file o della directory
    char *data;                 //puntatore ai dati di un file, stringa - NULL per cartelle
    bool is_file;               //valore booleano per distinguere file da directory
    struct node **resources;    //puntatore a risorse, se è un file è NULL, se è una dir è un vettore di puntatori
    int free_resources;         //risorse libere della directory, NULL per i file
    struct node *next_brother;  //fratello con lo stesso valore hash
    int height;                 //alteza della directory o risorsa nell'albero
    
} node;

typedef struct pointers{
    node *father;               //puntatore al padre di un nodo son
    node *son;                  //puntatore a un nodo
} pointers;


// Inizio prototipi funzioni {

int command_to_number(char *);

unsigned hash(char *);

node *Luke_NodeWalker(char *, char *);

pointers getResourcePointers(char *);

void create(char *);

void create_dir(char *);

void res_write(char *);

void res_read(char *);

void res_delete(char *);

void delete_r(char *);

void res_delete_r(node *);

void find(node *, char *, char *, unsigned);

int addToFoundPaths(char *);

char *concatenate(char *, char *);

int compare(const void *, const void *);
// Fine prototipi funzioni }




// Variabili globali

node root;
int slash='/';
char **found_paths=NULL;
// fine variabili globali



int main(){
    
    int arg=10;
    char command[15];
    char string[MAX_PATH_LENGHT];
    
    strcpy(root.name, "/");
    root.free_resources=MAX_RES_NUM;
    root.is_file=false;
    root.data=NULL;
    root.next_brother=NULL;
    root.resources=NULL;
    root.height=1;
    
    
    while(arg>0){
        
        scanf("%s", command); //acquisisce il comando
        
        if (strcmp(command, "exit")==0){
            return 0;
        }
        
        scanf("%*c");  //acquisisce il carattere spazio
        
        scanf("%s", string);
        
        arg=command_to_number(command);
        
        
        switch(arg){
            case 1:
                //create
                
                create(string);
                
                break;
                
            case 2:
                //create_dir
                
                create_dir(string);
                
                break;
                
            case 3:
                //read
                
                res_read(string);
                
                break;
                
            case 4:
                //write

                res_write(string);
                
                break;
                
            case 5:
                //delete
                
                res_delete(string);
                
                break;
                
            case 6:
                //delete_r
                
                delete_r(string);
                
                break;
                
            case 7:
                //find
                
                find(&root, string, strdup("/"), hash(string));
                
                break;
                
            default:
                printf("no\n");
                
        }
        
        
        
    }
    
    
    
}



void find(node *father, char *name, char *path, unsigned hashvalue){
    
    /*
     La find parte da root, nome file/dir, path, valore hash della risorsa. Visita in profondità ogni figlio e 
     concatena i vari nomi di percorso
     */
    
    static int count=0;
    node *pointer=NULL;
    char *temp=NULL;
    
    if(strcmp(path, "/")!=0){
        strcat(path, "/");
        //da aggiungere per ogni avanzamento in profondità nell'albero del fs, tranne nel caso di livello 1 (root)
    }
    
    if(father->is_file==false&&father->resources!=NULL){
        
        for (int i=0; i<HASHTABLE_DIM; i++) {
            
            pointer=father->resources[i];
            
            while(pointer!=NULL){
                if (pointer->is_file==false) {
                    find(pointer, name, concatenate(path, pointer->name), hashvalue);
                }
                pointer=pointer->next_brother;
            }
            
        }
        
        pointer=father->resources[hashvalue];
    }
    
    while(pointer!=NULL&&strcmp(pointer->name, name)!=0){
        
        pointer=pointer->next_brother;
    }
    
    if(pointer!=NULL){
        // risorsa trovata
        temp = concatenate(path, name);
        count = addToFoundPaths(temp);
    }
    
    if(path!=NULL){
        free(path);
    }
    if(temp!=NULL){
        free(temp);
    }
    
    if(father==&root){
        
        if(found_paths!=NULL){
            
            qsort(found_paths, count, sizeof(char*), compare);
            
            for (int j=0; j<count; j++) {
                printf("ok %s\n", found_paths[j]);
                free(found_paths[j]);
                found_paths[j]=NULL;
            }
            
            free(found_paths);
            found_paths=NULL;
            count=0;
            
        }
        else{
            printf("no\n");
        }
    }
    
    
}


int compare(const void *p1, const void *p2){
    
    char **s1=(char**)p1;
    char **s2=(char**)p2;
    
    return (strcmp(*s1, *s2));
}




char *concatenate(char *a, char *b){
    
    char *c=NULL;
    
    if(a!=NULL&&b!=NULL){
        while(c==NULL){
            c=calloc(strlen(a)+strlen(b)+2, sizeof(char));
        }
        strcpy(c, a);
        strcat(c, b);
    }
    return c;
}


int addToFoundPaths(char *string){
    
    static int count=0;
    
    while(found_paths==NULL){
        count=0;
        found_paths=calloc(BLOCK_SIZE, sizeof(char*));
    }
    
    if((count%BLOCK_SIZE)==0&&count!=0){
        
        char **temp=NULL;
        while(temp==NULL){
            temp=realloc(found_paths, BLOCK_SIZE*sizeof(char*));
        }
        found_paths=temp;
    }
    
    while(found_paths[count]==NULL){
        found_paths[count]=calloc(MAX_PATH_LENGHT, sizeof(char));
    }
    
    strcpy(found_paths[count], string);
    
    count++;
    
    return count;
}


void create(char *path){
    
    char *filename =  strrchr(path, slash);
    node *temp=NULL;
    pointers pointers;
    
    
    if (strcmp(path, "/")==0||strcmp(path, "\0")==0||filename==NULL) {   //errore in input oppure si cerca di creare la root
        printf("no\n");
    }
    
    else{
        
        pointers=getResourcePointers(path);
        
        filename++;
        
        if (pointers.father==NULL){ //precedentemente anche filename==NULL
            printf("no\n");
        }
        
        else {      //esiste il nodo padre, posso procedere a creare il file
            
            if(strlen(filename)<MAX_RES_NAME) {
                
                if(pointers.father->is_file==false&&(pointers.father->height<MAX_TREE_HEIGHT)){
                    
                    unsigned hashvalue=hash(filename);
                    
                    if (pointers.father->resources==NULL) {      //il padre non ha ancora figli, creo la tabella hash di puntatori ai figli
                        
                        pointers.father->resources = (node**) (calloc(HASHTABLE_DIM, sizeof(node*)));
                        //tabella hash dei figli creata e iniziallizzata a NULL
                        
                        temp = (node*)malloc(sizeof(node));
                        pointers.father->resources[hashvalue]=temp;
                        strcpy(temp->name, filename);
                        temp->is_file=true;
                        temp->free_resources=0;
                        temp->resources=NULL;
                        temp->next_brother=NULL;
                        temp->data=NULL;
                        temp->height=(pointers.father->height)+1;
                        pointers.father->free_resources--;
                        
                        printf("ok\n");
                    }
                    
                    else{       //il padre ha già altri figli
                        
                        if(pointers.son==NULL){ // la cartella non esiste, procedo a crearla
                            
                            if(pointers.father->free_resources!=0){
                                temp=(node*) malloc(sizeof(node));
                                strcpy(temp->name, filename);
                                temp->next_brother= pointers.father->resources[hashvalue];
                                temp->is_file=true;
                                temp->data=NULL;
                                temp->free_resources=0;
                                temp->resources=NULL;
                                temp->height=(pointers.father->height)+1;
                                pointers.father->resources[hashvalue]=temp;
                                pointers.father->free_resources--;
                                printf("ok\n");
                            }
                            else{
                                printf("no\n"); //eccede i limiti del filesystem (MAX_RES_NUM)
                            }

                        }
                        else{
                            printf("no\n"); //la cartella già esiste
                        }
                    }
                }
                else{
                    printf("no\n"); //si tenta di scrivere su un file o si eccedono i limiti del fs

                }
            }
            else{   //si eccedono i limiti del fs
                printf("no\n");
            }
        }
    }
}


void create_dir(char *path){
    
    char *name=strrchr(path, slash);
    node *temp;
    pointers pointers;
    
    if (strcmp(path, "/")==0||strcmp(path, "\0")==0||name==NULL){		//errore in input oppure si cerca di creare la root
        printf("no\n");
    }
    
    else{
        
        pointers=getResourcePointers(path);
        
        name++;
        
        if(pointers.father==NULL){  //percorso non trovato
            printf("no\n");
        }
        
        else{
            
            if(strlen(name)<MAX_RES_NAME){
                
                if(pointers.father->is_file==false&&(pointers.father->height<MAX_TREE_HEIGHT)){
                    
                    unsigned hashvalue=hash(name);
                    
                    if(pointers.father->resources==NULL){    //il padre non ha ancora figli, creo la tabella hash di puntatori ai figli
                        
                        pointers.father->resources = (node**) (calloc(HASHTABLE_DIM, sizeof(node*)));
                        // tabella hash ai figli creata e iniziallizzata a NULL
                        
                        pointers.father->resources[hashvalue]= (node *) malloc(sizeof(node));
                        strcpy(pointers.father->resources[hashvalue]->name,name);
                        pointers.father->resources[hashvalue]->is_file=false;
                        pointers.father->resources[hashvalue]->data=NULL;
                        pointers.father->resources[hashvalue]->free_resources=MAX_RES_NUM;
                        pointers.father->resources[hashvalue]->resources=NULL;
                        pointers.father->resources[hashvalue]->next_brother=NULL;
                        pointers.father->resources[hashvalue]->height=pointers.father->height +1;
                        pointers.father->free_resources--;
                        
                        printf("ok\n");
                    }
                    
                    else{  //il padre ha già altri figli
                        
                        if(pointers.son==NULL){  // la cartella non esiste, procedo a crearla
                            if(pointers.father->free_resources!=0){
                                temp= (node*) malloc(sizeof(node));
                                strcpy(temp->name,name);
                                temp->is_file=false;
                                temp->data=NULL;
                                temp->free_resources=MAX_RES_NUM;
                                temp->resources=NULL;
                                temp->height=pointers.father->height +1;
                                temp->next_brother=pointers.father->resources[hashvalue];
                                pointers.father->resources[hashvalue]=temp;
                                pointers.father->free_resources--;
                                printf("ok\n");
                            }
                            else{
                                printf("no\n");  // la cartella ha raggiunto il limite di MAX_RES_NUM figli
                            }
                        }
                        else{   //la cartella già esiste
                            printf("no\n");
                            
                        }
                    }
                }
                else{   //si tenta di scrivere su un file o si eccedono i limiti del fs
                    printf("no\n");
                }
            }
            else{   //si eccedono i limiti del fs
                printf("no\n");
            }
        }
    }
}

                        
                        


void res_write(char *path){

    char *data=NULL;
    void *temp=NULL;
    int count=-1;

    pointers pointers=getResourcePointers(path);
    
    //Allocazione dinamica della stringa da memorizzare nel file, count è il contatore del vettore di caratteri che contiene la stringa
    
    while(data==NULL){
        data=(char*)malloc(sizeof(char)*BLOCK_SIZE);
    }
    
    scanf("%*c");  //acquisisce il carattere spazio
    scanf("%*c"); //acquisisce virgoletta
    
    do{
        count++;
        if ((count%BLOCK_SIZE)==0&&(count!=0)) {
            while(temp==NULL){
                temp=realloc(data, sizeof(char)*BLOCK_SIZE);
            }
            data=(char*)temp;
        }
        scanf("%c", &data[count]);
    }
    while(data[count]!='"');
    
    data[count]='\0'; //elimina la seconda virgoletta
    
    if(pointers.son!=NULL){ //il file da scrivere esiste
        
        if(pointers.son->is_file==true){
            if(pointers.son->data!=NULL){
                //sovrascrivo il file
                free(pointers.son->data);
                pointers.son->data=NULL;
            }
            pointers.son->data = strdup(data);
            printf("ok %lu\n", strlen(pointers.son->data));
        }
        else{
            // non è un file, non scrivo
            printf("no\n");
        }
        
    }
    else{
        //path non esistente
        printf("no\n");
    }
    free(data);
    
}




void res_read(char *path){
    
    pointers pointers=getResourcePointers(path);
    
    if(pointers.son!=NULL){
        // la risorsa esiste
        if(pointers.son->is_file==true){
            // ed è un file
            if(pointers.son->data!=NULL){
                printf("contenuto %s\n", pointers.son->data);
            }
            else{
                printf("contenuto \n"); //non è stata eseguita una write
            }
        }
        else{
            // non è un file
            printf("no\n");
        }
    }
    else{
        //file non trovato
        printf("no\n");
    }
    
}




void res_delete(char *path){

  //resource delete

  char *name=strrchr(path, slash);
  unsigned hashvalue=0;

  node *father=NULL;
  node *temp;

    if(name!=NULL){
        name++;
        hashvalue=hash(name);
        father=Luke_NodeWalker(path, name);
        
    }
    
  if(father!=NULL){

    if(father->resources!=NULL){
      node *last_brother=NULL;  
      temp=father->resources[hashvalue];
      while(temp!=NULL&&strcmp(temp->name, name)!=0){
        last_brother=temp;
        temp=temp->next_brother;
      }
      if(temp!=NULL&&(temp->resources==NULL)){ //esiste la risorsa e non ha figli

        if(last_brother!=NULL){ //nodo all'interno della lista
          last_brother->next_brother=temp->next_brother;
        }
        else{ //nodo in testa alla lista
          father->resources[hashvalue]=temp->next_brother;
        }
        
        if(temp->data!=NULL){ //cancello i dati scritti sul file
          free(temp->data);
        }
        free(temp); //cancello la risorsa

        father->free_resources++;

        if(father->free_resources==MAX_RES_NUM){  //rendo la directory padre cancellabile se non ha più figli
          free(father->resources);
          father->resources=NULL;
        }

        printf("ok\n");

      }
      else{
        printf("no\n"); //la risorsa non esiste oppure ha figli
      }
    }
    else{
      printf("no\n"); //file inesistente
    }

  }
  else{
    printf("no\n"); //path non trovato
  }

}


void delete_r(char *path){
    
    char *name=strrchr(path, slash);
    unsigned hashvalue=0;
    
    node *father=NULL;
    node *resource;
    
    
    if(name!=NULL){
        name++;
        hashvalue=hash(name);
        father=Luke_NodeWalker(path, name);
        
    }
    
    if(father!=NULL){
        
        if(father->resources!=NULL){
            resource=father->resources[hashvalue];
            node *last_brother=NULL;

            while(resource!=NULL&&strcmp(resource->name, name)!=0){
                //scorro la lista dei figli con uguale hashvalue per cercare la risorsa da cancellare
                last_brother=resource;
                resource=resource->next_brother;
            }
            
            if(resource!=NULL){
                //esiste la risorsa da cancellare
                
                if(last_brother==NULL){
                    //la risorsa da cancellare è in testa alla lista
                    father->resources[hashvalue]=resource->next_brother;
                    res_delete_r(resource);
                    father->free_resources++;
                }
                
                else{
                 //la risorsa da cancellare non è in testa alla lista
                    last_brother->next_brother=resource->next_brother;
                    father->free_resources++;
                    res_delete_r(resource);
                }
                
                if(father->free_resources==MAX_RES_NUM){  //rendo la directory padre cancellabile se non ha più figli
                    free(father->resources);
                    father->resources=NULL;
                }
                
                printf("ok\n");
            }
            else{
                printf("no\n");
                //non esiste la risorsa da cancellare
            }
        }
        else{
            printf("no\n");
            //path inesistente: esiste il padre ma non ha risorse
        }
    }
    else{
        printf("no\n");
        //path inesistente
    }
    
}

void res_delete_r(node *resource){
    
    /*
     Dato il puntatore al nodo da cancellare con delete ricorsiva, se questo è una directory con figli viene chiamata la res_delete_r
     sui figli, scorrendo l'hashtable dei figli. Se il puntatore è o un file o una directory senza figli, con puntatore all'hashtable nullo,
     allora viene fatta la free dei dati e della risorsa.
     */
    node *next=NULL;
    
    if(resource!=NULL){
     
        if(resource->resources!=NULL){  // è una directory con figli
            
            for (int res_count=0; res_count<HASHTABLE_DIM; res_count++) {
                
                if(resource->resources[res_count]!=NULL){
                    
                    do{
                        next=resource->resources[res_count]->next_brother;
                        res_delete_r(resource->resources[res_count]);
                        resource->resources[res_count]=next;
                    }while (next!=NULL);
                    
                }
            }
            
            free(resource->resources);
            
        }
        
        if(resource->data!=NULL){
            free(resource->data);
        }
        
        free(resource);
        
    }
    
}

node *Luke_NodeWalker(char *path, char *son){
    
    /*
     Parametri passati alla funzione: stringa percorso della risorsa e nome della risorsa.
     Partendo da root tramite strtok divido la stringa del percorso in sottostringhe delimitate da '/'
     e mi servo di queste per muovermi all'interno del filesystem. Il token è un puntatore alla sottostringa, strtok
     modifica la stringa originale aggiungendo il terminatore '/0' al posto del '/' consentendomi di usare le sottostringhe
     senza allocare memoria.
     Quando token e son sono uguali, ovvero puntano alla stessa porzione di memoria del primo carattere del nome della risorsa,
     il ciclo while si ferma poichè sono arrivato alla directory padre.
     */
    
    unsigned hashvalue;
    node *pointer=&root;
    char *token=strtok(path, "/");
    
    while(token!=NULL&&token!=son&&pointer!=NULL){

        hashvalue = hash(token);
        
        if (pointer->resources!=NULL){
            pointer = pointer->resources[hashvalue];
            
            while(pointer!=NULL&&strcmp(pointer->name, token)!=0){
                pointer = pointer->next_brother;    //se pointer==NULL la risorsa non è stata trovata, termina il ciclo
            }
            
            token=strtok(NULL, "/");
        }
        
        else{
            return NULL;  //pointer->resources == NULL, percorso non trovato
        }
        
    }
    
    return pointer;   //esco dal ciclo while quando arrivo al padre della risorsa su cui operare
    
}


pointers getResourcePointers(char *path){
    
    /*
     Dato path e nome file o directory cerco la risorsa file o directory e restituisco i puntatori a padre e figlio.
     
     Se il puntatore al padre è diverso da NULL e ha risorse allora procedo a cercare il nodo figlio. Se, alla fine della
     coda di risorse con lo stesso hashvalue, pointer è NULL restituisco son=NULL, altrimenti se il confronto con il nome
     dà esito positivo restituisco il puntatore alla risorsa.
     
     */
    
    pointers toReturn;
    toReturn.father=NULL;
    toReturn.son=NULL;
    
    unsigned hashvalue=0;
    char *name=strrchr(path, slash); //punta all'ultimo simbolo di slash
    
    if(name!=NULL){
        name++; //punta al nome della risorsa
        hashvalue=hash(name);
        toReturn.father=Luke_NodeWalker(path, name);
    }
    
    if(toReturn.father!=NULL){
        if (toReturn.father->resources!=NULL) {
            toReturn.son=toReturn.father->resources[hashvalue];
            while(toReturn.son!=NULL&&(strcmp(toReturn.son->name, name))!=0){
                toReturn.son=toReturn.son->next_brother;
            }
        }
    }
    
    return toReturn;
}


int command_to_number(char *s){
    
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
    else return 8;
}


unsigned hash(char *c){
    
    /*Funzione hash per codificare il nome del file o della directory nell'array di puntatori
     ai nodi figli delle risorse. La funzione, ispirata a quella presente sul libro C programming language,
     tratta ogni carattere della stringa come numero con la codifica ASCII, lo addiziona a hash_val moltiplicato
     per il numero primo 31 e scala il valore alla dimensione dell'hashtable con l'operazione di modulo.
     */
    
    unsigned hash_val=0;
    
    if(c!=NULL){
        for(hash_val=0; *c!='\0'; c++){
            hash_val= (*c+31*hash_val)%HASHTABLE_DIM;
        }
    }
    return hash_val % HASHTABLE_DIM;
    
}
