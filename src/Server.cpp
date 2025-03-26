/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:18:51 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/26 15:30:17 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::listen(int epollFd) {
	for (size_t i = 0; i < _listens.size(); i++) {
		int fd = createSocket(_listens[i]);
		_fds.push_back(fd);
		addEpollFd(epollFd, fd, EPOLLIN);
	}
}

bool Server::hasFd(int fd) const {
	return find(_fds.begin(), _fds.end(), fd) != _fds.end();
}

bool Server::hasClient(int fd) const {
	return find(_clients.begin(), _clients.end(), fd) != _clients.end();
}

int Server::addClient(int serverFd) {
	sockaddr_in addr = {};
	socklen_t addrLen = sizeof(addr);
	int fd = accept(serverFd, reinterpret_cast<sockaddr*>(&addr), &addrLen);
	nonBlockingFd(fd);
	_clients.push_back(fd);
	return fd;
}

void Server::removeClient(int clientFd) {
	_clients.erase(find(_clients.begin(), _clients.end(), clientFd));
	close(clientFd);
}

Response Server::request(const Request& request) const {
	map<string, Location>::const_iterator iter = _locations.begin();
	while (iter != _locations.end() && request.getPath().find(iter->first) != 0)
		iter++;
	if (iter == _locations.end())
		throw HttpRequestError(NOT_FOUND);
	
	Location loc = iter->second;
	return loc.request(request);
}

Response Server::getErrorResponse(HttpStatus status) const {
	ostringstream ss;
	ss << status << " " << getHttpStatusMessage(status) << endl;

	return Response()
		.setStatus(status)
		.addHeader("Content-Type: text/plain")
		.setBody(ss.str());
}
