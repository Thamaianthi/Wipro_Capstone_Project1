//client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <semaphore.h>

#define PORT 8080
#define SHM_NAME "/shm_random_numbers"
#define SHM_SIZE sizeof(int)

int main() {
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Open shared memory
    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return -1;
    }

    // Map shared memory
    int *shared_memory = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    // Open semaphore
    sem_t *semaphore = sem_open("/semaphore", 0);
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        return -1;
    }

    while (1) {
        sem_wait(semaphore);
        int random_number = *shared_memory;
        sem_post(semaphore);
        printf("Received Number: %d\n", random_number);
        sleep(1);
    }

    // Cleanup
    sem_close(semaphore);
    munmap(shared_memory, SHM_SIZE);
    close(shm_fd);
    close(sock);

    return 0;
}