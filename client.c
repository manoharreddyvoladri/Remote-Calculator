#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>


 

#define BUFLEN 512
#define SERVER "127.0.0.1"
#define PORT 8888

 

void error(char *msg) {
    perror(msg);
    exit(1);
}

 

int main(void) {
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char buf[BUFLEN];

 

    // Create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        error("socket");
    }

 

    // Set server address and port
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

 

    // Get input from user
    int num1, num2;
    char op;
    printf("Enter first number: ");
    scanf("%d", &num1);
    printf("Enter operator (+, -, *, /, %%): ");
    scanf(" %c", &op);
    printf("Enter second number: ");
    scanf("%d", &num2);

 

    // Send input to server
    sprintf(buf, "%d %c %d", num1, op, num2);
    if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &si_other, slen) == -1) {
        error("sendto");
    }

 

    // Receive result from server
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1) {
        error("recvfrom");
    }

 

    // Print result
    printf("%d %c %d = %s\n", num1, op, num2, buf);

 

    close(s);
    return 0;
}
