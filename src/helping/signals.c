#include "includes.h"

extern pid_t current_pid;

// Catch SIGTSTP or SIGINT and send it to process running
void catchinterrupt(int signo) {
    printf(" Cought : signo %d\n", signo);
    if(current_pid > 0){
        kill(current_pid, signo);
        current_pid = -1;
    }
}
//sets the struct sigaction for the signals
void set_signals(struct sigaction *act){
    act->sa_handler = catchinterrupt;
    act->sa_flags = 0;

    sigfillset(&act->sa_mask);

    sigaction(SIGTSTP, act, NULL);
    sigaction(SIGINT, act, NULL);
}