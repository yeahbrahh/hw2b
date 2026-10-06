#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 6) {
        printf("Incorrect number of arguments\n Expected: 5\n Actual: %i\n", argc);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
            return 1;
        }
        else if (pid == 0) {
            execlp("./checker", "./checker", argv[1], argv[i + 2], (char *)NULL);
            perror("exec failed");
            _exit(EXIT_FAILURE);
        }
        else {
            printf("Coordinator: forked process with ID %i.\n", (int) pid);
            printf("Coordinator: waiting for process [%i].\n", (int) pid);
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                const int exit_code = WEXITSTATUS(status);
                printf("Coordinator: child process %i returned %i\n", (int) pid, exit_code);
        }
        }

    }
    printf("Coordinator: exiting\n");
    return 0;
}