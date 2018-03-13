/*parseline.c: parseline stuff idk*/

int main (int argc, char *argv[]) {
   int total, i, pipe, command;


   /*1: check if the command line length is within limits*/

   total = 0;
   i = 0;
   pipe = 0;
   /*1a: command line length < 512 bytes*/
   for (; i < argc; i++) {
      if (argv[i] == '|') {
         pipe = 1;
      }

      if 
      total += strlen(argv[i]);
   }

}
