/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:38:35 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/26 15:19:43 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include "Location.hpp"
#include "socket.hpp"

class Server {
public: // TODO: temp
	vector<sockaddr_in> _listens;
	vector<string> _serverNames;
	vector<string> _index;
	map<string, Location> _locations;
	map<HttpStatus, string> _errorPages;

	vector<int> _fds;
	vector<int> _clients;

public:
	void listen(int epollFd);

	bool hasFd(int fd) const;
	bool hasClient(int fd) const;

	int addClient(int serverFd);
	void removeClient(int clientFd);
	Response request(const Request& request) const;

	Response getErrorResponse(HttpStatus status) const;
};