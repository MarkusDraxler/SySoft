#include<stdio.h>

typedef struct buffer {
	struct buffer* next;
	size_t size;
	size_t left;
	char* buf;
} Buffer;

int main() {
	size_t pagesize = getpagesize();
	Buffer* head = 0;
	Buffer* tail = 0;
	Buffer* free = 0;
	char* cp;
	int ch;
	while((ch = getchar()) != EOF) {
		if(ch == '\n') {
			//print buffer
			print_buffer(head); putchar('\n');
			if(tail) {
				tail->next = free; free = head;
				head = 0; tail = 0;
			}
		} else {
			//allocate buffer
			if(!head || head->left == 0) {
				Buffer* bufp;
				if (free) {
					bufp = free; free = free->next;
				} else {
					bufp = (Buffer*) sbrk(pagesize);
					if(bufp == (void*) - 1) {
						perror("sbrk"); exit(1);
					}
				}
				bufp->next = head;
				bufp->size = pagesize - sizeof(struct buffer);
				bufp->left = bufp->size;
				bufp->buf = (char*)bufp + sizeof(struct buffer);
				head = bufp;
				if(!tail) tail = bufp;
				cp = bufp->buf + bufp->size;
			}
			*--cp = ch; --head->left;
		}
	}
	if(head) print_buffer(head);

	return 0;
}

void print_buffer(Buffer* bufp) {
	while (bufp) {
		printf("%.*s", bufp->size - bufp->left, bufp->buf + bufp->left);
		bufp = bufp->next;
	}
}
