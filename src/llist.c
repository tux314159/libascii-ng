#include <llist.h>

/* PUBLIC UTILITY FUNCTIONS */
struct llist_node *llist_getnode(struct llist *in, const size_t idx)
{
    size_t i;
    struct llist_node *p;

    if (in == NULL)
        return NULL;

    i = 0;
    p = in->head;
    TRAVERSE_LLIST(p, if (i++ == idx) return p;);

    return NULL;
}

/* INIT/DEINIT */
enum la_status llist_init(struct llist *in, const size_t dsize)
{
    NULLCHK(in);

    in->len = 0;
    in->dsize = dsize;
    in->head = NULL;
    in->tail = NULL;

    return LASCII_OK;
}

void llist_deinit(struct llist *in)
{
    struct llist_node *p;

    p = in->head;
    TRAVERSE_LLIST(p, free(p->data); free(p->prev););
    free(in->tail);
}


/* INSERT */
struct llist_node *llist_pushback(struct llist *in, const void *val)
{
    struct llist_node *new;

    new = malloc(sizeof(struct llist_node));

    if (new == NULL) {
        return NULL;
    }

    new->data = malloc(in->dsize);

    if (new->data == NULL) {
        free(new);
        return NULL;
    }

    memcpy(new->data, val, in->dsize);

    new->prev = in->tail;
    new->next = NULL;

    if (in->len == 0)
        in->head = in->tail = new;
    else
        in->tail->next = new;

    in->tail = new;
    in->len += 1;

    return new;
}

struct llist_node *llist_addnode(struct llist *in, struct llist_node *nod, const void *val)
{
    struct llist_node *new;

    if (nod == NULL) {
        return llist_pushback(in, val);
    }

    new = malloc(sizeof(struct llist_node));

    if (new == NULL) {
        return NULL;
    }

    new->data = malloc(in->dsize);

    if (new->data == NULL) {
        free(new);
        return NULL;
    }

    memcpy(new->data, val, in->dsize);

    new->prev = nod->prev;
    new->next = nod;

    nod->prev->next = new;
    nod->prev = new;

    in->len += 1;

    return new;
}

struct llist_node *llist_pushfront(struct llist *in, const void *val)
{
    struct llist_node *new;

    new = malloc(sizeof(struct llist_node));

    if (new == NULL) {
        return NULL;
    }

    new->data = malloc(in->dsize);

    if (new->data == NULL) {
        free(new);
        return NULL;
    }

    memcpy(new->data, val, in->dsize);

    new->prev = NULL;
    new->next = in->head;

    if (in->len == 0)
        in->head = in->tail = new;
    else
        in->head->prev = new;

    in->head = new;
    in->len += 1;

    return new;
}

enum la_status llist_popback(struct llist *in)
{
    struct llist_node *old;

    NULLCHK(in);

    old = in->tail;

    if (in->tail != in->head) {
        in->tail->prev->next = NULL;
        in->tail = in->tail->prev;
    }

    free(old->data);
    free(old);

    in->len -= 1;

    return LASCII_OK;
}

enum la_status llist_delnode(struct llist *in, struct llist_node *nod)
{
    NULLCHK(in);
    NULLCHK(nod);

    if (nod != in->head)
        nod->prev->next = nod->next;
    if (nod != in->tail)
        nod->next->prev = nod->prev;
    if (nod == in->head)
        in->head = nod->next;
    if (nod == in->tail)
        in->tail = nod->prev;

    free(nod->data);
    free(nod);

    in->len -= 1;

    return LASCII_OK;
}

enum la_status llist_popfront(struct llist *in)
{
    NULLCHK(in);

    llist_delnode(in, llist_getnode(in, 0));

    return LASCII_OK;
}

