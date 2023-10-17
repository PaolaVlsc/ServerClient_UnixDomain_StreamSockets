/*  ;; ----------------------------------------------------------------
    ;;                   ΒΕΛΑΣΚΟ ΠΑΟΛΑ      cs161020
    ;; ----------------------------------------------------------------
    ;;           Τμήμα Μηχανικών Πληροφορικής και Υπολογιστών
    ;;               Εργαστήριο ΛΣΙΙ 2020/21 - Εργασία 3.2: 
    ;;                  THREADS, SEMAPHORES, SOCKETS
    ;; ----------------------------------------------------------------
    ;;  Υπεύθυνος μαθήματος: ΜΑΜΑΛΗΣ ΒΑΣΙΛΕΙΟΣ 
    ;;  Καθηγητές: ΙΟΡΔΑΝΑΚΗΣ ΜΙΧΑΛΗΣ
    ;; ----------------------------------------------------------------
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SOCK_PATH "socket"

void initializeArray(int arrayClient[], int arrayA_size); // function to initialize array by client
int userChoice(int choice);                               // function for client to choose

int main()
{
    int socketfd, t;
    struct sockaddr_un server;

    int choice, array_size, i;
    int *arrayClient;

    char server_response_received[50];
    float average;

    // Create socket
    if ((socketfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Server
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, SOCK_PATH);

    // Connect client to server - connect()
    if (connect(socketfd, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) == -1)
    {
        perror("Error connection to server");
        exit(1);
    }

    // Program start print
    printf("Client connected to server\n");

    choice = 0;
    do
    {
        // Μέγεθος πίνακα
        printf("Give size of array: ");
        do
        {
            scanf("%d", &array_size);
            if (array_size < 0)
            {
                printf("Array size cannot be negative!\n");
                printf("Give array size again: ");
            }
        } while (array_size < 0);

        // send the size of array to server
        send(socketfd, &array_size, sizeof(int), 0);

        // allocate memory for arrayClient
        arrayClient = (int *)malloc(sizeof(int) * array_size);

        // Check if malloc was successfully completed
        if (arrayClient == NULL)
        {
            printf("Error: Not available memory\n");
            exit(EXIT_FAILURE);
        }

        // Αρχικοποίηση του πίνακα
        initializeArray(arrayClient, array_size);

        // Send the data of arrayClient to server
        send(socketfd, arrayClient, sizeof(int) * array_size, 0);

        // Receive server's response
        t = recv(socketfd, server_response_received, 50, 0);
        server_response_received[t] = '\0';
        recv(socketfd, &average, sizeof(float), 0);

        printf("\nAverage calculated by server: %f\n", average);
        printf("Server Response: %s\n", server_response_received);

        // Exit or continue?
        choice = userChoice(choice);

        // send to server client's response
        send(socketfd, &choice, sizeof(choice), 0);
    } while (choice != 2);
}

// Συνάρτηση: αρχικοποίηση πίνακα
void initializeArray(int arrayClient[], int arrayA_size)
{
    int i;
    printf("\nGive numbers for array\n");
    for (i = 0; i < arrayA_size; i++)
    {
        printf("arrayA[%d]: ", i);
        scanf("%d", &arrayClient[i]);
    }
}

// Συνάρτηση: επιλογή χρήστη-πελάτη
int userChoice(int choice)
{
    while (1)
    {
        printf("\nWould you like to give new data?\n");
        printf("1.) Enter new data \n");
        printf("2.) Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {

        case 1:
            printf("\nChoice 1\n\n");
            return 1;
            break;

        case 2:
            printf("\nExit!\n");
            return 2;
            break;
        default:
            printf("\nThis is not a valid option. Please Select Another\n\n");
            break;
        }
    }
}