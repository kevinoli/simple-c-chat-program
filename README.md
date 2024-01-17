# Chat Program in C

This is a project for CS 457 Fall 2023 at Colorado State University. The goal of this project is to implement a simple chat program in the C programming language using socket programming. The program allows communication between a client and a server, with messages being sent back and forth. Below are the details and requirements for this project.

## Introduction

In this project, you will be using socket programming in C to create a basic chat program. It's recommended that you refer to [Beej's Guide to Network Programming - Using Internet Sockets](https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf) for guidance on socket programming.

## Assignment Overview

### Program Structure

This assignment consists of a single program that serves as both the client and the server. The program facilitates communication by exchanging simple messages between the client and the server. Here's how it works:

1. The client initiates a connection request to the server.
2. Messages are exchanged in a back-and-forth manner.
3. Messages can be up to 140 characters in length, and the program should handle validation. If a user inputs a message longer than 140 characters, an error message should be displayed, and the user should be prompted to enter a new message.

### Program Invocation

The program can be invoked as follows:

- Server-side:
```
$ ./chat
```

- Client-side:
```
$ ./chat -p 3790 -s 129.82.44.141
```
or
```
$ ./chat -s 129.82.44.141 -p 3790
```

The `-p` flag specifies the port to connect to, and the `-s` flag specifies the IP address of the server.

- Help Command:
```
$ ./chat -h
```
This should display a help message and exit.

### Server Responsibilities

1. Set up a TCP port and listen for incoming connections, displaying the IP and PORT it is listening on.
2. Accept incoming connections from clients.
3. Receive messages from clients and print them to the screen.
4. Prompt the user for a message to send.
5. Send the message to the client.
6. Return to step 3.

### Client Responsibilities

1. Set up a TCP connection to the specified server's IP and port.
2. Prompt the user for a message to send.
3. Send the message to the server.
4. Receive messages from the server and print them to the screen.
5. Return to step 2.

### Makefile

Make sure to use the provided `makefile` to compile your code into an executable called `chat`. Do not modify the makefile.

### Interaction Requirements

To facilitate testing with other teams' clients and servers, follow these packet format specifications:

- Version (16 Bits, Binary)
- String Length (16 Bits, Binary)
- Message (Up to 140 Bytes, ASCII)

The version is always 457, the string length represents the message length in bytes, and the message follows immediately afterward. Ensure proper byte order (Big-Endian) for packets sent over the network.

## Grading

- No credit is given for code that fails to compile on the department's Linux machines.
- 10%: Accept input flags in any order and perform sanity checks.
- 10%: Display an error and prompt for a new message if the message exceeds 140 characters.
- 10%: Properly use network-to-host and host-to-network conversions.
- 10%: Ensure your program works with another team's randomly chosen client and server.
- 50%: Pass send/receive test cases.
- 10%: Output formatting should match the provided example.

## Points and Submission

This project is worth 100 points, and it contributes to 10% of your total grade. To submit your assignment:

- Turn in your assignment by submitting a tarball (GroupName) on CANVAS. Ensure that the files are at the root of the archive and not in any folders. GTAs will evaluate the project using the provided makefile.
