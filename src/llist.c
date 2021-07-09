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

    if (nod->prev) {
        nod->prev->next = new;
    }
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

/* Delete */

void llist_delnode(struct llist *in, struct llist_node *nod)
{
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

    return;
}

void llist_popfront(struct llist *in)
{
    llist_delnode(in, llist_getnode(in, 0));

    return;
}

void llist_popback(struct llist *in)
{
    struct llist_node *old;

    old = in->tail;

    if (in->tail != in->head) {
        in->tail->prev->next = NULL;
        in->tail = in->tail->prev;
    }

    free(old->data);
    free(old);

    in->len -= 1;

    return;
}

/* RE-ORDERING */

void llist_nodeswap(struct llist_node *a, struct llist_node *b)
{
    if (a == NULL || b == NULL) { /* bruh */
        return;
    }

    /* note we cannot just swap data pointers here; we need to
     * preserve references to nodes.
     */
    if (a->next == b) {
        /* x <-> a <-> b <-> y */
        a->next = b->next;  // y
        b->next = a;        // a
        b->prev = a->prev;  // x
        a->prev = b;        // b

        /* code style exception ehe */
        if (a->next) a->next->prev = a;
        if (b->prev) b->prev->next = b;
    } else if (b->next == a) {
        /* x <-> b <-> a <-> y */
        b->next = a->next;  // y
        a->next = b;        // b
        a->prev = b->prev;  // x
        b->prev = a;        // a

        if (b->next) b->next->prev = b;
        if (a->prev) a->prev->next = a;

    } else {
        /* w <-> a/b <-> x ... y <-> b/a <-> z */
        struct llist_node *t;
        t = a->next;        // x/z
        a->next = b->next;  // z/x
        b->next = t;        // x/z
        t = a->prev;        // w/y
        a->prev = b->prev;  // y/w
        b->prev = t;        // w/y

        if (a->next) a->next->prev = a;
        if (a->prev) a->prev->next = a;
        if (b->next) b->next->prev = b;
        if (b->prev) b->prev->next = b;
    }
}
