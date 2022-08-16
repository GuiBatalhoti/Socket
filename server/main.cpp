/*
 * This is implementation of a socket from the server point of view.
 */

//Including the libraries
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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

    //Server stander messages:
    const char entrance_message[] = "Welcome to the server!!! \n" //For a new client connection
                                    "Enter 'Bye' to disconnect...\n";
    const char received_message[] = "Received message"; //For client messages received

    int port; //the port of the server and it's input
    printf("Type the port of the server: ");
    scanf("%d", &port);

    // Message buffer for receiving messages
    char buffer[256];

    //the sockets that the server has
    int server_socket, client_socket;

    //for verifying the return of read() ou recev()
    int read_return;

    // struct of the address of server and client
    struct sockaddr_in server_address, client_address;

    // the size of client socket
    socklen_t client_len;

    //creating the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        error("Error opening the socket.\n");
    }
    printf("Socket opened.\n");


    //making sure that the server address is empty
    bzero( (char *) &server_address, sizeof (server_address));

    //putting the values to the server socket, the server is int the "localhost"
    server_address.sin_family = AF_INET; //IPv4
    server_address.sin_addr.s_addr = INADDR_ANY; //any address can connect
    server_address.sin_port = htons(port); //the port of the server

    //binding the server socket to the address specified
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof server_address) < 0 ){
        error("Binding server to IP failed.\n");
    }
    printf("Socket bound to IP: %s .\n", "127.0.0.1");

    //listening to any connection attempt
    listen(server_socket, 5);

    printf("Waiting for connection...\n");

    //the size of client address
    client_len = sizeof (client_address);

    //accept the client connection
    client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_len);
    if ( client_socket < 0){
        error("Error on accept.");
    }

    printf("Connection initialized.\n\n");

    //send the entrance message
    send(client_socket, entrance_message, sizeof entrance_message, MSG_WAITALL);

    //just to split the terminal
    printf("====================\n\n");

    //loop for receiving messages from client
    while (true){
        //clearing the buffer message
        bzero(buffer, sizeof buffer);

        //reading the message
        read_return = read(client_socket, buffer, sizeof buffer);
        if (read_return < 0){
            error("Error reading. \n");
        }
        printf("Client says: %s \n", buffer); //print the client message

        //send to client that the message was received
        send(client_socket, received_message, sizeof received_message, 0);

        //verifying if the client said "Bye" to disconnected
        int aux = strncmp("Bye", buffer, 3);
        if (aux == 0) {
            break;
        }
    }

    //closing the sockets
    printf("\n\nClient disconnected...\n");
    close(client_socket);
    close(server_socket);
    printf("Server closed.\n");

    return 0;
}
