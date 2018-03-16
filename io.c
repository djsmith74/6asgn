/*io.c: handles all i/o required for command line*/
#include "io.h"

/*returns a char pointer to the whole line input by the user*/
void get_input(char *input) {
   int c;

   /* checking if stdin and stdout are tty */
   if (isatty(fileno(stdin)) == 1 && isatty(fileno(stdout)) == 1) {
       printf("8-P ");
   }
        
   /*get input from user*/
   if (fgets(input, MAX_IN + 5, stdin) == NULL) {
       if (feof(stdin)) {
           printf("someone pressed that ^D AHAH\n");
           exit(EXIT_SUCCESS);
       }
   } 
   if (strcmp(input, "\n") == 0) {
       perror("empty file");
       exit(EXIT_FAILURE);
   }

   /*check if more than 512 chars (bytes) were given*/
   if (strlen(input) > MAX_IN) {
       perror("command too long");
       exit(EXIT_FAILURE);
   }

   c = strlen(input);
   input[c - 1] = '\0';
}

/*cuts up the input string, and returns the number of words*/
int divide_line(char *input, char **buffer) {
   char d[1] = {' '};
   char *token;
   int i = 0;

   /*get the first token*/
   token = strtok(input, d);

   /*walk through other tokens*/
   while (token != NULL) {
      buffer[i] = token;
      token = strtok(NULL, d);
      i++;
   }
   return i;
}
