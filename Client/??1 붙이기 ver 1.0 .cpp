#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char* message);

#define BUF_SIZE 1024

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_adr;

    if(argc != 3){
        printf("Usage : %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("connect() error");
    } else {
        printf("Connected..............\n");
    }

    printf("프로그램을 시작합니다.......\n");
    int con = 1;

    while(con){
        char word[BUF_SIZE];
        int word_len;

        printf("문자를 입력해주세요(프로그램 종료 0): ");
        scanf("%s", word);
        word_len = strlen(word);

        write(sock, &word_len, sizeof(int));
        write(sock, word, word_len);

        read(sock, &word_len, sizeof(int));
        read(sock, word, word_len);

        word[word_len] = '\0';

        printf("출력: %s\n", word);
        printf("\n");
        printf("프로그램을 계속 사용하시겠습니까?(계속 1, 종료 0): ");
        scanf("%d", &con);
        write(sock, &con, sizeof(int));
        if(con == 0){
            printf("프로그램을 종료합니다....\n");
            sleep(1);
            break;
        }
    }

    close(sock);

    return 0;
}

void error_handling(char* message){
    printf("%s\n", message);
    exit(1);
}