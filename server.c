#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_USERS 10
#define MAX_MESSAGE_SIZE 256

struct Client {
    int socket;
    char username[50];
};

struct Client clients[MAX_USERS];
int numClients = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handleClient(void *arg) {
    struct Client *client = (struct Client *)arg;
    char message[MAX_MESSAGE_SIZE];

    while (1) {
        int bytesReceived = recv(client->socket, message, sizeof(message), 0);
        if (bytesReceived <= 0) {
            // Client disconnected
            printf("%s has left the chat.\n", client->username);

            // Remove the client from the list
            pthread_mutex_lock(&mutex);
            for (int i = 0; i < numClients; i++) {
                if (clients[i].socket == client->socket) {
                    for (int j = i; j < numClients - 1; j++) {
                        clients[j] = clients[j + 1];
                    }
                    numClients--;
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);

            close(client->socket);
            return NULL;
        }

        // Broadcast the message to all connected clients
        message[bytesReceived] = '\0';
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < numClients; i++) {
            if (clients[i].socket != client->socket) {
                send(clients[i].socket, message, strlen(message), 0);
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating server socket");
        exit(1);
    }

    struct sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding server socket");
        close(serverSocket);
        exit(1);
    }

    if (listen(serverSocket, 5) == -1) {
        perror("Error listening on server socket");
        close(serverSocket);
        exit(1);
    }

    printf("Server listening on port 12345...\n");

    while (1) {
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("Error accepting connection");
            close(serverSocket);
            exit(1);
        }

        struct Client newClient;
        newClient.socket = clientSocket;
        recv(clientSocket, newClient.username, sizeof(newClient.username), 0);

        pthread_t tid;
        pthread_create(&tid, NULL, handleClient, &newClient);

        // Add the client to the list
        pthread_mutex_lock(&mutex);
        if (numClients < MAX_USERS) {
            clients[numClients] = newClient;
            numClients++;
        }
        pthread_mutex_unlock(&mutex);
        printf("%s has joined the chat.\n", newClient.username);
    }

    close(serverSocket);
    return 0;
}
