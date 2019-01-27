#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int fd;
	int n=0;

	void *speicherbereich;
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

		speicherbereich = mmap(0, attr.st_size, PROT_READ, MAP_SHARED, fd, 0);

		if(speicherbereich == MAP_FAILED) {
			write(2, "Fehler!\n", 8);
			continue;
		}

		close(fd);
		if(write(1, speicherbereich, attr.st_size) !=attr.st_size) {
			write(2, "Fehler!\n", 8);
			continue;
		}

		munmap((caddr_t) speicherbereich, attr.st_size);
	}

}
