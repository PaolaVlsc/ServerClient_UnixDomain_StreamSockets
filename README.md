# ServerClient_UnixDomain_StreamSockets

This project demostrates how server - client works over a unix domain socket. It utilizes technologies of multithreading, sockets and mutexes.
It was conducted for the final project of University of West Attica for course "Operating Systems II"

# Table of Contents
1. [Server](#server)
   
   1.1 [ask03_server.c](#ask03_serverc)
3. [Client](#client)
   
   2.1 [ask03_client.c](#ask03_clientc)
5. [Built-in Functions](#built-in-functions)
6. [Program Flow](#program-flow)
7. [Run](#run)
8. [Results](#results)
9. [Author](#author)


## Server

### [ask03_server.c](https://github.com/PaolaVlsc/ServerClient_UnixDomain_StreamSockets/blob/main/ask03_server.c)

This C program serves as a multithreaded server for receiving data from clients, calculating the average of the data, and performing checks on the sequence. It is designed to handle multiple client connections simultaneously over a Unix domain socket. The program demonstrates the use of threads, sockets, and mutexes to ensure thread safety.

## Client

### [ask03_client.c](https://github.com/PaolaVlsc/ServerClient_UnixDomain_StreamSockets/blob/main/ask03_client.c)

The code facilitates communication between a client and a server over a Unix domain socket, allowing the client to send an array to the server and receive results based on the server's processing. The client can also choose to exit the program.

## Built-in Functions

In the program, the following functions have been used:

- `pthread_create()`: Used for creating a new thread.
- `pthread_join()`: Used for synchronizing threads or processes.
- `pthread_mutex_lock()`: Used for thread synchronization and mutual exclusion for critical sections.
- `send()`: Used for sending data over a socket.
- `recv()`: Used for receiving data from a socket.
- `bind()`: Used to bind a port to the server.
- `listen()`: Used to make the server listen on a specific port.
- `accept()`: Used to accept a connection from a client.
- `connect()`: Used for a client to connect to a server.
- `close()`: Used to close a socket.
- `socket()`: Used to create a socket.

## Program Flow

The flow of the program is defined as follows:

1. Initially, the server is launched.
2. The server enters a waiting state, waiting for a client to connect.
3. Subsequently, the client program is executed.

In the `main()` function, the following interactions take place:

4. The user is prompted to provide the following information:

   - The size of the array.
   - Input data for array/vector A.

5. The client program waits for a response from the server while the server performs essential calculations. The server calculates the average, increments counters, and sends appropriate messages.

6. Once the client receives the messages from the server, it displays the calculated average on the screen and allows for the input of new data.

7. During the implementation of the exercise, it became evident that mutexes (mutual exclusions) were necessary, as certain variables served as critical sections:
   - `correct_sequence`
   - `numberOfRequests`

These variables are shared among threads, so proper synchronization is essential to prevent incorrect results.

## Run

```bash
gcc -o a ask03_server.c -lpthread
./a
gcc -o b ask03_client.c
./b
```

## Results

Below, you'll find snapshots of the terminal from different code executions. It is observed that the calculations are performed correctly, and the expected results are printed. It is also noted that the server can simultaneously accommodate multiple clients.

![image](https://github.com/PaolaVlsc/ServerClient_UnixDomain_StreamSockets/assets/87998374/0d8972ac-dde2-43be-bde3-4c79ce4f7201)

![image](https://github.com/PaolaVlsc/ServerClient_UnixDomain_StreamSockets/assets/87998374/e9765e66-275c-4508-a710-86225888818f)

![image](https://github.com/PaolaVlsc/ServerClient_UnixDomain_StreamSockets/assets/87998374/64ecf889-26b7-43b1-9f2c-fe310f2aca90)


## Author

This project was written by Velasco Paola for the subject Operating Systems II final project. 2020-21


## Extras 
[Report paper in greek](https://github.com/PaolaVlsc/ServerClient_UnixDomain_StreamSockets/blob/main/extras/cs161020_%CE%92%CE%95%CE%9B%CE%91%CE%A3%CE%9A%CE%9F%CE%A0%CE%91%CE%9F%CE%9B%CE%91_%CE%9B%CE%A3%CE%99%CE%99_%CE%95%CE%A1%CE%93%CE%91%CE%A3%CE%99%CE%9102.pdf)
