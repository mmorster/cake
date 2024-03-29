#ifndef _MEL_LOGS_H
#define _MEL_LOGS_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum {
	LOG_WARNING, LOG_SUCCESS, LOG_ERROR, LOG_INFORMATION
} logging;

#if __WIN32
#include <windef.h>
#include <winbase.h>
#include <wincon.h>

#define TERM_GREEN FOREGROUND_GREEN
#define TERM_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define TERM_RED FOREGROUND_RED
#define TERM_BLUE FOREGROUND_BLUE

#define set_term_color(CONSOLE, TERM_COLOR) SetConsoleTextAttribute(CONSOLE, TERM_COLOR)
#define clear_term_color(CONSOLE, ATR) SetConsoleTextAttribute(CONSOLE, ATR)
#else
#include <unistd.h>

#define TERM_GREEN "\033[32m"
#define TERM_YELLOW "\033[33m"
#define TERM_RED "\033[31m"
#define TERM_BLUE "\033[34m"

#define set_term_color(TERM_COLOR) printf(TERM_COLOR)
#define clear_term_color() printf("\033[0m")
#endif

static void MEL_log(int, const char *, ...);

#if __WIN32
static bool dirty = false;
static HANDLE hConsole;
static WORD saved_attributes;
#endif

inline static void MEL_log(int l, const char * format, ...){
#if __WIN32
	if (!dirty){
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
		saved_attributes = consoleInfo.wAttributes;
		dirty = true;
	}
#endif
    switch(l){
#if __WIN32
        case LOG_WARNING:
            set_term_color(hConsole, TERM_YELLOW);
            fprintf(stdout, "[!] ");
            break;
        case LOG_SUCCESS:
            set_term_color(hConsole, TERM_GREEN);
            fprintf(stdout, "[+] ");
            break;
        case LOG_ERROR:
            set_term_color(hConsole, TERM_RED);
            fprintf(stdout, "[-] ");
            break;
        case LOG_INFORMATION:
            set_term_color(hConsole, TERM_BLUE);
            fprintf(stdout, "[#] ");
            break;
        default:
            break;
    }
    clear_term_color(hConsole, saved_attributes);
#else
        case LOG_WARNING:
            if (isatty(STDOUT_FILENO)){
                set_term_color(TERM_YELLOW);
            }
            fprintf(stdout, "[!] ");
            break;
        case LOG_SUCCESS:
            if (isatty(STDOUT_FILENO)){
                set_term_color(TERM_GREEN);
            }
            fprintf(stdout, "[+] ");
            break;
        case LOG_ERROR:
            if (isatty(STDOUT_FILENO)){
                set_term_color(TERM_RED);
            }
            fprintf(stdout, "[-] ");
            break;
        case LOG_INFORMATION:
            if (isatty(STDOUT_FILENO)){
                set_term_color(TERM_BLUE);
            }
            fprintf(stdout, "[#] ");
            break;
        default:
            break;
    }
    if (isatty(STDOUT_FILENO)){
        clear_term_color();
    }
#endif
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	putchar('\n');
	va_end(args);
}

#endif
