/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:56:06 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/26 11:32:01 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(const string& raw) {
	istringstream ss(raw);
	string line;
	
	getline(ss, line);
	istringstream lineStream(line);
	string linePart;
	lineStream >> linePart;
	_method = ::getMethod(linePart);
	lineStream >> linePart;
	_path = linePart;
	lineStream >> linePart;
	if (linePart != "HTTP/1.1")
		throw HttpRequestError(HTTP_VERSION_NOT_SUPPORTED);

	while (getline(ss, line) && line != "\r")
		_headers.push_back(line);

	ostringstream bodyStream;
	bodyStream << ss.rdbuf();
	_body = bodyStream.str();
}
