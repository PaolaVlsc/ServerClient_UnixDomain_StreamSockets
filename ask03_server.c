/*  ;; ----------------------------------------------------------------
    ;;                   ΒΕΛΑΣΚΟ ΠΑΟΛΑ      cs161020
    ;; ----------------------------------------------------------------
    ;;           Τμήμα Μηχανικών Πληροφορικής και Υπολογιστών
    ;;               Εργαστήριο ΛΣΙΙ 2020/21 - Εργασία 3.1: 
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
#include <pthread.h>
#include <semaphore.h>

#define SOCK_PATH "socket"

int correct_sequence; // Number of correct checks that the avg > 10
int numberOfRequests; // Number of total requests (Whether sequence is ok or fail)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *checkAverage(void *arg); // Συνάρτηση που θα εκτελέσει κάθε νήμα

int main()
{
    int socketfd, newsocketfd, t, i, len;
    struct sockaddr_un local, remote;
    pthread_t thread[50]; // max 50 threads

    // Create socket
    if ((socketfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);

    // bind
    if (bind(socketfd, (struct sockaddr *)&local, len) == -1)
    {
        perror("bind");
        exit(1);
    }

    // listen / waiting for client
    if (listen(socketfd, 5) == -1)
    {
        perror("listen");
        exit(1);
    }

    // Initializations
    i = 0;
    correct_sequence = 0;

    for (;;)
    {
        printf("Waiting for connection...\n");
        t = sizeof(remote);
        // Accept client connection on socket.
        if ((newsocketfd = accept(socketfd, (struct sockaddr *)&remote, &t)) == -1)
        {
            perror("accept");
            exit(1);
        }
        printf("Client %d has connected.\n", i + 1);
        //create thread
        pthread_create(&thread[i++], NULL, (void *)&checkAverage, (void *)&newsocketfd);
    }

    // Απελευθέρωση πόρου
    pthread_mutex_destroy(&mutex);

    return 0;
}

void *checkAverage(void *arg)
{
    int sock = *(int *)arg;
    int choice, i, flag;

    char server_response[100];

    int array_size, *array_received;
    float avg;

    choice = 0;

    do
    {
        int sum = 0;

        // receive array size from client
        recv(sock, &array_size, sizeof(int), 0);

        // allocate memory for array_received
        array_received = (int *)malloc(array_size * sizeof(int));

        // Check if malloc was successfully completed
        if (array_received == NULL)
        {
            printf("Error: Not available memory\n");
            exit(EXIT_FAILURE);
        }

        // receive array data from client
        recv(sock, array_received, sizeof(int) * array_size, 0);

        // sum
        for (i = 0; i < array_size; i++)
        {
            sum += array_received[i];
        }

        // average
        avg = sum / array_size;
        //printf("Average = %f", avg);

        // flag for correct sequences
        if (avg > 10)
            flag = 0;
        else
            flag = 1;

        if (flag == 0) // If sequence was correct
        {
            strcpy(server_response, "Sequence Correct");
            // Έλεγχος και δημιουργία συγχρονισμού με mutex
            if (pthread_mutex_lock(&mutex))
            {
                perror("mutex_lock");
                exit(3);
            }
            correct_sequence++;
            numberOfRequests++;
            // Έλεγχος επιτυχούς τερματισμού συγχρονισμού των mutex
            if (pthread_mutex_unlock(&mutex))
            {
                perror("pthread_mutex_unlock() error");
                exit(4);
            }
        }
        else if (flag == 1) // If sequence is not correct
        {
            strcpy(server_response, "Check Failed");
            // Έλεγχος και δημιουργία συγχρονισμού με mutex
            if (pthread_mutex_lock(&mutex))
            {
                perror("mutex_lock");
                exit(3);
            }
            numberOfRequests++;
            // Έλεγχος επιτυχούς τερματισμού συγχρονισμού των mutex
            if (pthread_mutex_unlock(&mutex))
            {
                perror("pthread_mutex_unlock() error");
                exit(4);
            }
        }
        else
            strcpy(server_response, "Failed to check (other)");

        send(sock, server_response, 50, 0);
        send(sock, &avg, sizeof(float), 0);

        // Response of client if he wants to continue
        recv(sock, &choice, sizeof(int), 0);

        printf("Correct Sequences checks : %d\n", correct_sequence);
        printf("Number of total requests : %d\n", numberOfRequests);
    } while (!choice);

    close(sock); // Close socket connection
    pthread_exit(NULL);
}