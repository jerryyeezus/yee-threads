#include "list.h"
#include <stdio.h>
#include <stdlib.h>

list *list_create ( )
{
    list *ret = malloc ( sizeof ( list ) );
    ret->head = NULL;
    ret->tail = NULL;
    ret->size = 0;
    return ret;
}

/**
* list_remove
*
* Update list pointers only, then return data member of the removed node.
*/
void *list_remove ( list *list, node *it )
{
    if ( list->size > 2 )
    {
        it->prev->next = it->next;
        it->next->prev = it->prev;
    }
    else
    {
        it->prev->next = it->prev;
        it->prev->prev = it->prev;
    }

    if ( it == list->head )
        list->head = list->head->next;

    if ( it == list->tail )
        list->tail = list->tail->prev;

    list->size--;
    return it->data;
}

/**
* list_add
*
* Add to tail of the list
*/
void list_add ( list *list, void *data )
{
    int size;
    node *new = malloc ( sizeof ( node ) );
    new->data = data;
    size = list->size;
    if ( size == 0 )
    {
        new->next = new;
        new->prev = new;
        list->head = new;
        list->tail = new;
    }
    else if ( size == 1 )
    {
        new->next = list->head;
        new->prev = list->head;
        list->head->next = new;
        /* added this... */
        list->head->prev = new;

        list->tail = new;
    }
    else
    {
        new->next = list->head;
        new->prev = list->tail;
        list->tail->next = new;
        list->head->prev = new;
        list->tail = new;
    }

    list->size++;
}

void list_print ( list *list, void ( *print ) ( void * ) )
{
    node *cur = list->head;
    int i = 0;
    while ( i++ < list->size )
    {
        print ( cur->data );
        cur = cur->next;
    }
}


/*
int main ( int argc, const char *argv[] )
{
    list *threads = list_create();
    int a =5;
    int b = 4;
    int c = 9;
    void *foo = &a;
    list_add ( threads, foo );
    foo = &b;
    list_add ( threads, foo );
    foo = &c;
    list_add ( threads, foo );

//    node *tmp = list_reZZ

    list_print ( threads, printShit );
    return 0;
}
*/
