Files:
Checker.c
Checker.h
Coordinator.c
Makefile

Coordinator: The coordinator is responsible for using the
(1) fork() command to launch another process
(2) exec() command to replace the program driving this process, while also supplying the arguments that
this new program (Checker) needs to complete its execution.
(3) wait() command to wait for the completion of the execution of the process.

Checker: This program requires two arguments to complete its task. The Checker checks whether or not
argTwo (the dividend) is divisible by argOne (the divisor) and prints out the result. Both these arguments are
positive integers.