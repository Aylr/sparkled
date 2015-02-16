#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// strtok takes a set of characters and pulls out the "tokens" in between the 
// delimiters.  You need to call it repeatedly, once for every token in the 
// string.  The first time you call it, you pass it the string.  For subsequent 
// calls, you pass it a NULL pointer, and it knows how to pick up where it left 
// off.  You know when you've gone through the whole string when strtok returns 
// NULL instead of a string.
//
// atoi takes a string representation of an integer and converts it to an 
// integer variable type.  There's also atol for longs and atof for floats.
// 
// Also note the two different ways of setting up strings in C -- as a character
// array and a character pointer.

int main ()
{
  // strtok example
  char str[] ="- This, is a sample string.";
  char * tok;

  printf("\nSplitting string \"%s\" into tokens:\n",str);
  // Pass the string and the delimiters you want it to look for
  tok = strtok(str, " ,.-");
  while (tok != NULL) {
    printf("%s\n",tok);
    tok = strtok(NULL, " ,.-");
  }

  // strtok and atoi together!
  char str2[] = "4,8,15,16,23,42";
  int nums[6];
  unsigned char i = 0;

  printf("\nConverting string \"%s\" into integers:\n",str2);
  tok = strtok(str2, ",");
  while (tok != NULL) {
    nums[i] = atoi(tok);
    tok = strtok(NULL, ",");
    i++;
  }

  // Print out the result
  printf("Integers extracted:");
  for (i = 0; i < 6; i++) {
    printf(" %d", nums[i]);
  }
  printf("\n");

  return 0;
}
