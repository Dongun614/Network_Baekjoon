#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char* message);

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

    printf("프로그램을 시작합니다.....\n");
    int con = 1;
    while(con){
        int choose;
        printf("- 사칙 연산 프로그램 - \n");
        printf("- [1] 덧셈 \n");
        printf("- [2] 뺄셈 \n");
        printf("- [3] 곱셈 \n");
        printf("- [4] 나눗셈 \n");
        printf("- [0] 프로그램 종료 \n");
        printf("계산할 사칙연산을 선택해주세요(숫자로 입력): ");
        
        //계산할 사칙연산 선택
        scanf("%d", &choose); //이거 문자로 받아서 숫자로 변환하는 방식으로 바꾸고 싶고, 거기에 대한 예외처리 해보기

        //선택한 사칙연산에 대한 숫자 전달
        write(sock, &choose, sizeof(int));

        //0을 선택시 프로그램 종료
        if(choose == 0){
            printf("프로그램을 종료합니다...\n");
            sleep(1);
            break;
        }

        char* arithmetic[4] = {"덧셈", "뺄셈", "곱셈", "나눗셈"}; 
        int A, B;

        //연산에 사용할 숫자 2개 입력 받기
        printf("선택하신 연산은 \"%s\" 입니다.\n", arithmetic[choose-1]);
        printf("연산에 사용할 두 개의 숫자를 입력해주세요: ");
        scanf("%d %d", &A, &B);

        //입력받은 숫자 2개 전송
        write(sock, &A, sizeof(int));
        write(sock, &B, sizeof(int));

        //결과값 받기
        int result;
        read(sock, &result, sizeof(int));

        printf("결과 : %d\n", result);

        printf("프로그램을 계속 사용하시겠습니까?(1: yes, 0: no): ");
        scanf("%d", &con);
        write(sock, &con, sizeof(int));
        if(con == 0){
            printf("프로그램을 종료합니다...\n");
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

//Version별 구상
//헤더파일 만들어서 해보기
//makefile 배우기
//서버에서 색깔로 Client와 사칙연산을 다른 색깔로 뜰 수 있게
//멀티스레드로 구현해보기