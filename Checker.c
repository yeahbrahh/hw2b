//
// Created by kilog on 9/26/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include  <unistd.h>
#include  <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "Checker.h"

bool is_divisible(const int argOne, const int argTwo) {
    return argTwo % argOne == 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Incorrect number of arguments\n Expected: 3\n Actual: %i\n", argc);
    }
    const int shared_memory_id = 0;
    int *shared_memory_ptr = shmat(shared_memory_id, NULL, 0);

    *shared_memory_ptr = 1;
    shmdt(shared_memory_ptr);

    const int fd = atoi(argv[0]);
    const int argOne = atoi(argv[1]);
    const int argTwo = atoi(argv[2]);

    int args[2] = {argOne, argTwo};

    read(fd, &args, sizeof(args));

    pid_t pid = getpid();
    printf("Checker process [%i]: Starting.\n", (int) pid);
    if (argOne == 0) {
        printf("Checker process [%i]: %i *IS NOT* divisible by %i.\n", (int) pid, argTwo, argOne);
        printf("Checker process [%i]: Returning %i.\n", (int) getpid(), 0);
        return 0;
    }
    const bool divisible = is_divisible(argOne, argTwo);
    if (divisible) {
        printf("Checker process [%i]: %i *IS* divisible by %i.\n", (int) pid, argTwo, argOne);
        printf("Checker process [%i]: Returning %i.\n", (int) getpid(), 1);
        return 1;
    }
    else {
        printf("Checker process [%i]: %i *IS NOT* divisible by %i.\n", (int) pid, argTwo, argOne);
        printf("Checker process [%i]: Returning %i.\n", (int) getpid(), 0);
        return 0;
    }

    return 0;

}


