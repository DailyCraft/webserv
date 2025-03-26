/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:09:39 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/26 16:28:12 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

static Response readDir(const char* path) {
	(void) path;
	return Response()
		.setStatus(OK)
		.addHeader("Content-Type: text/plain")
		.setBody("TODO"); // TODO
}

static Response readFile(const char* path) {
	int fd = open(path, O_RDONLY);
	vector<char> buf;
	char temp[1024];
	ssize_t r;
	while ((r = read(fd, temp, 1024)) > 0)
		buf.insert(buf.end(), temp, temp + r);
	if (r == -1) {
		cerr << "Failed to read file." << endl;
		throw HttpRequestError(INTERNAL_SERVER_ERROR);
	}
	close(fd);

	return Response()
		.setStatus(OK)
		.addHeader("Content-Type: text/plain")
		.setBody(buf);
}

Response Location::request(const Request& request) {
	string path = _root + request.getPath().substr(4); // TODO: 4 is for /get
	if (access(path.c_str(), F_OK) == -1)
		throw HttpRequestError(NOT_FOUND);

	struct stat s;
	stat(path.c_str(), &s);
	return S_ISDIR(s.st_mode) ? readDir(path.c_str()) : readFile(path.c_str());
}