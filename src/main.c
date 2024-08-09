#include <stdio.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1
#define READ_BUF_SIZE 1024
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

void loop()
{
    char *line;
    char **args;
    int status;

    do
    {
        printf(">");
        line = read_ln();
        args = tokenize_args(line);
        status = exec(args);
        free(line);
        free(args);

    } while (status);
}

char *read_ln()
{
    int buf_size = READ_BUF_SIZE;
    int pos = 0;
    char *buffer = malloc(sizeof(char) * buf_size);
    int c;

    if (!buffer)
    {
        fprintf(stderr, "vsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Note: Can be replaced with getline instead , But learning things
        //  hard way

        c = getchar();

        if (c == EOF || c == '\n')
        {
            buffer[pos] = '\0';
            return buffer;
        }
        else
        {
            buffer[pos] = c;
        }
        pos++;

        if (pos >= buf_size)
        {
            buf_size += buf_size;
            buffer = realloc(buffer, buf_size);
            if (!buffer)
            {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **tokenize_args(char *stream)
{
    
}

int exec(char **args)
{
}

int main(int argc, char **argv)
{
    loop();
    return EXIT_SUCCESS;
}