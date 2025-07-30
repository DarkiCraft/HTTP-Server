#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int main() {
	int server = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in addr{AF_INET, htons(8080), INADDR_ANY};
	bind(server, (sockaddr*)&addr, sizeof(addr));
	listen(server, SOMAXCONN);
	fcntl(server, F_SETFL, fcntl(server, F_GETFL, 0) | O_NONBLOCK);

	int ep = epoll_create1(0);
	epoll_event ev{EPOLLIN, {.fd = server}};
	epoll_ctl(ep, EPOLL_CTL_ADD, server, &ev);

	epoll_event events[64];
	char buf[1024];

	while (true) {
		int n = epoll_wait(ep, events, 64, -1);
		for (int i = 0; i < n; i++) {
			int fd = events[i].data.fd;

			if (fd == server) {
				int client = accept(server, nullptr, nullptr);
				fcntl(client, F_SETFL, fcntl(client, F_GETFL, 0) | O_NONBLOCK);
				epoll_event cev{EPOLLIN, {.fd = client}};
				epoll_ctl(ep, EPOLL_CTL_ADD, client, &cev);
			} else {
				int bytes = read(fd, buf, sizeof(buf));
				if (bytes <= 0) {
					epoll_ctl(ep, EPOLL_CTL_DEL, fd, nullptr);
					close(fd);
				} else {
					write(fd, buf, bytes);	// Echo back
				}
			}
		}
	}
}
