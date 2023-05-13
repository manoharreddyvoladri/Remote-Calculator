#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>


 

#define BUFLEN 512
#define PORT 8888

 

void error(char *msg) {
    perror(msg);
    exit(1);
}

 

int main(void) {
    struct sockaddr_in si_me, si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];

 

    // Create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        error("socket");
    }

 

    // Bind the socket to PORT
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1) {
        error("bind");
    }

 

    // Wait for messages from clients and perform calculations
    while(1) {
        printf("Waiting for data...\n");
        fflush(stdout);

 

        // Receive message from client
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1) {
            error("recvfrom");
        }

 

        // Parse message to obtain two integers and an operator
        int num1, num2;
        char op;
        if (sscanf(buf, "%d %c %d", &num1, &op, &num2) != 3) {
            printf("Invalid message format: %s\n", buf);
            continue;
        }

 

        // Calculate the result of the operation
        int result;
        switch(op) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 == 0) {
                    printf("Division by zero\n");
                    continue;
                }
                result = num1 / num2;
                break;
            case '%':
                result = num1 % num2;
                break;
            default:
                printf("Invalid operator: %c\n", op);
                continue;
        }

 

        // Send the result back to the client
        sprintf(buf, "%d", result);
        if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &si_other, slen) == -1) {
            error("sendto");
        }
    }

 

    close(s);
    return 0;
}
