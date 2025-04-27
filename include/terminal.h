#ifndef TERMINAL_H
#define TERMINAL_H

#ifndef ECHOCTL
#define ECHOCTL 0001000
#endif

/**
 * @brief Disables terminal echoing of control characters like ^C and ^Z.
 *
 * This function modifies the terminal attributes to prevent display of control
 * characters when typed (e.g., ^C for SIGINT), and redirects stderr to
 * /dev/null.
 */
void InitTerminalConfig();

/**
 * @brief Restores terminal echoing of control characters like ^C and ^Z.
 *
 * This function re-enables the echoing of control characters, reversing the
 * effect of InitTerminalConfig().
 */
void RevertTerminalConfig();

#endif	// TERMINAL_H

// include/terminal.h