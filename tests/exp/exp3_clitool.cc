#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main() {
    fork();
    system("free -h");
    return 0; 
}