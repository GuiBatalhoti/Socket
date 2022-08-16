/*
 * This is implementation of a socket from the client point of view.
 */

//Including the libraries
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//defining constants
#define PORT 4000

//===============================================================================================================================
//Error function
void error(const char* msn){
    /*
     * Function to display the error message that the program has got.
     * And then exit the program
     */

    perror(msn); //this function maps the number of the error
    exit(1);
}

//===============================================================================================================================
//Main function

int main() {

    // the server socket end the port
    int server_socket, port;

    // to verify if the sent message was sent
    int send_return;

    //server IP address
    struct sockaddr_in server_address;

    //IP address to connect
    char IP[15];

    //the host server
    struct hostent *server;

    //the messages sent and received
    char buffer[256], response[256];

    //entering the port to connect
    printf("Enter the port to connect: ");
    scanf("%i", &port);

    //entering the IP address to connect
    printf("Enter the IP address to connect: ");
    std::cin >> IP;

    //taking the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        error("Error opening the socket");
    }

    //taking the server address
    server = gethostbyname((char *) IP);
    if (server == NULL){
        error("No such host.");
    }

    //making sure tha the address ins empty
    bzero((char *) &server_address, sizeof server_address);

    //configuring the server address
    server_address.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port);

    //connecting the client socket to the server
    if (connect(server_socket, (struct sockaddr*) &server_address, sizeof server_address) < 0){
        error("Connection failed");
    }

    //infinity loop for entering the client input
    while (true){
        //making the buffer empty
        bzero(buffer, sizeof buffer);

        //taking the client input
        printf("> ");
        std::cin.getline(buffer, sizeof buffer);

        //sending the message to the server and verifying if it was sent
        send_return = send(server_socket, buffer, sizeof buffer, MSG_WAITALL);
        if (send_return < 0){
            error("Error writing the data.");
        }
        printf("Message sent...\n");

        //taking the server response for the message sent
        read(server_socket, response, sizeof response);
        printf("Server says: %s \n\n", response);
        bzero(response, sizeof response); //making the response is empty after receiving

        //if the client said "Bye" end the loop to disconnect
        int aux = strncmp("Bye", buffer, 3);
        if (aux == 0) {
            break;
        }
    }

    //closing the socket to disconnect
    close(server_socket);
    printf("\nDisconnected...\n");

    return 0;
}
