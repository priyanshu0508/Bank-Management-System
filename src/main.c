#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "ui.h"
#include "storage.h"
#include "auth.h"

// Globals
static volatile int keep_running = 1;

void handle_sigint(int dummy) {
    (void)dummy;
    ui_clear_screen();
    printf("\n%sGracefully shutting down Bank Management System...%s\n", ANSI_YELLOW, ANSI_RESET);
    keep_running = 0;
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);
    
    // Initialize storage subsystem
    storage_init();
    
    while(keep_running) {
        ui_show_main_menu();
    }
    
    return 0;
}
