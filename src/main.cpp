/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:04:38 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/24 16:25:59 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Config.hpp"
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include "Response.hpp"

static void nonBlockingFd(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw runtime_error("fcntl error");
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) == -1)
		throw runtime_error("fcntl error");
}

static int createSocket(const sockaddr_in& address) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	nonBlockingFd(fd);
	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	bind(fd, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
	listen(fd, SOMAXCONN);
	return fd;
}

static void addEpollFd(int efd, int fd, uint32_t events) {
	epoll_event event = {.events = events, .data = {.fd = fd}};
	if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &event) == -1)
		throw runtime_error("Failed to add fd to epoll.");
}

static void onReceive(int fd) {
	char buffer[1024];
	ssize_t nbytes = read(fd, buffer, sizeof(buffer));

	if (nbytes == -1) {
		throw runtime_error("Failed to read client data");
		close(fd);
	} else if (nbytes == 0) {
		close(fd);
		cout << "Connection closed" << endl;
	} else {
		string resString = Response()
			.setStatus(OK)
			.addHeader("Content-Type: text/html")
			.setBody("Hello, World!")
			.toString();
		send(fd, resString.c_str(), resString.length(), 0);
	}
}

static void epoll_loop(int efd, int sfd) {
	epoll_event events[MAX_EVENTS];

	while (true) {
		int n = epoll_wait(efd, events, MAX_EVENTS, -1);

		for (int i = 0; i < n; i++) {
			if (events[i].data.fd == sfd) {
				sockaddr_in client = {};
				socklen_t clientLen = sizeof(client);
				int clientFd = accept(sfd, reinterpret_cast<sockaddr*>(&client), &clientLen);
				nonBlockingFd(clientFd);
				addEpollFd(efd, clientFd, EPOLLIN | EPOLLET);
				cout << "Connection opened" << endl;
			} else
				onReceive(events[i].data.fd);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc > 2) {
		cerr << argv[0] << " [config_file]" << endl;
		return 1;
	}

	ifstream is(argc == 2 ? argv[1] : "./configs/default.conf");
	if (!is) {
		cerr << "Failed to open the config file!" << endl;
		return 1;
	}

	try {
		Config config(is);
	} catch (exception& e) {
		cerr << "Failed to parse the config file. Reason: " << e.what() << endl;
	}

	is.close();

	int efd = epoll_create(1);
	if (efd == -1) {
		cerr << "Failed to create the epoll fd." << endl;
		return 1;
	}

	int sfd = createSocket(parseAddress("0.0.0.0:8080"));
	addEpollFd(efd, sfd, EPOLLIN);
	epoll_loop(efd, sfd);
}
