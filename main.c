#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Give file name.\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);

    char* mapper;

    mapper =  (char*) mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);

    if (mapper == MAP_FAILED)
    {
        printf("MAP_FAILED\n");
        return 1;
    }

    for (int i=0; i<100; i++)
    {
        putchar(mapper[i]);
    }

}