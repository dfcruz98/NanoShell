#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

void execute(char **args);
int args_execution(char **args);
char **split_line(char *l);
char *read_line();
void loop_input();
void signal_handler(int sig);

void execute(char **args)
{
  pid_t p, wpid;

  p = fork();

  if (p == 0)
  {
    if (execvp(args[0], args) == -1)
      printf("Command not found: %s", args[0]);

    exit(EXIT_FAILURE);
  }
  else if (p < 0)
  {
    printf("Error creating new process\n");
  }
  else
  {
    // Wait for process to finish
    wpid = waitpid(p, &wpid, WUNTRACED);
  }
}

int args_execution(char **args)
{

  if (args == NULL || args[0] == NULL)
  {
    return 1;
  }

  if (strcmp(args[0], "clear") == 0)
  {
    printf("Clear %s \n", args[0]);
    return 1;
  }

  execute(args);

  return 1;
}

char **split_line(char *l)
{

  int bufsize = 64;
  int bufs = bufsize;
  int i = 0;

  char **tokens = malloc(sizeof(char *) * bufsize);

  if (!tokens)
  {
    fprintf(stderr, "Error allocating space for tokens");
    exit(EXIT_FAILURE);
  }

  char *t;

  t = strtok(l, " ");

  while (t != NULL)
  {

    tokens[i++] = t;

    if (--bufs < 1)
    {
      bufs = 64;
      tokens = realloc(tokens, sizeof(char *) * bufsize);

      if (!tokens)
      {
        fprintf(stderr, "Error allocating mor space for tokens");
        exit(EXIT_FAILURE);
      }
    }

    t = strtok(NULL, " ");
  }

  tokens[i] = NULL;

  return tokens;
}

char *read_line()
{

  int b_size = 1024;
  char *buffer = malloc(sizeof(char *) * b_size);
  int b_space = b_size;
  int b_index = 0;

  if (!buffer)
  {
    fprintf(stderr, "Erro allocating space to read line");
    exit(EXIT_FAILURE);
  }

  int c;

  while (1)
  {
    // Read new character
    c = getchar();

    if (c == EOF || c == '\n')
    {
      buffer[b_index] = '\0';
      return buffer;
    }
    else
    {
      buffer[b_index++] = c;
    }

    // decrement buffer available space and check if we need to allocate mor
    if (--b_space < 1)
    {
      b_space = b_size;
      buffer = realloc(buffer, sizeof(char *) * b_size);

      if (!buffer)
      {
        fprintf(stderr, "Error allocating mor space to read line");
        exit(EXIT_FAILURE);
      }
    }
  }

  return buffer;
}

// Run shell and execute input
void loop_input()
{
  char *line = NULL;
  char **tokens;

  // Read command from terminal
  while (1)
  {
    printf("# ");

    line = read_line();

    tokens = split_line(line);
    args_execution(tokens);

    free(line);
    free(tokens);
  }
}

int main()
{

  /* --------------------------------- Signals -------------------------------- */

  struct sigaction act;
  act.sa_handler = signal_handler; // Signal handler function

  if (sigaction(SIGINT, &act, NULL) < 0)
    printf("Erro seting handler for SIGINT");

  /* --------------------------------- Execution -------------------------------- */
  loop_input();

  return EXIT_SUCCESS;
}

/**
 * Signal's manager function
 * 
 * @param signo reciving signal
 * */
void signal_handler(int sig)
{

  int aux;
  aux = errno;

  switch (sig)
  {
  case SIGINT:
    // Prevent user from closing the shell using the CTRL C, instead of use exit
    loop_input();
    break;
  default:
    break;
  }

  errno = aux;
}
