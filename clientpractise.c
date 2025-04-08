#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int main()
{
    int client_addr;
    struct sockaddr_in address;
    client_addr=socket(AF_INET,SOCK_STREAM,0);
    char buffer={'h','e','l','l','o'};

    address.sin_family=AF_INET;
    address.sin_port=htons(8080);


    int binding=inet_pton(AF_INET,"127.0.0.1",&address.sin_addr.s_addr);


    int server_addr=connect(client_addr,(struct sockaddr *)&address,sizeof(address));

    send(client_addr,buffer,strlen(buffer),0);

}