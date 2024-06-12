#include "header.h"

#define CWND 4
#define SSTHRESH 8
#define DATA_SIZE 1024
#define ROUND 10

/*
 * @description
 * Write your server's send function here.
 * 
 * 1. Send cwnd sequences of data start with correct sequence number.
 * 2. Simulate packet loss.
 * 
 * @param
 * You can increase or decrease parameters by yourself.
 * Some useful information can be used either as global varaible or passed as parameters to functions:
 * 1. client_fd: The socket descriptor used for sending/receiving data  
 *               with a client that has connected to server.
 * 2. last_acked: Previous acknowledge packets.
 * 3. cwnd: congestion window size
*/

extern bool packet_loss();

// *last_acked = 2
void server_send(int client_fd, int *cwnd, int *ssthresh, int *last_acked) {
    int data = *last_acked; 

    if(*cwnd < *ssthresh){
        printf("State: slow start (cwnd = %d), ssthresh = %d\n", *cwnd, *ssthresh);
    } else{
        printf("State: congestion avoidance (cwnd = %d), ssthresh = %d\n", *cwnd, *ssthresh);
    }

    for (int i = 0; i < *cwnd; ++i) {
        if (!packet_loss()) {
            data++; 
            printf("Send: seq_num = %d\n", data);
            send(client_fd, &data, sizeof(data), 0); 
        } else {
            printf("Packet loss simulated for sequence %d\n", data + 1); 
            data++; 
        }
    }

    *last_acked = data;
}

/*
 * @description
 * Write your server's receive function here.
 * ** We don't need to consider time-out in this lab. 
 *    loss will only occur when packet_loss() = true. **
 * 1. Receive ACKs from client.
 * 2. Detect if 3 duplicate ACK occurs.
 * 3. Update cwnd and ssthresh
 * 
 *
 * @param
 * You can increase or decrease parameters by yourself.
 * Some useful information can be used either as global varaible or passed as parameters to functions:
 * 1. client_fd: The socket descriptor used for sending/receiving data  
 *               with a client that has connected to server.
 * 2. last_acked: Previous acknowledge packets.
 * 3. ssthresh: slow start threshold
 * 4. cwnd: congestion window size
*/
void server_receive(int client_fd, int *cwnd, int *ssthresh, int *last_acked) {
    int ack, duplicate_acks = 0;
    int prev_ack = -1;

    for (int i = 0; i < *cwnd; ++i) {
        ssize_t bytes_received = recv(client_fd, &ack, sizeof(ack), 0);
        if (bytes_received <= 0) {
            perror("Receive failed");
            return;
        }
        printf("ACK: ack_num = %d\n", ack + 1);

        if(ack == prev_ack){
            duplicate_acks++;
            if(duplicate_acks == 3){
                printf("3 duplicate ACKs : ACK_num = %d, ssthresh = %d\n", ack, *ssthresh);
            }
        } else{
            duplicate_acks = 0;
        }

        prev_ack = ack;
        *last_acked = ack;
    }
    *cwnd  = *cwnd * 2;
}

/*
 * @description
 * Task1
 * 1. Create a TCP socket bind to port 45525.
 * 2. Listen the TCP socket.
 * 3. Accept the connect and get the Client socket file descriptor
 * 4. Send 1 message "Hi I'm server {your_student_id}" to client
 * 5. Go finish the client.c TASK 1
 *
 * Task2:
 * Start data transmission with simulating congestion control.
 *
 * To be simple, we receive ACKs only after all cwnd sequences of data are sent, 
 * so we cannot react to 3-duplicate ACKs immediately. This is ok for this lab.
 * Ex: 
 *      while(ROUND--) {
 *          server_send();
 *          server_receive();
 *      }
*/
int main(int argc, char* argv[]) {
    int sockfd, clientfd, len;
    struct sockaddr_in server_addr, client_addr;
    char message[] = "Hi, I'm server 111062335...\n";

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up server's address.
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(45525); // port number
    server_addr.sin_addr.s_addr = INADDR_ANY; // bind to any available IP address

    //Bind socket to the address.
    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    //Listening the socket.
    listen(sockfd, 5);

    //Accept the connect request.
    len = sizeof(client_addr);
    clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);

    printf("New connection\n");

    // Send 1 message to client.
    send(clientfd, message, strlen(message), 0);

    // Start congestion control
    int cwnd = CWND;
    int ssthresh = SSTHRESH;
    int last_acked = 2;

    for(int i = 0;i<2;i++){
        server_send(clientfd, &cwnd, &ssthresh, &last_acked);
        server_receive(clientfd, &cwnd, &ssthresh, &last_acked);
    }

    // Close the socket.
    close(clientfd);
    close(sockfd);

    return 0;
}