#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {
    if (argc < 6) {
        printf("Incorrect number of arguments\n Expected: 5\n Actual: %i\n", argc);
        exit(EXIT_FAILURE);
    }
    const int shared_memory_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int fd[2];
    pipe(fd);

    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
            return 1;
        }
        // child process
        else if (pid == 0) {
            char buffer[5];
            sprintf(buffer, "%d", fd[0]);
            execlp("./checker", "./checker", argv[1], argv[i + 2], (char *)NULL);
            perror("exec failed");
            _exit(EXIT_FAILURE);
        }
        // parent process
        else {
            close(fd[0]);
            write(fd[1], &argv, sizeof(argv));
            close(fd[1]);
            printf("Coordinator: forked process with ID %i.\n", (int) pid);
            printf("Coordinator: waiting for process [%i].\n", (int) pid);
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                const int exit_code = WEXITSTATUS(status);
                printf("Coordinator: child process %i returned %i\n", (int) pid, exit_code);
        }
            const int *shared_memory_ptr = shmat(shared_memory_id, NULL, 0);
            shmctl(*shared_memory_ptr, IPC_RMID, NULL);
        }

    }
    printf("Coordinator: exiting\n");
    return 0;
}