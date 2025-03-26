/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:07:21 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/26 16:12:12 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <sstream>
#include "http.hpp"

class Response {
	string _httpVersion;
	HttpStatus _status;
	vector<string> _headers;
	vector<char> _body;

public:
	Response& setStatus(HttpStatus status);
	Response& addHeader(const string& header);
	Response& setBody(const vector<char> body);
	Response& setBody(const string& body);
	string toString() const;
};