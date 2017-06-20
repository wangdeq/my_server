#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>

#define ser_addr "2001::3"
#define port 9521
#define MAX_PAYLOAD 1024 /* maximum payload size*/
#define NETLINK_TEST 21
#define samp_itvl 5

struct sockaddr_nl src_addr, dest_addr ;
struct nlmsghdr *nlh = NULL ;
struct iovec iov ;
int sock_fd ;
struct msghdr msg ;
char my_policy ;
char policy_ans = 'n' ;
char tmp [100] ;
int flag = 0 ;
int flag_pipes[2] ;
int msgs_pipes[2] ;

int get_flag () ;
int change_flag ( int num ) ;
char get_policy ( void ) ;
char set_policy ( void ) ;
void net_link_init ( void ) ;
char mysend_msg ( int sock_fd , struct msghdr msg , const char my_policy ) ;
void mysend_ans ( int sock_fd , struct msghdr msg , const char my_policy ) ;
void my_netlink_sender ( void ) ;
int my_server ( void ) ;

int main ( int argc , char ** argv )
{
	pid_t mypid ;
	
	if ( ( pipe ( flag_pipes ) == 0 ) && ( pipe ( msgs_pipes ) == 0 ) )
	{
		mypid = fork () ;

		if ( mypid < 0 )
		{
			printf ( "fork failed !!!\n" ) ;
		}
		else if ( mypid == 0 )
		{
			printf ( "netlink starting !!!\n" ) ;
			my_netlink_sender () ;
			printf ( "netlink closed !!!\n" ) ;
		}
		else
		{
			printf ( "server starting !!!\n" ) ;
			my_server () ;
			printf ( "server closed !!!\n" ) ;
		}
	}

	exit ( 0 ) ;
}

int get_flag ()
{
	return flag ;
}

int change_flag ( int num )
{
	if ( flag == num )
	{
		return 0 ;
	}
	else 
	{
		flag = num ;
	}

	return 0 ;
}

char set_policy (  void  )
{
	char get_ans ;
	char c ;

	printf ( "input the char :" ) ;
	if ( scanf ( "%c" , &get_ans ) == EOF )
	{
		get_ans = 'x' ;
	}

	while ( ( c = getchar () ) != '\n' && c != EOF ) ; 

	return get_ans ;
}

char get_policy (  void  )
{
	return set_policy () ;
}

int my_server ( void )
{
	int server_sockfd , client_sockfd ;
	int server_len , client_len ;
	char num = '0' ;
	char ans ;
	int result ;	
	char buff[3] ;

	struct sockaddr_in6 server_address ;
	struct sockaddr_in6 client_address ;
	
	fd_set readfds , testfds ;

	server_sockfd = socket ( AF_INET6 , SOCK_STREAM , 0 ) ;

	server_address.sin6_family = AF_INET6 ;
	//server_address.sin6_addr.s_addr = inet_addr ( ser_addr ) ;
	inet_pton ( AF_INET6 , ser_addr , &server_address.sin6_addr ) ;	
	server_address.sin6_port = port ;
	server_len = sizeof ( server_address ) ;

	bind ( server_sockfd , ( struct sockaddr * )&server_address , server_len ) ;

	printf ( "server is start , wait for the client !!!\n" ) ;

	listen ( server_sockfd , 5 ) ;

	client_len = sizeof ( client_address ) ;
	client_sockfd = accept ( server_sockfd , ( struct sockaddr * )&client_address , &client_len ) ;

	while ( num != 'x' )
	{	
		printf ( "------------------------\n\n" ) ;
		
		num = get_policy () ;
	
		if ( num != my_policy )
		{
			my_policy = num ;
			change_flag ( 1 ) ;

			if ( num == 'x' )
			{
				result = '0' ;
			}
			else
			{
				result = get_flag () ;
			}
			
			printf ( "ready to change to : %c\n" , my_policy ) ;
			write ( flag_pipes[1] , &result , sizeof ( int ) ) ;
			printf ( "flag is : %d\n" , get_flag () ) ;
		}
		else
		{	
			change_flag ( 0 ) ;
			result = get_flag () ;
			printf ( "not gonna change , current is : %c\n" , my_policy ) ;
			write ( flag_pipes[1] , &result , sizeof ( int ) ) ;
			printf ( "flag is : %d\n" , get_flag () ) ;
			sleep ( samp_itvl ) ;
			continue ;
		}
		
		write ( client_sockfd , &my_policy , 1 ) ;
		read ( client_sockfd , &ans , 1 ) ;
	
		while ( 1 )
		{
			if ( ans == my_policy )
			{
				ans = 'y' ;
				write ( client_sockfd , &ans , 1 ) ;
				buff[0] = my_policy ;

				write ( msgs_pipes[1] , buff , strlen (buff) ) ;
				printf ( "alg choice comm correct !\n" ) ;
				printf ( "------------------------\n" ) ;
				sleep ( samp_itvl ) ;
				break ;
			}
			else
			{
				printf ( "alg choice comm wrong !\n" ) ;
				ans = 'n' ;
				write ( client_sockfd , &ans , 1 ) ;
				write ( client_sockfd , &num , 1 ) ;
				read ( client_sockfd , &ans , 1 ) ;
			} 
		}

		
	}
	
	close ( client_sockfd ) ;
	close ( server_sockfd ) ;
	
	return 0 ;
}


