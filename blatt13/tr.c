#include"libowfat-0.32/stralloc.h"
#include<unistd.h>
#include<regex.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>

extern char* cmdname;
void die(char* filename);


int main(int argc, char* argv[]) {
	regmatch_t pmatch[2];
	size_t nmatch = 2;
	regex_t regex;
	char* pattern = argv[1];
	char* linetochange;

	cmdname = argv[3];

	for(int i=0; i<=argc; i++) printf("%s", argv[i]);

	char* infile = argv[3];
	char* buf;
	ssize_t nbytes;

	int infd = open(infile, O_RDWR);
	int outfd = open(infile, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	struct stat statbuf;
	fstat(infd, &statbuf);
	nbytes = statbuf.st_size;
	buf = (char*) mmap(0, nbytes, PROT_READ, MAP_SHARED, infd, 0);
	if(buf == MAP_FAILED) die(infile);
	ssize_t count;
	for(ssize_t written = 0; written < nbytes; written += count) {
		count = write(outfd, buf + written, nbytes - written);
		if(count <= 0) die (infile);
	}
	if(nbytes < 0) die (infile);

	//make the change with regexec
	unsigned int regex_flags = REG_NOSUB;
	unsigned int regex_error = regcomp(&regex, pattern, regex_flags);
	regexec(&regex, linetochange, nmatch, pmatch, 0);

	return 0;
}

void die(char* filename) {
	stralloc msg = {0};
	if(stralloc_copys(&msg, cmdname) && stralloc_cats(&msg, ": ") && 
			stralloc_cats(&msg, strerror(errno)) && 
			stralloc_cats(&msg, ": ") &&
			stralloc_cats(&msg, filename) &&
			stralloc_cats(&msg, "\n")) {
		write(2, msg.s, msg.len);

	} exit(1);
}
