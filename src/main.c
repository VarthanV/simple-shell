#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <builtins.h>

#define EXIT_SUCCESS 0
#define READ_BUF_SIZE 1024
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

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
    int buf_size = TOK_BUFSIZE, pos = 0;
    char **tokens = malloc(buf_size * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(stream, TOK_DELIM);
    while (token != NULL)
    {
        tokens[pos] = token;
        pos++;
        token = strtok(NULL, TOK_DELIM);
    }

    tokens[pos] = NULL;
    return tokens;
}

int launch(char **args)
{
    pid_t pid, wpid; // process id and wait pid
    int status;
    //  https://www.geeksforgeeks.org/fork-system-call/
    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1)
        {
            perror("vsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("error forking");
    }
    else
    {
        // Parent process
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int exec(char **args)
{
    int i;
    if (args[0] == NULL)
        return 1;
    
}

void loop()
{
    char *line;
    char **args;
    int status;

    do
    {
        printf("->");
        line = read_ln();
        args = tokenize_args(line);
        status = exec(args);
        free(line);
        free(args);

    } while (status);
}

int main(int argc, char **argv)
{
    loop();
    return EXIT_SUCCESS;
}