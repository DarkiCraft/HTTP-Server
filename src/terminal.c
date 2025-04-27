#include "terminal.h"

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void InitTerminalConfig() {
	struct termios term;
	if (tcgetattr(STDIN_FILENO, &term) == 0) {
		term.c_lflag &= ~ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) != 0) {
			perror("Error: In DisableConstrolCharEcho(): tcsetattr() failed");
		}
	} else {
		perror("Error: In DisableConstrolCharEcho(): tcgetattr() failed");
	}

	(void)freopen("/dev/null", "w", stderr);
}

void RevertTerminalConfig() {
	struct termios term;
	if (tcgetattr(STDIN_FILENO, &term) == 0) {
		term.c_lflag |= ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
	}
}

// src/terminal.c