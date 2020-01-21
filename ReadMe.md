# Chat Room
This chat room is build with in 2 parts:
Client side using C++ and
Server side using Python

## Server
This server connects to the local SQL database first, and starts listening to new clients.
Everytime one client is connected, one more thread will be created to receive, process and send messages to this specific client.

## Client
This client has one thread for sending message to the server and another thread for receiving message from server.
A TCP/IP class was built based on TCP/IP Socket for easier use.
