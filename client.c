#include "header.h"

/*
 * @description
 * Task1
 * 1. create a socket and connect to server.
 *    (server's IP address = "127.0.0.1")
 *    (server's Port number = 45525)
 * 2. Receive 1 message from server and print it out.
 *    (The message you sent from server)
 * 
 * Task2
 * Continuously receive data from Server and send back ACK.
*/

#define BUFFER_SIZE 1024

int main(int argc , char *argv[]){   
    int sockfd, clientfd;
    struct sockaddr_in server_addr, client_addr;
    char recv_msg[BUFFER_SIZE];

    //Create socket.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Set up server's address.
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(45525);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Connect to server's socket.
    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    //Receive message and print it out.
    ssize_t bytes_received = recv(sockfd, recv_msg, sizeof(recv_msg), 0);
    recv_msg[bytes_received] = '\0';
    printf("%s", recv_msg);

    // Receive data and send ACK.
    bool isPacketLoss = false;
    int rcv_data;
    int ack = 0;
    int packetLossNum;

    while(1){
        ssize_t data_received = recv(sockfd, &rcv_data, sizeof(rcv_data), 0);  
        if(rcv_data == 7 || rcv_data == 12){
            // packet_loss()
            if(isPacketLoss == false){
                packetLossNum = rcv_data;
                isPacketLoss = true;
                send(sockfd, &rcv_data, sizeof(rcv_data), 0);   
            } else{
                send(sockfd, &packetLossNum, sizeof(packetLossNum), 0);
            }        
            printf("Loss: seq_num: %d\n", rcv_data);   
        } else{
            ack = rcv_data + 1;
            printf("Received: seq_num = %d\n", rcv_data);
            if(isPacketLoss == true){
                send(sockfd, &packetLossNum, sizeof(packetLossNum), 0);
            } else{
                send(sockfd, &ack, sizeof(ack), 0);
            }
        }
    }

    // Close the socket
    close(sockfd);
    return 0;
}