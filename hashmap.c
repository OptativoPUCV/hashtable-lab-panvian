#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;
int enlarge_called=0;

struct Pair {
     char * key;
     void * value;
};

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  unsigned long p=hash(key,map->capacity);
  while(map->buckets[p]!=NULL && is_equal(map->buckets[p]->key,key)==0){
    p++;
    if(p==map->capacity) p=0;
  }
  map->size++;
  map->buckets[p]=createPair(key,value);
  map->current=p;
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  int p=0;
  Pair ** old_b=(Pair**)malloc(map->capacity*sizeof(Pair));
  old_b=map->buckets;
  map->capacity=map->capacity*2;
  map->buckets=(Pair**)malloc(map->capacity*sizeof(Pair));
  while(p<map->capacity/2) {
    if(old_b[p]!=NULL && old_b[p]->key!=NULL) {
      insertMap(map, old_b[p]->key, old_b[p]->value);
    }
    p++;
  }
}


HashMap * createMap(long capacity) {
  HashMap *map=(HashMap *)  malloc(sizeof(HashMap));
  map->buckets=(Pair**)malloc(capacity*sizeof(Pair));
  map->capacity=capacity;
  map->size=0;
  map->current=-1;
  return map;
}

void eraseMap(HashMap * map,  char * key) {    
  unsigned long p=hash(key,map->capacity);
  while(map->buckets[p]!=NULL) {
    if(is_equal(key,map->buckets[p]->key)) {
      map->buckets[p]->key=NULL;
      map->size--;
      return;
    }
    p++;
    if(p==map->capacity) p=0; 
  }
}

void * searchMap(HashMap * map,  char * key) {   
  unsigned long p=hash(key,map->capacity);
  do {
    if(is_equal(map->buckets[p]->key,key)) {
      map->current=p;
      return map->buckets[p]->value;
    }
    p++;
    if(p==map->capacity) p=0;
  }while(map->buckets[p]!=NULL);
  return NULL;
}

void * firstMap(HashMap * map) {  //si el mapa esta vacio no sirve
  unsigned long p=0;
  while(map->buckets[p]==NULL || map->buckets[p]->key==NULL) {
    p++;
    if(p==map->capacity) return NULL;
  }
  map->current=p;  
  return map->buckets[p]->value;  
}

void * nextMap(HashMap * map) {
  int p=map->current+1;
  while(map->buckets[p]==NULL || map->buckets[p]->key==NULL) {
    p++;
    if(map->buckets[p]==NULL || map->buckets[p]->key==NULL) return NULL;
  }
  map->current=p;
  return map->buckets[p]->value;
}
