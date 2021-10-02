#include <iostream>
#include <sys/socket.h>

#include <unistd.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>
using namespace std ;
int main() {

    //creat a socket
    int listening = socket(AF_INET , SOCK_STREAM , 0) ;

    if(listening == -1)
    {
        cerr << "Can't create a socket! "<< endl  ;
        return  -1;
    }
    // bind the socket to ip/ port

    sockaddr_in hint ;
    hint.sin_family = AF_INET ;
    hint.sin_port = htons(54000) ;
    inet_pton(AF_INET, "0.0.0.0" , &hint.sin_addr) ;

    if((bind(listening,  (sockaddr *)(&hint), sizeof(hint))) == -1)
    {
        cerr << "Can't bind to IP/Port" << endl ;
        return -2 ;
    }
    // Mark the socket for listening
    if(listen(listening , SOMAXCONN )== -1)

    {
        cerr <<"Can't listen";
        return -3 ;
    }
    // Accept a call
    sockaddr_in client ;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST] ;
    char srv[NI_MAXSERV] ;
    int clientSocket = accept(listening,
                              (sockaddr *)(&client) ,
                              &clientSize) ;
    if(clientSocket == -1){
        cerr << "Problem with Client connecting!" ;
        return -4 ;
    }

    close(listening);

    memset(host , 0 , NI_MAXHOST) ;
    memset(srv , 0 , NI_MAXSERV)  ;

    int result = getnameinfo((sockaddr*)&client , sizeof(client), host , NI_MAXHOST, srv , NI_MAXSERV, 0);

    if(result)
    {
        cout << host << " connected on " << srv << endl ;
    }
    else
     {
        inet_ntop(AF_INET , &client.sin_addr , host , NI_MAXHOST) ;
        cout << host << " connected on" << ntohs(client.sin_port) << endl ;
     }
    // display
    char buf[4096] ;
    while(true)
    {
        // clear buf
        memset(buf , 0 , 4096) ;
        // wait for message

        int bytesRes = recv(clientSocket , buf, 4096 , 0) ;
        if(bytesRes == 0)
        {
            cout << "The client disconnected" << endl ;
            break ;
        }

        // display message
        cout << "Received: " << string(buf , 0 , bytesRes ) << endl ;
        // resend message
        send(clientSocket , buf , bytesRes +1 , 0) ;
    }
    close(clientSocket) ;






    return 0;
}
