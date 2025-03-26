/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:11:48 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/26 15:54:33 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

void nonBlockingFd(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		throw runtime_error("fcntl error");
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) == -1)
		throw runtime_error("fcntl error");
}

void addEpollFd(int efd, int fd, uint32_t events) {
	epoll_event event = {.events = events, .data = {.fd = fd}};
	if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &event) == -1)
		throw runtime_error("Failed to add fd to epoll.");
}

int createSocket(const sockaddr_in& address) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
		throw runtime_error("Failed to create socket: " + string(strerror(errno)));
	nonBlockingFd(fd);
	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1
		|| bind(fd, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) == -1
		|| listen(fd, SOMAXCONN) == -1)
		throw runtime_error("Failed to setup socket: " + string(strerror(errno)));
	return fd;
}
