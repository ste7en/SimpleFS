#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Inizio Costanti
#define MAX_RES_NAME 256  //ovvero 255 più il carattere terminatore della stringa
#define MAX_TREE_HEIGHT 255
#define MAX_RES_NUM 1024
#define HASHTABLE_DIM 128

// Fine Costanti


typedef struct node {
    
    char name[MAX_RES_NAME];    //nome del file o della directory
    char *data;                 //puntatore ai dati di un file, stringa - NULL per cartelle
    bool is_file;               //valore booleano per distinguere file da directory
    struct node **resources;    //puntatore a risorse, se è un file è NULL, se è una dir è un vettore di puntatori
    int free_resources;         //risorse libere della directory, NULL per i file
    struct node *next_brother;  //fratello con lo stesso valore hash
    int height;                 //alteza della directory o risorsa nell'albero
    
    } node;

//Inizio prototipi funzioni {

int command_to_number(char *);

unsigned hash(char *);

node *Luke_NodeWalker(char *, char *);

node *getResourcePointer(char *);

void create(char *);

void create_dir(char *);

void res_write(char *, char *);

void res_read(char *);

void res_delete(char *);

void res_delete_r(char *);

void find(node *, char*, char *, char**, int);      //parte da root, passo stringa path, nome file/dir, vettore di stringhe, intero contatore

// } Fine prototipi funzioni




// Variabili globali

node root;
int slash='/';
const char space[]=" ";

// fine variabili globali

/*
  DA SISTEMARE

  - THREE HEIGHT <––
  - FIND
  - WRITE <––
  - READ <––
  - DELETE <––
  - DELETE_R
  - controllare conflitti hash e strcmp <––
  - cancellare last son che non serve a niente <––
*/


