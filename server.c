#include "header.h"
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
void server_send() {}

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
void server_receive() {}

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
    // Create TCP socket

    // Set up server's address.

    //Bind socket to the address.
    
    //Listening the socket.
    
    //Accept the connect request.

    // Send 1 message to client.

    // Start congestion control

    // Close the socket.
    return 0;
}