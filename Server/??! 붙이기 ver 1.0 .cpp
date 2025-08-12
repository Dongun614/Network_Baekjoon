//참고자료
//https://blockdmask.tistory.com/350

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

//1. 문자열의 길이를 받는다.
//2. 문자열을 받는다.
//3. 받은 길이에 ??! 만큼 더해서 길이를 전송한다.
//4. 문자열에 ??!을 붙여서 전송한다.

void error_handling(char* message);

#define CLIENT_NUM 10
#define BUF_SIZE 1024
const char* PUT_THIS = "\?\?!";

int main(int argc, char* argv[]){
    printf("서버를 엽니다........\n");

    int serv_sock, clnt_sock;

    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;

    socklen_t clnt_adr_sz;

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handling("socket() error");
    }

    int reuse_opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse_opt, sizeof(reuse_opt));

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("listen() error");
    }

    if(listen(serv_sock, CLIENT_NUM) == -1){
        error_handling("listen() error");
    }

    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if(clnt_sock == -1){
        error_handling("accept() error");
    } else {
        printf("Connected client %d...\n", clnt_sock);
    }

    printf("Client %d에서 프로그램이 시작되었습니다...\n", clnt_sock);

    int con = 1;
    while(con){
        int length;
        char word[BUF_SIZE];

        read(clnt_sock, &length, sizeof(int));
        read(clnt_sock, word, length);
        word[length] = '\0';

        strcat(word, PUT_THIS);

        length = strlen(word);

        write(clnt_sock, &length, sizeof(int));
        write(clnt_sock, word, length);

        read(clnt_sock, &con, sizeof(int));
        if(con == 0){
            printf("%d Client가 프로그램을 종료합니다......\n", clnt_sock);
            sleep(1);
            break;
        }
    }

    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void error_handling(char* message){
    printf("%s\n", message);
    exit(1);
}