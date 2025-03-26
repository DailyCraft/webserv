/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:04:38 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/26 16:18:53 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <csignal>
#include "Config.hpp"

bool sigReceived = 0;

static void onSignal(int sig) {
	(void) sig;
	sigReceived = true;
}

static string onReceive(int epollFd, Server& server, int clientFd) {
	char buf[1024];
	ssize_t r = read(clientFd, buf, sizeof(buf));

	if (r == -1) {
		cerr << "Failed to read client data" << endl;
		return server.getErrorResponse(INTERNAL_SERVER_ERROR).toString();
	}
	if (r == 0) {
		epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, NULL);
		server.removeClient(clientFd);
		cout << "Connection closed" << endl;
		return "";
	} else {
		buf[r] = 0;
		try {
			return server.request(Request(buf)).toString();
		} catch (HttpRequestError& e) {
			return server.getErrorResponse(e.getStatus()).toString();
		}
	}
}

static void epollLoop(int efd, Server& server) {
	epoll_event events[MAX_EVENTS];

	while (!sigReceived) {
		int n = epoll_wait(efd, events, MAX_EVENTS, -1);

		for (int i = 0; i < n; i++) {
			int fd = events[i].data.fd;
			if (server.hasFd(fd)) {
				int client = server.addClient(fd);
				addEpollFd(efd, client, EPOLLIN | EPOLLET);
				cout << "Connection opened" << endl;
			} else if (server.hasClient(fd)) {
				string response = onReceive(efd, server, fd);
				if (response != "")
					send(fd, response.c_str(), response.length(), 0);
			} else
				throw runtime_error("Unexpected fd");
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
		cerr << "Failed to create the epoll fd. Reason: " << strerror(errno) << endl;
		return 1;
	}

	signal(SIGINT, onSignal);
	signal(SIGQUIT, onSignal);

	try {
		Server server;
		server._listens.push_back(parseAddress("0.0.0.0:8080"));
		Location loc;
		loc._allowedMethods.push_back(GET);
		loc._root = "./";
		server._locations["/get"] = loc;
		
		server.listen(efd);
		epollLoop(efd, server);
	} catch (exception& e) {
		cerr << e.what() << endl;
		return 1;
	}
}
