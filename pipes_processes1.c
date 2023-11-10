// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
  
int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    pid_t p;  // Used to store two ends of second pipe 
  
    if (pipe(fd) == -1){
      fprintf(stderr, "Pipe Failed");
      return 1;
    }

    p = fork();
    
    if (p < 0) {
      fprintf(stderr, "fork Failed");
      return 1;
    }

    // Parent process (P1)
    else if (p > 0) {
      close(fd[0]); 

      char input_str[100];
      printf("Enter a string: ")
      scanf("%s", input_str);
    }

    write(fd[1], input_str, strlen(input_str) + 1);

    wait(NULL);

    char concat_str[100];
    read(fd[1], concat_str, 100);
    printf("Concatenated string: %s/n", concat_str);

    close(fd[1]);

    char input_str[100];

    read(fd[0], input_str, 100);

    char fixed_str[] = "howard.edu"; 
    int k = strlen(input_str);
    int i;
    for (i = 0; i < strlen(fixed_str); i++)
        input_str[k++] = fixed_str[i];
    input_str[k] = '\0';

    printf("Concatenated string in P2: %s/n", input_str);

    write(fd[1], input_str. strlen(input_str) + 1);

    close(fd[0]);

    exit(0);    