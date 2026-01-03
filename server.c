#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>

#include <errno.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL SO_NOSIGPIPE
#endif


static volatile sig_atomic_t keep_running = true;

void handle_SIGINT(int signal)
{
    UNUSED(signal);
    keep_running = false;
}

void handle_client(int client_socket) {
    char buf[MAX_MSG_LENGTH] = {0};
    ssize_t bytes_read = 0;
    int pid = getpid();

    const char *welcome_msg = "Welcome to the Echo Server! Type 'exit' to disconnect.\n";
    const char *goodbye_msg = "Goodbye!\n";

    send(client_socket, welcome_msg, strlen(welcome_msg), 0);
    while (keep_running) {
        if ((bytes_read = recv(client_socket, buf, sizeof(buf) - 1, 0)) <= 0) break;
        // Null-terminate the string
        buf[bytes_read] = '\0';
        printf("\t[%d] Received: %s", pid, buf);

        // Check for exit command
        if (strcmp(buf, "exit\n") == 0) {
            send(client_socket, goodbye_msg, strlen(goodbye_msg), 0);
            printf("\t[%d] Closing client connection.\n", pid);
            break;
        } else {
            // Echo the message back to the client
            send(client_socket, "Echo: ", 6, 0);
            send(client_socket, buf, bytes_read, 0);
        }
    }

    if (bytes_read == 0) {
        printf("\t[%d] Client disconnected.\n", pid);
    } else if (bytes_read < 0) {
        perror("recv failed");
    }

    shutdown(client_socket, SHUT_RDWR);
	printf("\t[%d] Dying.\n", pid);
}

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    struct sigaction act = {0};

    act.sa_handler = handle_SIGINT;
    if (sigaction(SIGINT, &act, NULL) == -1) {
        FATAL("set SIGINT handler");
    }

    int server_socket_fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (server_socket_fd == -1) {
        FATAL("opening IPv4 socket");
    }

    int opt_ipv6_only = 0;
    setsockopt(server_socket_fd, IPPROTO_IPV6, IPV6_V6ONLY, &opt_ipv6_only, sizeof(opt_ipv6_only));

    struct sockaddr_in6 socket = {0};
	socket.sin6_family = AF_INET6;
	socket.sin6_addr = in6addr_any;
	socket.sin6_port = htons(DEFAULT_PORT);

	if(bind(server_socket_fd, (void*) &socket, sizeof(socket))) {
        FATAL("binding TCP socket");
    }

	if(listen(server_socket_fd, 1) == -1) {
        FATAL("server_socket listen");
    }

    printf("Telnet server listening on port %d...\n", DEFAULT_PORT);

    while (keep_running) {
        struct sockaddr client_socket = {0};
        unsigned int client_socket_sz = sizeof(client_socket);

        int client_socket_fd = accept(server_socket_fd, &client_socket, &client_socket_sz);
        if (client_socket_fd == -1) {
            FATAL("client_socket accept");
        }

    	printf("\nClient connected. Forking...\n");

        int pid = fork();
		if(pid == -1) {
            return EXIT_FAILURE;
        } else if (pid == 0) {
            handle_client(client_socket_fd);
            close(client_socket_fd);
            client_socket_fd = -1;
        } else {
            printf("\tchild pid = %d\n", pid);
            waitpid(pid, NULL, 0);
        }

        if (client_socket_fd != -1) {
            close(client_socket_fd);
        }
	}

	close(server_socket_fd);
    return EXIT_SUCCESS;
}
