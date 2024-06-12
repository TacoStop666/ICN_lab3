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
    bool is_packet_loss = false;
    int packet_loss_num;

    while(1){
        Segment seg;
        ssize_t data_received = recv(sockfd, &seg, sizeof(seg), 0);  
        if(seg.loss){
            if(is_packet_loss){
                seg.ack_num = packet_loss_num;              
            } else{
                is_packet_loss = true;
                packet_loss_num = seg.seq_num;
                seg.ack_num = seg.seq_num;
            }
            printf("Loss: seq_num = %u\n", seg.seq_num);
            send(sockfd, &seg, sizeof(seg), 0);
        } else{
            if(is_packet_loss){
                if(packet_loss_num == seg.seq_num){
                    is_packet_loss = false;
                    seg.ack_num = seg.seq_num + 1;                    
                } else{
                    seg.ack_num = packet_loss_num;
                }
                seg.ack_num = packet_loss_num;
            } else{
                seg.ack_num = seg.seq_num + 1;
            }
            printf("Received: seq_num = %u\n", seg.seq_num);
            send(sockfd, &seg, sizeof(seg), 0);
        }
    }

    // Close the socket
    close(sockfd);
    return 0;
}