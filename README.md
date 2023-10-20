# ChatApplication_in_C

The Simple Chat Application in C is a basic implementation of a text-based chat system. It includes a server and client, allowing multiple users to connect and engage in real-time text communication.

## Features

- **Multiple Connections:** The server supports multiple client connections concurrently.
- **User Authentication:** Clients provide a username upon connecting to the server.
- **Real-time Messaging:** Users can send and receive messages in real-time, broadcasted to all connected clients.
- **Dynamic User Management:** Users can join and leave the chat dynamically, and the server adjusts accordingly.
- **Simplified Interface:** The user interface is command-line based for simplicity.
- **Multithreading:** Multithreading is used to handle multiple clients simultaneously.

## Getting Started

### Prerequisites

- C compiler (e.g., GCC)
- Git

### Running the Server

1. Compile the server code:

   gcc server.c -o server -pthread
2. Run the server:
  ./server
### Running the Client

1. Compile the client code:
  gcc client.c -o client
2. Run the client:
   ./client

