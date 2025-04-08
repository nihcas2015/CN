#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main()
{
    int server_fd,client_addr;
    struct sockaddr_in address;
    server_fd=socket(AF_INET,SOCK_STREAM,0);


    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(8080);

    int binding=bind(server_fd,(struct sockaddr *)&address,sizeof(address));

    if(binding<0)
    {
        perror("Binding is not worked");

    }

    listen(server_fd,3);

    while(1)
    {
        client_addr=accept(server_fd,(struct sockaddr *)&address,sizeof(address));
        

    }



    return 0;

}