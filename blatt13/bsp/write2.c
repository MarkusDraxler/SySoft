#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int fd;
	int n=0;

	struct stat attr;

	while(argv[++n] !=NULL) {
		if((fd=open(argv[n], O_RDONLY)) < 0) {
			write(2, "Fehler!\n", 8);
			continue;
		}
		if(fstat(fd, &attr) == -1) {
			write(2, "Fehler!\n", 8);
			continue;
		}

		char* buf  = (char*)mmap(0, attr.st_size, PROT_READ, MAP_SHARED, fd, 0);

		if(buf == MAP_FAILED) {
			write(2, "Fehler!\n", 8);
			continue;
		}

		close(fd);

		int nbytes = attr.st_size;
		char *cp = buf;
		while(cp < buf + nbytes) {
			write(1, cp, 1);
			++cp;
		}

		munmap((caddr_t) buf, attr.st_size);
	}

}
