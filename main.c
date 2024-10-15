#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>


typedef struct {
	unsigned int line_count;
	unsigned int word_count;
	unsigned int char_count; 
} WC_Stats;

WC_Stats count(char* file, size_t size)
{
	WC_Stats stats = {0, 0, 0};
	bool previous_space = false;
	
	for (size_t i = 0; i < size; i++)
	{
		if (file[i] == '\0')
		{
			break;
		}
		if (isspace(file[i]))
		{
			if (!previous_space)
			{
				stats.word_count++;
			}
			if (file[i] == '\n')
			{
				stats.line_count++;
			}
			previous_space = true;
		}
		else
		{
			previous_space = false;	
		}
		stats.char_count++;
	}
	return stats;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Give file name.\n");
        return 1;
    }

    char* filename = argv[1];

    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
    	printf("Cannot open '%s'.\n", filename);
    	return 1;
    }
  
    char* mapped_file = (char*) mmap(NULL, 4092, PROT_READ, MAP_SHARED, fd, 0);
    if (mapped_file == MAP_FAILED)
    {
        printf("File mapping failed.\n");
        return 1;
    }

	WC_Stats stats = count(mapped_file, 4092);

	printf("%d %d %d %s\n", stats.line_count, stats.word_count, stats.char_count, filename);

	return 0;
}
