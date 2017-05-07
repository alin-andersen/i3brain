#ifndef ELIST_H
#define ELIST_H

struct elist
{
    struct elist* next;
    struct elist* prev;
};

#define byte char
#define STRUCT_OFFSET(item, type, member) ((type*)((byte*)(item)-(unsigned long)(&((type*)0)->member)))
#define ELIST_INIT(list)      (list)->next  =  (list); (list)->prev = (list);
#define ELIST_EMPTY(list)     ((list)->next == (list)->prev)
#define ELIST_NOT_EMPTY(list) ((list)->next != (list)->prev)
#define ELIST_FIRST(list)     ((list)->next)
#define ELIST_LAST(list)      ((list)->prev)
#define ELIST_LINK_AFTER(list, item)	 \
    (list)->next->prev = item;		 \
    (item)->next = (list)->next;	 \
    (list)->next = item;		 \
    (item)->prev = list;
#define ELIST_LINK_BEFORE(list, item)		  \
    (list)->prev->next = item;			  \
    (item)->prev = (list)->prev;		  \
    (list)->prev = item;			  \
    (item)->next = list;
#define ELIST_UNLINK(item)			\
    (item)->next->prev = (item)->prev;		\
    (item)->prev->next = (item)->next;
#define ELIST_FOREACH_BEG(list) for(struct elist* _ptr = (list)->next, *_next; _ptr != (list);) { _next = _ptr->next;
#define ELIST_FOREACH_END _ptr = _next; }
#define ELIST_FOREACH_DATA(type, member) ((type*)STRUCT_OFFSET((byte*)_ptr, type, member))
#define ELIST_FOREACH_LINK_AFTER(item)  ELIST_LINK_AFTER(_next, item)
#define ELIST_FOREACH_LINK_BEFORE(item) ELIST_LINK_BEFORE(_next, item)
#define ELIST_FOREACH_UNLINK            ELIST_UNLINK(_ptr)

#endif
