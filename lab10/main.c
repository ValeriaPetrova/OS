#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <wait.h>

#define EXIT_CODE 1
#define ERROR -1
#define SUCCESS 0
#define CHILD 0
#define TRUE 1
#define NUM_OF_ARGS 2

int main(int argc, char * argv[]){
    pid_t pid;// pid_t тип данных для ID процесса
    if (argc < NUM_OF_ARGS) {
        printf ("incorrect number of arguments\n");
        exit(EXIT_CODE);
    }
    pid = fork();// создать новый прооцесс. Родительский процесс -> идентификатор порожденного
    //процесса. Проржденный процесс -> 0. Неудача -> -1 и устанавливается значение errno

    if (pid == ERROR) {
        perror("Fork error");
        exit(EXIT_CODE);
    }

    if (pid == CHILD) {// порожденный процесс
        char *command = argv[1];
        int callingCode = execvp(command, argv + 1);
        if (callingCode == ERROR) {
            perror("Execvp error");
            exit(EXIT_CODE);
        }
    }

    else {
        int status;
        pid_t childID = wait(&status);//ожидание остановки или завершения порожденного процесса
        if (childID == ERROR) {
            perror("Error in wait");
            exit(EXIT_CODE);
        }

        if (WIFEXITED(status) == TRUE) {
            int exitStatus = WEXITSTATUS(status);
            printf("\nThe child process %d ended with code %d\n", childID, exitStatus);// возвращает код завершения подпроцесса
            exit(SUCCESS);
        } else if (WIFSIGNALED(status) == TRUE){
            int signal = WTERMSIG(status);
            printf("\nthe child process %lld failed with signal%d\n", pid, signal);
            exit(EXIT_CODE);
        }
    }
}