int main(){
    
    int arg=10;  //valore di iniziallizzazione a 10
    char command[15];
    char *string=NULL;
    char *data=NULL;
    int count=-1;


    strcpy(root.name, "/");
    root.free_resources=MAX_RES_NUM;
    root.is_file=false;
    root.resources=NULL;
    root.height=1;


    while(arg>0){

    	if(string!=NULL){
        free(string);
        string=NULL;
    	}

      if(data!=NULL){
        free(data);
        data=NULL;
      }
      
    	scanf("%s", command); //acquisisce il comando
    	
      if (strcmp(command, "exit")==0){
    		exit(0);
    	}
      
      getchar();  //acquisisce il carattere spazio

      /*
      Segue l'allocazione dinamica di una stringa che conterrà o il percorso del file/directory
      o l'argomento della funzione find. L'allocazione si ferma appena incontra uno spazio o un simbolo di newline,
      lasciando nello stdin il resto dell'input nel caso, ad esempio, di write.
      */
      count=-1;
      do{
        count++;
        string=(char*)realloc(string, sizeof(char));
        scanf("%c", &string[count]);
      }
      while(string[count]!=' '&&string[count]!='\n');

      string[count]='\0';


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
                  
              
              //Allocazione dinamica della stringa da memorizzare nel file, count è il contatore del vettore di caratteri che contiene la stringa
              
              count=-1;

              scanf("%*c"); //acquisisce virgoletta
              
              do{
                count++;
                data=(char*)realloc(data, sizeof(char));
                scanf("%c", &data[count]);
              }
              while(data[count]!='"');
              
              data[count]='\0';

              res_write(string, data);

              break;
              
          case 5:
              //delete

              res_delete(string);
              
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
    
    
    
}

node *Luke_NodeWalker(char *path, char *son){
    
    unsigned hashvalue;
    node *pointer=&root;
    char *token=strtok(path, "/");


    while(token!=son&&token!=NULL&&pointer!=NULL){     //aggiungo anche path!=NULL ?? perchè?? prima c'era -- AGGIUNTO POINTER!=NULL
     // oppure strcmp(token, son)!=0
        hashvalue = hash(token);

        if (pointer->resources!=NULL){
            pointer = pointer->resources[hashvalue];

            while(pointer!=NULL&&strcmp(pointer->name, token)!=0){
                pointer = pointer->next_brother; 
            }

            token=strtok(NULL, "/");
            
        }

        else{
          return NULL;  //pointer->resources == NULL, path non presente
        }
        
    }

    return pointer;   //esco dal ciclo while quando arrivo al padre della risorsa su cui operare

}





void create(char *path){
    
  unsigned hashvalue;
  char *name;
  node *father=NULL;
  name = strrchr(path, slash);
  //path = strtok(path, "/");
  node *temp;
  
  if (path==NULL) {   // caso in cui si cerca di creare la directory root, preesistente
      printf("no\n");
  }
  
  else{
      
      if (name!=NULL){
          name++;
          father = Luke_NodeWalker(path, name);
      }
      
      if (father==NULL||name==NULL){
          printf("no\n");
      }
      
      else {      //esiste il nodo padre, posso procedere a creare il file
          
          if(strlen(name)<MAX_RES_NAME) {

            if(!(father->is_file)&&(father->height<MAX_TREE_HEIGHT)){
                        
                        hashvalue=hash(name);
                        
                        if (father->resources==NULL) {      //il padre non ha ancora figli, creo la tabella hash di puntatori ai figli
                            
                            father->resources = (node**) (calloc(HASHTABLE_DIM, sizeof(node*)));
                    //tabella hash dei figli creata e iniziallizzata a NULL
                            
                            temp = (node*) malloc(sizeof(node));
                            father->resources[hashvalue]=temp;
                            strcpy(temp->name, name);
                            
                            temp->is_file=true;
                            temp->free_resources=0;
                            temp->resources=NULL;
                            temp->next_brother=NULL;
                            temp->data=NULL;
                            temp->height=(father->height)+1;
                            father->free_resources--;
                            
                            printf("ok\n");
                        }
                        
                        else{       //il padre ha già altri figli
                            
                            bool res_exists=false;     //valore booleano se è già presente il file nella directory padre
                            
                            if (father->free_resources!=0){
                                
                                node *temp=father->resources[hashvalue];
                                
                                while(temp!=NULL&&!res_exists){  //perchè se temp==NULL allora o non ci sono figli con lo stesso hashvalue o arrivo alla fine della coda
                                    
                                    if(strcmp(father->resources[hashvalue]->name, name)==0){ //file o directory già esistente
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
                                    temp->next_brother= father->resources[hashvalue];
                                    temp->is_file=true;
                                    temp->data=NULL;
                                    temp->free_resources=0;
                                    temp->resources=NULL;
                                    temp->height=(father->height)+1;
                                    father->resources[hashvalue]=temp;
                                    father->free_resources--;
                                    printf("ok\n");
                                }
                            }
                        }
                  
              }
            else{
                printf("no\n"); //si cerca di scrivere in un file o eccede i limiti del filesystem
            }
          }
          else{
            printf("no\n"); //lunghezza del file eccede i limiti del filesystem
          }
      }
  }

}



void create_dir(char *path){

  unsigned hashvalue;
  char *name;
  node *father=&root;
  name = strrchr(path, slash);
  //path= strtok(path, "/");

  if (path==NULL){		//impossibile creare la root, che esiste già
  	printf("no\n");
  }

 	else{

 		if(name!=NULL){
 			name++;
 			father= Luke_NodeWalker(path, name);
 		}

 		if(father==NULL){
 			printf("no\n");
 		}

 		else{

   			if(strlen(name)<MAX_RES_NAME){

          if(!(father->is_file)&&(father->height<MAX_TREE_HEIGHT)){

                   hashvalue=hash(name);
        
                   if(father->resources==NULL){  //il padre non ha ancora figli, creo la tabella hash di puntatori ai figli
                     father->resources = (node**) (calloc(HASHTABLE_DIM ,sizeof(node*)));
                            // tabella hash ai figli creata e iniziallizzata a NULL 
        
                            father->resources[hashvalue]= (node *) malloc(sizeof(node));
        
                            strcpy(father->resources[hashvalue]->name, name);
        
                            father->resources[hashvalue]->is_file=false;
                            father->resources[hashvalue]->free_resources=MAX_RES_NUM;
                            father->resources[hashvalue]->resources=NULL;
                            father->resources[hashvalue]->next_brother=NULL;
                            father->resources[hashvalue]->height=father->height +1;
                            father->free_resources--;
        
                            printf("ok\n");
                   }
        
                   else{  //il padre ha già altri figli
                     
                    bool res_exists=false;  //valore booleano, vero se è già presente il file nella directory padre
        
                     if(father->free_resources!=0){
        
                       node *temp=father->resources[hashvalue];
        
                       while(temp!=NULL&&!res_exists){
                         //perchè se temp==NULL allora o non ci sono figli con lo stesso hashvalue o arrivo alla fine della coda
        
                         if(strcmp(father->resources[hashvalue]->name, name)==0){
                           //già esistente
                           printf("no\n");
                           res_exists=true;
                         }
                         else{
                           temp=temp->next_brother;
                         }
                       }
        
                      if(!res_exists){
                          temp= (node*) malloc(sizeof(node));
                          strcpy(temp->name, name);
                          temp->next_brother=father->resources[hashvalue];
                          temp->is_file=false;
                          temp->free_resources=MAX_RES_NUM;
                          temp->resources=NULL;
                          temp->height=father->height +1;
                          father->resources[hashvalue]=temp;
                          father->free_resources--;
                          printf("ok\n");
          
                       }       
        
        
                     }       
        
                   }        
        
        
                 }
               }
               else{
                printf("no\n"); //lunghezza del nome eccede i limiti del filesystem
               }
 		}


 	}


}


void res_write(char *path, char *data){

  char *name=strrchr(path, slash);

  name++;
  node *father=Luke_NodeWalker(path, name);
  unsigned hashvalue_son=hash(name);
  unsigned long data_lenght=strlen(data);

  /*
  Se il puntatore al padre è diverso da NULL e ha risorse allora procedo a cercare il nodo file, se alla fine della
  coda temp non punta al nodo su cui devo scrivere, la risorsa non esiste. Altrimenti se esiste ed è un file vado avanti
  e scrivo.
  */

  if(father!=NULL){

    if (father->resources!=NULL){
      
      node *temp = father->resources[hashvalue_son];

      while(temp!=NULL&&(strcmp(temp->name, name)!=0)){
        temp=temp->next_brother;
      }

      if (temp==NULL)
      {
        printf("no\n"); //non esiste il file
      }

      else{

        if(temp->is_file==true){
          
          if(temp->data!=NULL){ //sovrascrive il file
            free(temp->data);
            temp->data=NULL;
          }

          temp->data = (char*)calloc(data_lenght+1, sizeof(char));
          strcpy((temp->data), data);
          
          printf("ok %lu\n", data_lenght);
          
        }
        
        else{
          printf("no\n"); //tentativo di scrivere dati in una directory
        }
      }
    }

    else{
      printf("no\n"); //path non esistente, esiste il padre ma non ha figli
    }

  }

  else{
    printf("no\n"); //path non esistente
  }

}




void res_read(char *path){

    node *resource=getResourcePointer(path);
    
    if(resource!=NULL){
        // la risorsa esiste
        if(resource->is_file){
            // ed è un file
            if(resource->data!=NULL){
                printf("contenuto %s\n", resource->data);
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
  name++;
  unsigned hashvalue=hash(name);

  node *father=Luke_NodeWalker(path, name);
  node *temp;
  node *last_brother=NULL;

  if(father!=NULL){

    if(father->resources!=NULL){
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
    name++;
    unsigned hashvalue=hash(name);
    node *father=Luke_NodeWalker(path, name);
    node *resource;
    
    if(father!=NULL){
        
        if(father->resources!=NULL){

            resource=father->resources[hashvalue];
            while(resource!=NULL&&(strcmp(resource->name, name)!=0)){
                resource=resource->next_brother;
            }
            
            if (resource==NULL) {
                printf("no\n");
            }
            
            
        }

        
    }
    else{
        printf("no\n");
    }
    
    
    
}

node *getResourcePointer(char *path){
    
    char *name=strrchr(path, slash);
    name++;
    unsigned hashvalue=hash(name);
    node *father=Luke_NodeWalker(path, name);
    
    node *pointer=NULL;
    
    if(father!=NULL){
        if (father->resources!=NULL) {
            pointer=father->resources[hashvalue];
            while(pointer!=NULL&&(strcmp(pointer->name, name))!=0){
                pointer=pointer->next_brother;
            }
        }
    }
    else{   //path non trovato
        return NULL;
    }
    
    return pointer;
}


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
    else return 8;
}


unsigned hash(char *c){
    
    unsigned hash_val;
    for(hash_val=0; *c!='\0'; c++){
        hash_val= (*c+31*hash_val)%HASHTABLE_DIM; //funzione hash dal libro C programming language
    }

    return hash_val % HASHTABLE_DIM;
    
}
