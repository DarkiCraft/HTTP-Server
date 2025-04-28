#include "terminal.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void InitTerminalConfig() {
	struct termios term;
	if (tcgetattr(STDIN_FILENO, &term) == 0) {
		term.c_lflag &= ~ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) != 0) {
			perror("Error: In InitTerminalConfig(): tcsetattr() failed");
		}
	} else {
		perror("Error: In InitTerminalConfig(): tcgetattr() failed");
	}

#ifdef NDEBUG
	(void)freopen("/dev/null", "w", stderr);
#endif
}

void RevertTerminalConfig() {
	struct termios term;
	if (tcgetattr(STDIN_FILENO, &term) == 0) {
		term.c_lflag |= ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
	}
}

// src/terminal.c