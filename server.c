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

int cwnd = 1;
int ssthresh = 8;
int last_acked = 1;

void server_send(int client_fd) {
    if(cwnd < ssthresh){
        printf("State: slow start (cwnd = %d, ssthresh = %d)\n", cwnd, ssthresh);
    } else{
        printf("State: congestion avoidance (cwnd = %d, ssthresh = %d)\n", cwnd, ssthresh);
    }

    for (int i = 0;i<cwnd;i++) {
        Segment seg;
        seg.seq_num = last_acked + i;
        seg.loss = packet_loss();
        printf("Send: seq_num = %u\n", seg.seq_num);
        send(client_fd, &seg, sizeof(seg), 0);
    }
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

bool is_duplicate = false;
int duplicate_acks = 0;

void server_receive(int client_fd) {

    int prev_ack;
    for(int i = 0;i<cwnd;i++){
        Segment seg;
        ssize_t bytes_received = recv(client_fd, &seg, sizeof(seg), 0);
        printf("ACK: ack_num = %u\n", seg.ack_num);
        if(i == cwnd - 1){
            last_acked = seg.ack_num;
        }
        if(seg.ack_num == prev_ack){
            duplicate_acks++;
            if(duplicate_acks == 2){
                is_duplicate = true;
                printf("3 duplicate ACKs : ACK_num = %d, ssthresh = %d\n", prev_ack, ssthresh);
            }
        } else{
            duplicate_acks = 0;
        }
        prev_ack = seg.ack_num;
    }

    if(is_duplicate == true){
        is_duplicate = false;
        ssthresh = cwnd / 2;
        cwnd = 1;
    } else if(cwnd < ssthresh){
        cwnd *= 2;
    } else{
        cwnd += 1;
    }
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

    for(int i = 0;i<15;i++){
        server_send(clientfd);
        server_receive(clientfd);
    }

    // Close the socket.
    close(clientfd);
    close(sockfd);

    return 0;
}