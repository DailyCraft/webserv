/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:09:03 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/24 16:07:34 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response& Response::setStatus(HttpStatus status) {
	_status = status;
	return *this;
}

Response& Response::addHeader(const string& header) {
	_headers.push_back(header);
	return *this;
}

Response& Response::setBody(const string &body) {
	_body = body;
	ostringstream ss;
	ss << "Content-Length: " << body.length();
	return addHeader(ss.str());
}

string Response::toString() const
{
	ostringstream ss;

	ss << "HTTP/1.1 " << _status << " " << getHttpStatusMessage(_status) << endl;
	
	for (size_t i = 0; i < _headers.size(); i++)
		ss << _headers[i] << endl;
	
	ss << endl;
	ss << _body;

	return ss.str();
}