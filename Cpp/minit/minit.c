/**
    # Minimal init for Docker containers.

    Based on code and ideas from <http://ewontfix.com/14/>.
 */


#define _GNU_SOURCE // For execvpe
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdlib.h> // exit
#include <stdio.h> // printf


/* Need a global for this so the signal handler knows who to talk to */
int child = 0;


/* Signal handler passes signal to child */
static void sig_handler(int sig) {
    int status;
    kill(child, sig);
    waitpid(child, &status, 0);
    exit(EXIT_FAILURE);
}


int execute(sigset_t *pset, char *prog, char *argv[], char *env[]) {
    child = fork();
    if ( child > 1 ) {
        return child;
    } else if ( child < 0 ) {
        printf("fork failed\n");
        exit(EXIT_FAILURE);
    }
    // We're now in the child so need to exec the requested program
    sigprocmask(SIG_SETMASK, pset, 0);
    setsid();
    setpgid(0, 0);
    execvpe(prog, argv, env);
    printf("execvpe failed\n");
    exit(255);
}


int main(int argc, char *argv[], char *env[]) {
    sigset_t set, oldset;
    int status, child;

    if (getpid() != 1) {
        printf("Minit is not needed, but launching process anyway...");
        execvpe(argv[1], argv + 1, env);
        printf("execvpe failed\n");
        exit(255);
    } else if ( argc < 2 ) {
        printf("Must supply a program for minit to execute\n");
        exit(EXIT_FAILURE);
    }

    sigfillset(&set);
    sigdelset(&set, SIGINT); // Allow ctrl-c to work interactively
    sigdelset(&set, SIGTERM); // Allow docker stop
    sigprocmask(SIG_BLOCK, &set, &oldset);

    child = execute(&oldset, argv[1], argv + 1, env);
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    for ( ;; ) {
        if ( wait(&status) == child ) {
            if ( WIFEXITED(status) ) {
                exit(WEXITSTATUS(status));
            }
        }
    }
}
