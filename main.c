/**
 * Word count with mmap syscall
 * 
 * Authors: Rasmus Salmi & Oskari Heinonen
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

// Struct for storing wc statistics
typedef struct {
	unsigned int line_count;
	unsigned int word_count;
	unsigned int char_count; 
} WC_Stats;

/**
 * @brief Count number of lines, words, and characters in a file (string)
 * 
 * @param file File as a string
 * @param size Size of the file string in bytes
 * @return WC_Stats Struct containing the counted values
 */
WC_Stats count(char* file, size_t size)
{
	WC_Stats stats = {0, 0, 0};
	// Flag for checking if the last character was whitespace
	bool previous_space = false;
	
	for (size_t i = 0; i < size; i++)
	{
		// Assume file ending to whitespace
		if (file[i] == '\0')
		{
			break;
		}
		if (isspace(file[i]))
		{
			// Don't count as a word if multiple whitespaces back to back
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

	// Open file to get it's file descriptor, read-only mode
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
    	printf("Cannot open '%s'.\n", filename);
    	return 1;
    }

	// Get file stats
	struct stat file_stats;
	fstat(fd, &file_stats);
	size_t file_size = file_stats.st_size; // Store size of the file in bytes

	// Use mmap system call to map the file contents to the process's memory space.
	// Map memory in read-mode, MAP_SHARED shouldn't make much of a difference because
	// we are only reading.
    char* mapped_file = (char*) mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
	if (mapped_file == MAP_FAILED)
    {
        printf("File mapping failed.\n");
        return 1;
    }
	
	// word count
	WC_Stats stats = count(mapped_file, file_size);
	printf("%d %d %d %s\n", stats.line_count, stats.word_count, stats.char_count, filename);

	return 0;
}
