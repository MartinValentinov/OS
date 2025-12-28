#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>


int readline(int file)
{
    char buffy = ' ';
    while(buffy != '\n')
    {
        int i = read(file , &buffy, 1);
        if(i == -1)
        {
           perror("ERROR WHILEREADING FILE"); 
           return 1; 
        } 
        if(i == 0) return -1;
        printf("%c", buffy);
    }
    return 0;

}

void read10lines(int file)
{
    for(int i = 0; i <= 10; i++)
    {
        int result = readline(file);
        if(result == -1) break;
    }
}


int main(int argc, char* argv[]) {

    char tmp = 0;
    int count = 0;
    for(int i = 1; i < argc; i++)
    {
        printf("%s\n", argv[i]);
        int cur_file = open(argv[i], O_RDONLY);
        if(cur_file == -1)
        {
            printf("Error opening file: %s\n", argv[i]);
            continue;
        }
        lseek(cur_file, 0, SEEK_END);
        int beginning;
        while(count < 10)
        {
            beginning = lseek(cur_file, -1, SEEK_CUR);
            if (beginning <= 0) break;
            read(cur_file, &tmp, 1);
            if(tmp == '\n')
            {
                count++;
            }
            lseek(cur_file, -1, SEEK_CUR); 
        }
        count = 0;
        read10lines(cur_file);
        printf("\n\n");

        close(cur_file);

    }

    return 0;
}