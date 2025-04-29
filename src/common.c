#include "common.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

const size_t PORT = 8080;
const size_t MAX_PENDING_CONNECTIONS = 10;
const size_t BUFFER_SIZE = 4096;

volatile sig_atomic_t is_server_running = 0;

const size_t HTTP_INVALID_METHOD = 405;
const size_t HTTP_BAD_REQUEST = 400;
const size_t HTTP_NOT_FOUND = 404;
const size_t HTTP_OK = 200;

// src/common.c