
# Usage Guide

## Pre-Compilation Setup

Before compiling the project, ensure all the necessary dependencies are installed. To do this, you need to run the `configure` script.

**Run the configure script**:
```bash
./configure
```

This script will check if the required commands (`gcc`, `make`, `ar`) and packages (`curl`, `binutils`, `libsqlite3-dev`) are installed. If any dependencies are missing, the script will attempt to install them.

It will also verify that you have the correct version of GCC installed (version 5 or higher).

**If any issues occur during configuration**, the script will provide relevant error messages. Follow the instructions to resolve them before continuing.

Once the configuration script completes successfully, you are ready for the next steps.

---

## Compilation and Running the Server

To compile the project, run:
```bash
make
```

If you encounter issues, try cleaning and rebuilding:
```bash
make clean && make
```

To start the server:
```bash
./server
```

After running the server, **note down the IP address displayed in the terminal**. You will use this IP for the requests.

---

## Using the `client` to Send Requests

To launch the client application, run:

```bash
./client
```

You will be guided through on-screen prompts to perform various requests. The client serves as a user-friendly wrapper for the core request types described below.

If you prefer to send requests manually via the command line, refer to the section below.

---

## GET Requests

To make a `GET` request, use the following format:
```bash
curl http://<server_ip>:<port>?roll_num=23K-0760
```

**Note:** The roll number must follow the format `YYA-DDDD` (e.g., `23K-0760`).

---

## POST Requests

To make a `POST` request, send the roll number and name in JSON format:
```bash
curl http://<server_ip>:<port> -H "Content-Type: application/json" -d '{"roll_num": "23K-0760", "name": "Muhammad Abd-Ur-Rahman"}'
```

---

## DELETE Requests

To make a `DELETE` request, specify the roll number in the query string:
```bash
curl http://<server_ip>:<port>?roll_num=23K-0760 -X DELETE
```

---

##  Closing the Server and Client

Signal handling has been implemented, ensuring that both the `client` and `server` programs will exit gracefully upon receiving SIGINT or SIGTERM signals.

To terminate either program:

- **Using Ctrl + C**: While the program is running, press `Ctrl + C` to send the `SIGINT` signal.
- **Sending a signal via the kill command**: You can also send a signal using the `kill` command, specifying the process ID (PID) of the program. For example:
```bash
kill -SIGTERM <PID>  # Sends SIGTERM to the specified process
```

Where `<PID>` is the process ID of either the server or client. You can find the PID using the `ps` command:
```bash
ps aux | grep <program_name>
```

This ensures a clean shutdown of resources, including closing any open sockets or freeing allocated memory.

---