/*
 * map.h
 *
 *  Created on: 6 Jun 2013
 *      Author: arjun
 */

#ifndef MAP_H_
#define MAP_H_

#include "assemblerUtils.h"

typedef struct elem {
	string key;
	uint32_t value;
} elem;

elem* newElem(string k, uint32_t v) {
	elem result;
	result.key = k;
	result.value = v;
	return &result;
}

typedef struct list_elem {
	struct list_elem *next;
	struct list_elem *prev;
	elem* element;
	int position;
} list_elem;

typedef list_elem* listptr;

listptr newList(elem* a) {
	list_elem head;
	head.position = 0;
	head.element->key = a->key;
	head.element->value = a->value;
	head.prev = NULL;
	head.next = NULL;
	return &head;
}

listptr getHead(listptr a) {
	while (a->prev->position != 0) {
		return getHead(a->prev);
	}
	return a;
}

uint32_t getElem(string key, listptr a) {
	listptr head = getHead(a);
	if (strncmp(head->element->key, key, 511) == 0) {
		return (head->element->value);
	} else {
		while (head->next != NULL ) {
			if (strncmp(head->element->key, key, 511) == 0) {
				return head->element->value;
			} else {
				head = head->next;
			}
		}
	}
	return 32;
}

void addElem(elem* x, listptr a) {
	listptr newitem = newList(x);
	if (a == NULL ) {
		a = newitem;
	} else {
		while (a->next != NULL ) {
			a = a->next;
		}
		a->next = newitem;
		newitem->prev = a;
		newitem->position = (a->position)+1;
	}
}

#endif /* MAP_H_ */
