# 📡 IPC Client-Server Calculator

A robust Client-Server application written in **C** that performs mathematical calculations using **Inter-Process Communication (IPC)** mechanisms.
The project demonstrates advanced OS concepts such as Signal Handling, Process Management (`fork`, `exec`), and File I/O.

## 👥 Authors
* **Stav Macri**
* **Nadav Swartz**

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
