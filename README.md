# Unix-Programming
Codes in C for developin network applications in UNIX environment

# Description of .c files
User Input is represented by n below

tree.c - program to create n process where the parent has n children and each of these n children have n-1 children and so on.

signal.c - program to show usage of the two user defined signals to achieve process synchronization. There are n processes and ith process is child of (i-1)th process.

file_and_pipe.c - program to transfer data between processes through two methods : temporary files and pipe. Goal is to compare time taken by both methods. There are n processes and ith process is child of (i-1)th process.
