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

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit"};

int (*builtin_func[])(char **) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit};

int lsh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

void blue()
{
    printf("\033[0;34m");
}

void reset()
{
    printf("\x1B[0m");
}

void magenta()
{
    printf("\x1B[35m");
}

/*
  Builtin function implementations.
*/
int lsh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("lsh");
        }
    }
    return 1;
}

int lsh_help(char **args)
{
    int i;
    magenta();
    printf("VSH help !!\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");
    reset();
    blue();

    for (i = 0; i < lsh_num_builtins(); i++)
    {
        printf("  %s\n", builtin_str[i]);
    }

    reset();
    magenta();
    printf("Use the man command for information on other programs.\n");
    reset();
    return 1;
}

int lsh_exit(char **args)
{
    return 0;
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
    {
        return 1;
    }
    for (i = 0; i < lsh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return launch(args);
}

void loop()
{
    char *line;
    char **args;
    int status;

    do
    {
        blue();
        printf("->");
        reset();
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