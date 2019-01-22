#include <stdio.h>
#include <stralloc.h>

/* hash algorithm from Dan Bernstein's cdbhash.c */
#define HASHSTART 5381

static unsigned int compute_hash(stralloc* s) {
   int hashval = HASHSTART;
   for (unsigned int index = 0; index < s->len; ++index) {
	  char ch = s->s[index];
	  hashval += hashval << 5;
	  hashval ^= ch;
   }
   return hashval;
}

int main() {

}
