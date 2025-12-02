# 📡 IPC Client-Server Calculator

A robust Client-Server application written in **C** that performs mathematical calculations using **Inter-Process Communication (IPC)** mechanisms.
The project demonstrates advanced OS concepts such as Signal Handling, Process Management (`fork`, `exec`), and File I/O.

## 🛠️ How it Works
The system consists of two main components:
1.  **Server:** Runs in the background, waits for signals, reads requests from a shared file, performs calculations, and signals the result back.
2.  **Client:** Sends a request (PID, numbers, operator) to a shared file, signals the server, and waits for a response (with a timeout mechanism).

### Communication Protocol
* **SIGUSR1:** Used to notify the server that a request is ready, and to notify the client that a result is ready.
* **SIGALRM:** Handles timeouts if the server is unresponsive.
* **File I/O:** Data (`to_srv.txt`, `response_client_PID.txt`) is exchanged via files to avoid shared memory complexity.

## 🚀 Features
* **Concurrency Handling:** Uses a lock mechanism (checking if `to_srv.txt` exists) to manage multiple clients.
* **Error Handling:** Manages division by zero and invalid operations.
* **Timeout Mechanism:** Clients wait a maximum of 15 seconds before timing out.
* **Random Backoff:** Clients sleep for a random time if the server is busy.

## 💻 Usage

### 1. Compile the Code
You need a GCC compiler (Linux/WSL).
```bash
gcc server.c -o server.out
gcc client.c -o client.out
```
### 2. Run the Server
First, start the server in the background:
```bash
./server.out &
```
Note the PID (Process ID) of the server, you will need it.

### 3. Run the Client
Format: ./client.out [Server_PID] [Num1] [Num2] [Operation]

 * Operations: 1 (+), 2 (-), 3 (*), 4 (/)

### Example:

```bash
./client.out 1234 10 5 1
# Output: Result is: 15
```

## 📂 Project Structure
 * server.c: The main server logic and signal handlers.
 * client.c: The client interface for sending requests.
 * Project_Report.pdf: Detailed documentation and execution screenshots.
