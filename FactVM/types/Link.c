#include "Link.h"

inline Link* Link_last(Link* link) {
	Link* result = 0;
	while (link) { link = (result = link)->next; }
	return result;
}
Link* Link_append(Link* head, Link* item, Link* tail) {
	item->next = 0;
	if (!head) return 0;
	if (!tail) tail = Link_last(head);
	tail->next = item;
	return 0;
}


Link* Link_searchByIndex(Link* link, size_t index) {
	size_t c = 0;
	while (link) {
		if (c == index) {
			return link;
		}
		link = (link)->next;
		c++;
	}
	return 0;
}
size_t Link_searchByValue(Link* link, word_t value) {
	size_t c = 0;
	while (link) {
		if (*((word_t*)(link + 1)) == value) {
			return c;
		}
		link = (link)->next;c++;
	}
	return -1;
}

size_t Link_searchByItem(Link* link, void* compareItem, size_t itemSize) {
	size_t c = 0;
	while (link) {
		bool_t matched = 1;
		word_t* p = (word_t*)compareItem;
		word_t* lp = (word_t*)(link + 1);
		for (size_t c = 0; c < itemSize; c++) {
			if (*p != *lp) { matched = 0; break; }
			p++; lp++;
		}
		if (matched) {
			return c;
		}
		link = link->next; c++;
	}
	return -1;
}

LinkSearchResult Link_searchByPredicate(Link* link, LinkPredicate predicate, void* pParam) {
	size_t c = 0;
	LinkSearchResult rs;
	while (link) {
		if (predicate(link + 1, c, pParam)) {
			rs.index = c; rs.item = link;
			return rs;
		}
		link = (link)->next;
		c++;
	}
	rs.index = -1; rs.item = 0;
	return rs;
}


LinkRemoveResult Link_removeByIndex(Link* link, size_t index) {
	LinkRemoveResult rs;
	Link* prev = 0;
	size_t c = 0;
	while (link) {
		if (c == index) {
			if (prev) prev->next = link->next;
			rs.link = link;
			rs.prev = prev;
			return rs;
		}
		link = (prev = link)->next;
		c++;
	}
	rs.prev = rs.link = 0;
	return rs;
}
LinkRemoveResult Link_removeByValue(Link* link, word_t value) {
	LinkRemoveResult rs;
	Link* prev = 0;
	while (link) {
		if (*((word_t*)(link + 1)) == value) {
			if (prev) prev->next = link->next;
			rs.link = link; rs.prev = prev;
			return rs;
		}
		link = (prev = link)->next;
	}
	rs.link = rs.prev = 0;
	return rs;
}

LinkRemoveResult Link_removeByItem(Link* link, void* compareItem, size_t itemSize) {
	LinkRemoveResult rs;
	Link* prev = 0;
	while (link) {
		bool_t matched = 1;
		word_t* p = (word_t*)compareItem;
		word_t* lp = (word_t*)(link + 1);
		for (size_t c = 0; c < itemSize; c++) {
			if (*p != *lp) { matched = 0; break; }
			p++; lp++;
		}
		if (matched) {
			if (prev) prev->next = link->next;
			rs.prev = prev; rs.link = link;
			return rs;
		}
		link = (prev = link)->next;
	}
	rs.link = rs.prev = 0;
	return rs;
}

LinkRemoveResult Link_removeByPredicate(Link* link, LinkPredicate predicate, void* pParam) {
	LinkRemoveResult rs;
	size_t c = 0;
	Link* prev = 0;
	while (link) {
		if (predicate(link + 1, c, pParam)) {
			if (prev) prev->next = link->next;
			rs.link = link; rs.prev = prev;
			return rs;
		}
		link = (prev = link)->next;
		c++;
	}
	rs.link = rs.prev = 0;
	return rs;
}


size_t Link_length(Link* link) {
	size_t index = 0;
	while (link) {
		link = link->next;
		index++;
	}
	return index;
}