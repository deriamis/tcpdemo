/*
   Copyright 2026 Ryan Egesdahl

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdio.h>

#include "common.h"


static volatile sig_atomic_t keep_running = true;

void send_cmd(int sock) {
    char str[MAX_MSG_LENGTH] = {0};

    while (keep_running) {
        if (fgets(str, MAX_MSG_LENGTH, stdin) != str) break;
        if (strcmp(str, "exit\n") == 0) keep_running = false;
        if(send(sock, str, strlen(str)+1, 0) < 0) perror("client send");
    }

    printf("Closing connection.\n");
}

void receive(int sock) {
    char buf[MAX_MSG_LENGTH] = {0};
    int filled = 0;

    while(keep_running) {
        if ((filled = recv(sock, buf, MAX_MSG_LENGTH-1, 0)) == 0) break;
        buf[filled] = '\0';
        printf("%s", buf);
        fflush(stdout);		
    }

    printf("Server disconnected.\n");
}

int main(int argc, char **argv) {
    if(argc != 2) FATAL("args");

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) FATAL("socket");

    struct in_addr server_addr;
    if(!inet_aton(argv[1], &server_addr)) FATAL("inet_aton");

    struct sockaddr_in connection;
    connection.sin_family = AF_INET;
    memcpy(&connection.sin_addr, &server_addr, sizeof(server_addr));
    connection.sin_port = htons(DEFAULT_PORT);
    if (connect(sock, (const struct sockaddr*) &connection, sizeof(connection)) != 0) FATAL("connect");

    int pid = fork();
    if(pid == -1) {
        return EXIT_FAILURE;
    } else if (pid == 0) {
        receive(sock);
    } else {
        send_cmd(sock);
        waitpid(pid, NULL, 0);
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);
    return EXIT_SUCCESS;
}
