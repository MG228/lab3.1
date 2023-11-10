#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <argument>\n", argv[0]);
        return 1;
    }

    int pipe1[2];
    int pipe2[2];
    pid_t p1, p2, p3;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    p1 = fork();
    
    if (p1 < 0) {
        perror("Fork (P1) failed");
        return 1;
    }
    
    if (p1 == 0) {
        // P1 (Parent) - cat scores
        close(pipe1[0]); // Close reading end of pipe1
        dup2(pipe1[1], 1); // Redirect stdout to pipe1
        close(pipe1[1]);
        
        execlp("cat", "cat", "scores", NULL);
        perror("execlp (P1) failed");
        exit(1);
    }

    p2 = fork();
    
    if (p2 < 0) {
        perror("Fork (P2) failed");
        return 1;
    }
    
    if (p2 == 0) {
        // P2 (Child) - grep
        close(pipe1[1]); // Close writing end of pipe1
        close(pipe2[0]); // Close reading end of pipe2
        dup2(pipe1[0], 0); // Redirect stdin from pipe1
        dup2(pipe2[1], 1); // Redirect stdout to pipe2
        close(pipe1[0]);
        close(pipe2[1]);
        
        execlp("grep", "grep", argv[1], NULL);
        perror("execlp (P2) failed");
        exit(1);
    }

    p3 = fork();
    
    if (p3 < 0) {
        perror("Fork (P3) failed");
        return 1;
    }
    
    if (p3 == 0) {
        // P3 (Child's Child) - sort
        close(pipe2[1]); // Close writing end of pipe2
        dup2(pipe2[0], 0); // Redirect stdin from pipe2
        close(pipe2[0]);
        
        execlp("sort", "sort", NULL);
        perror("execlp (P3) failed");
        exit(1);
    }
    
    close(pipe1[0]); // Close reading end of pipe1 in the parent
    close(pipe1[1]);
    close(pipe2[0]); // Close reading end of pipe2 in the parent
    close(pipe2[1]);
    
    wait(NULL); // Wait for P1
    wait(NULL); // Wait for P2
    wait(NULL); // Wait for P3
    
    return 0;
}
