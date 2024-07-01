# TCP Client-Server Calculator with Data Integrity

## Description
This project demonstrates a TCP client-server model where the server acts as a calculator. The client sends two numbers and an arithmetic operation (add, subtract, multiply, divide, modulus) to the server, which calculates the result and returns it to the client. The server stays active until the client sends a stop command. A hash function is used to ensure data integrity.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Usage](#usage)
  - [Server Side](#server-side)
  - [Client Side](#client-side)
- [Implementation Details](#implementation-details)
- [Data Integrity](#data-integrity)
- [Conclusion](#conclusion)
- [License](#license)

## Introduction
This project showcases a simple TCP client-server application implemented in C. The server functions as a calculator, performing various arithmetic operations based on the client's input. The server also incorporates a hash function to verify the integrity of the data exchanged between the client and server.

## Features
- Perform basic arithmetic operations (add, subtract, multiply, divide, modulus)
- Continuous server operation until client termination
- Data integrity check using a hash function

## Requirements
- GCC compiler for C
- Basic knowledge of TCP/IP networking
- Basic understanding of C programming

## Usage

### Server Side
1. Compile the server code:
    ```sh
    gcc -o server server.c
    ```
2. Run the server:
    ```sh
    ./server
    ```

### Client Side
1. Compile the client code:
    ```sh
    gcc -o client client.c
    ```
2. Run the client:
    ```sh
    ./client
    ```

## Implementation Details
### Server Code
The server listens for incoming client connections and performs arithmetic operations based on the client's request. The server remains active until explicitly terminated by the client.

### Client Code
The client connects to the server, sends two numbers and an arithmetic operation, receives the result, and displays it. The client can terminate the server by sending a stop command.

### Example Usage
**Client Side:**
```sh
Enter two numbers: 2, 3
Enter operation: modulus
Output: 2
