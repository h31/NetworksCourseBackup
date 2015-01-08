#include "serf.h"


void cnect()
{
    int port;
    char adr[15];
    
    struct sockaddr_in peer;
    int s;
    int rc;
    char buf[ 1 ];
    
    printf("port(default 8888): ");
    scanf("%d",  port);
    printf("addr(default 127.0.0.1): ");
    scanf("%s",  adr);
    if(port == NULL) {port = 8888;}
    if(adr == NULL) {adr = "127.0.0.1";}
    
    peer.sin_family = AF_INET;
    peer.sin_port = htons( port );
    peer.sin_addr.s_addr = inet_addr( adr ); 
    
    s = socket( AF_INET, SOCK_STREAM, 0 );
    if ( s < 0 ) {perror( "socket call failed" );  return 1; }
     
    rc = connect( s, ( struct sockaddr * )&peer, sizeof( peer ) );
    if ( rc ) {perror( "connect call failed" ); return 1;}

    printf("you are connected!\n");


}