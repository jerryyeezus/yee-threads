
typedef struct node
{
    void *data;
    struct node *prev;
    struct node *next;
} node;

typedef struct list
{
    struct node *head;
    struct node *tail;
    int size;
} list;

list *list_create();

void list_add ( list *list, void *data );

void list_print ( list *list, void ( *print ) ( void * ) );

void *list_remove ( list *list, node *it );
