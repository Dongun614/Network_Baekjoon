#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

//어떤 사칙연산을 할지 물어본다.
//자연수 두개를 입력받는다.
//해당 자연수를 사용자가 선택한 연산에 맞추어 계산한다.
//해당 값을 보내준다.

#define CLIENT_NUM 10

void error_handling(char* message);

int main(int argc, char* argv[]){
    printf("서버를 엽니다......\n");

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
        int choose;
        char* arithmetic[4] = {"덧셈", "뺄셈", "곱셈", "나눗셈"}; 
        read(clnt_sock, &choose, sizeof(int));
        printf("Client %d choose %s\n", clnt_sock, arithmetic[choose-1]);

        if(choose == 0){
            printf("Client %d 연결 종료......\n", clnt_sock);
            sleep(1);
            break;
        }

        int A, B;
        //사용자가 입력한 두 수 입력받기
        read(clnt_sock, &A, sizeof(int));
        read(clnt_sock, &B, sizeof(int));

        int result;
        switch(choose){
            case 1: //덧셈
                result = A + B;
                write(clnt_sock, &result, sizeof(int));
                break;

            case 2: //뺄셈
                result = A - B;
                write(clnt_sock, &result, sizeof(int));
                break;

            case 3: //곱셈
                result = A * B;
                write(clnt_sock, &result, sizeof(int));
                break;

            case 4: //나눗셈
                result = A / B;
                write(clnt_sock, &result, sizeof(int));
                break;
        }
        
        read(clnt_sock, &con, sizeof(int));
        if(con == 0){
            printf("Client %d 연결 종료......\n", clnt_sock);
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