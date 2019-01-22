#include<stdio.h>
#include"./libowfat-0.32/stralloc.h"
#include<math.h>
#include<limits.h>
#include<string.h>
#include<stdlib.h>

#define PI 3.14159265358979323846

struct hashLoc* insert( const char* o, unsigned int p);
float calculateDistance(float lat1, float lat2, float lon1, float lon2);
int readFile(FILE* fp, stralloc* sa);
unsigned long hashIt(unsigned const char *str); 


struct hashLoc {
	float lon;
	float lat;
	char* loc[PATH_MAX];
	struct hashLoc *next;
} hashLoc;

struct hashLoc *hash_table[255];
int main() {
	stralloc sa = {0};
	const char* file = "./gemeinden.txt";
	FILE* fp = fopen(file, "r");

	while(readFile(fp, &sa)) {
		printf("%s", sa.s);
		stralloc_init(&sa);
	}

	fclose(fp);

	return 0;
}

float calculateDistance(float lat1, float lat2, float lon1, float lon2) {
	float dist=0;
	float lat = (lat1 + lat2) / 2 * 0.01745;
	float dx = 111.3 * cos(lat) * (lon1 - lon2);
	float dy = 111.3 * (lat1 - lat2);

	dist = sqrt(dx * dx + dy * dy);

	return dist;
}

int readFile(FILE* fp, stralloc* sa) {
	sa->len = 0;
	for(;;) {
		if(!stralloc_readyplus(sa, 1)) return 0;
		if(fread(sa->s + sa->len, sizeof(char), 1 , fp) <=0) return 0;
		if(sa->s[sa->len] == '\n') break;
		++sa->len;
	}
	return 0;
}

struct hashLoc* insert( const char* o, unsigned int p) {
	struct hashLoc *pointer;
	int hash_adresse = hashIt(o);

	pointer = hash_table[hash_adresse];

	while(pointer != NULL) {
		if(strcmp(o, pointer->loc) == 0) {
			if(pointer->lon == p) {

				return pointer;
			}
			pointer = pointer->next;
		}
	}

	pointer = malloc(sizeof(struct hashLoc));
	if(pointer == NULL) {
		return NULL;
	}
	strcpy(pointer->loc, o);
	pointer->lon = p;
	pointer->next = hash_table[hash_adresse];
	return pointer;
}

unsigned long hashIt(unsigned const char *str) {
	unsigned long hash = 5381;
	int c;

	while(c == *str++) hash = ((hash << 5) + hash + c);
	return hash;
}
