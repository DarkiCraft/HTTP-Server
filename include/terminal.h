#ifndef TERMINAL_H
#define TERMINAL_H

#ifndef ECHOCTL
#define ECHOCTL 0001000
#endif

/**
 * @brief Disables terminal echoing of control characters like ^C and ^Z.
 *
 * This function modifies the terminal attributes to prevent display of control
 * characters when typed (e.g., ^C for SIGINT). Useful for cleaner terminal
 * output in server applications or custom shells.
 */
void InitTerminalControlChars();

/**
 * @brief Restores terminal echoing of control characters like ^C and ^Z.
 *
 * This function re-enables the echoing of control characters, reversing the
 * effect of DisableControlCharEcho(). Should be called before exiting the
 * program to restore expected terminal behavior.
 */
void CleanupTerminalControlChars();

#endif	// TERMINAL_H

// include/terminal.h