void my_netlink_sender ( void )
{
	char buff [BUFSIZ + 1] ;

	sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST) ;
	if ( sock_fd == -1 )
	{
		printf ( "error happened in getting socket : %s.\n" , strerror (errno) ) ;
		return ;
	}

	while ( 1 )
	{	
		read ( flag_pipes[0] , &flag , sizeof ( int ) ) ;
		if ( flag == 0 )
		{
			printf ( "not change .\n" ) ; 
			continue ;
		}
		else
		{
			printf ( "ready to read !\n" ) ;
			read ( msgs_pipes[0] , buff , BUFSIZ ) ;

			my_policy = buff[0] ;
			
			while (1)
			{
				policy_ans = mysend_msg ( sock_fd, msg , my_policy) ;
	
				printf ( "sended message : %c.\n" , my_policy ) ;	
				printf ( "received message : %c.\n" , policy_ans ) ;	

				if ( policy_ans == my_policy )
				{
					printf ( "send message successful !\n" ) ;
					policy_ans = 'y' ;
					break ;
				}
				else 
				{
					printf ( "send message wrong , try again !\n" ) ;
					policy_ans = 'n' ;
					continue ;
				}
			}
		}

		printf ( "------------------------\n" ) ;
	}

	/* Close Netlink Socket */
	close(sock_fd) ;
}

void net_link_init ()
{
	memset(&msg, 0, sizeof(msg)) ;
	memset(&src_addr, 0, sizeof(src_addr)) ;

	src_addr.nl_family = AF_NETLINK ;
	src_addr.nl_pid = getpid() ; /* self pid */
	src_addr.nl_groups = 0 ; /* not in mcast groups */

	bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr)) ;

	memset(&dest_addr, 0, sizeof(dest_addr)) ;
	dest_addr.nl_family = AF_NETLINK ;
	dest_addr.nl_pid = 0 ; /* For Linux Kernel */
	dest_addr.nl_groups = 0 ; /* unicast */

	/* Fill the netlink message header */
	nlh=(struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD)) ;
	
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD) ;
	nlh->nlmsg_pid = getpid() ; /* self pid */
	nlh->nlmsg_flags = 0 ;

	memset(&msg, 0, sizeof(msg)); 	
	msg.msg_name = (void *)&dest_addr ;
	msg.msg_namelen = sizeof(dest_addr) ;
}

char mysend_msg ( int sock_fd , struct msghdr msg , const char key_word )
{
	int state ;

	net_link_init () ;

	strcpy(NLMSG_DATA(nlh), &key_word) ;
	iov.iov_base = (void *)nlh ;
	iov.iov_len = nlh->nlmsg_len ;
	msg.msg_iov = &iov ;
	msg.msg_iovlen = 1 ;

	printf(" Sending message. ...\n") ;
	state = sendmsg(sock_fd, &msg, 0) ;
	if ( state == -1 )
	{
		printf( "error happen in sending msg : %s\n" , strerror(errno) ) ;
	}

	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD)) ;
	printf(" Waiting message. ...\n") ;

	recvmsg(sock_fd, &msg, 0) ;
	strcpy( tmp , NLMSG_DATA(nlh)) ;

	return tmp[0] ;	
}
