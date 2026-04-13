#ifndef UI_H
#define UI_H

#include "user.h"

// ANSI Escape Codes for UI
#define ANSI_RESET   "\x1b[0m"
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_BOLD    "\x1b[1m"
#define ANSI_CLEAR_SCREEN "\x1b[2J\x1b[H"

void ui_clear_screen();
void ui_print_header(const char* title);
void ui_print_error(const char* msg);
void ui_print_success(const char* msg);

// Input handling methods
void ui_get_string(const char* prompt, char* buffer, int max_len);
void ui_get_password(const char* prompt, char* buffer, int max_len);
int ui_get_int(const char* prompt);
double ui_get_double(const char* prompt);
void ui_pause();

// Menus
void ui_show_main_menu();
void ui_show_customer_menu(UserAccount* user);
void ui_show_admin_menu(UserAccount* admin);

#endif // UI_H
