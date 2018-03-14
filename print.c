/*print.c: prints the necessary info given in the struct list*/
#include "print.h"

void print(stage_stats **stats, char *buffer) {
   int i = 0;
   char *token;
   token = strtok(buffer, "|");

   /*get the proper line for each stage*/
   while (stats[i] != NULL && i < MAX_COMMANDS && token != NULL) {
      print_stats(i, stats[i], token);
      token = strtok(NULL, "|");
      /*free(stats[i]);*/
      i++;
   }
}


/*prints all of the information for a stage*/
void print_stats(int i, stage_stats *stg, char *line) {
   int k = 0;
 
   printf("\n--------\n");
   printf("Stage %d: \"%s\"\n", i, line);
   printf("--------\n");

   if (stg->input_line == NULL) {
      printf("     input: pipe from stage %d\n", i - 1);
   }
   else {
      printf("     input: %s\n", stg->input_line);
   }

   if (stg->output_line == NULL) {
      printf("    output: pipe to stage %d\n", i + 1);
   }
   else {
      printf("    output: %s\n", stg->output_line);
   }

   printf("      argc: %d\n", stg->num_args);
   printf("      argv: ");

   for (; k < stg->num_args - 1; k++) {
      printf("\"%s\",", stg->arg_list[k]);
   }

   printf("\"%s\"\n", stg->arg_list[k]);

   /*free input string*/
   /*free(stg->input_line);*/

   /*free output string*/
   /*free(stg->output_line);*/

   /*free argument lists*/
   /*free(stg->arg_list);*/
}
