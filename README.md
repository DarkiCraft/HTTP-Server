
# Usage Guide

## Pre-Compilation Setup

Before compiling the project, ensure all the necessary dependencies are installed. To do this, you need to run the `configure` script.

**Run the configure script**:
```bash
./configure
```

This script will check if the required commands (`gcc`, `make`, `ar`) and packages (`binutils`, `libsqlite3-dev`) are installed. If any dependencies are missing, the script will attempt to install them.

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

## Summary

- **GET:** Retrieve information based on the roll number.
- **POST:** Add information (roll number and name).
- **DELETE:** Remove information by specifying the roll number.

Ensure the server is running and accessible before making any requests.

---