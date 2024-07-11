#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char* argv[]) {
  // declare pipes
  int fd1[2], fd2[2];
  pid_t pid;

  if (pipe(fd1) == -1 || pipe(fd2) == -1) {
    fprintf(stderr, "Pipe failed");
    return 1;
  }

  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork Failed");
    return 1;
  }

  if (pid > 0) { // Parent process
    close(fd1[READ_END]);
    close(fd2[WRITE_END]);

    
    FILE* fp = fopen("input.txt", "r");
    if (fp == NULL) {
      fprintf(stderr, "Failed to open file\n");
      exit(1);
    }

    char line[BUFSIZ];
    while (fgets(line, BUFSIZ, fp)) {
      write(fd1[WRITE_END], line, strlen(line));
      memset(line, 0, BUFSIZ);
      read(fd2[READ_END], line, BUFSIZ);
      printf("%s", line);
    }

    fclose(fp);

    close(fd1[WRITE_END]);
    close(fd2[READ_END]);

  }
  else { // Child process
    pid = fork();

    if (pid > 0) { // Second process
      close(fd1[WRITE_END]);
      close(fd2[READ_END]);

      char line[BUFSIZ];
      while (read(fd1[READ_END], line, BUFSIZ)) {
        for (int i = 0; i < strlen(line); i++) {
          // Change lower to upper and upper to lower cases
          
          // write code here
          if (islower(line[i])) {
		  line[i] = toupper(line[i]);
	     } else if (isupper(line[i])) {
		  line[i] = tolower(line[i]);
	     }
        }
        write(fd2[WRITE_END], line, strlen(line));
        memset(line, 0, BUFSIZ);
      }

      close(fd1[READ_END]);
      close(fd2[WRITE_END]);

    }
    else { // Third process

      // Close unused pipes

      // write code here
      close(fd1[READ_END]);
      close(fd1[WRITE_END]);
	close(fd2[WRITE_END]);
      FILE* fp = fopen("output.txt", "w");

      char line[BUFSIZ];

      while (read(fd2[READ_END], line, BUFSIZ)) {
        // prints to the output.txt file

        // write code here
        fprintf(fp, "%s", line);
	  memset(line, 0, BUFSIZ); 
      }

      fclose(fp);

      close(fd2[READ_END]);
    }
  }
  return 0;
}
