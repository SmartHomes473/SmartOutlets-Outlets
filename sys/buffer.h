#ifndef __OUTLETS_BUFFER_H
#define __OUTLETS_BUFFER_H

typedef struct {
	volatile void* buffer;
	volatile void* head;
	volatile void* tail;
	size_t size;
	size_t el_size;
} cbuffer_t;

#define __cbuffer_incr_ptr(CBUF, PTR)\
	PTR += CBUF->el_size;\
	if (PTR > CBUF->buffer + CBUF->size) {\
		PTR = CBUF->buffer;\
	}

#define cbuffer_incr_head(CBUF)\
	__cbuffer_incr_ptr(CBUF, CBUF->head);

#define cbuffer_incr_tail(CBUF)\
	__cbuffer_incr_ptr(CBUF, CBUF->tail);

#endif
