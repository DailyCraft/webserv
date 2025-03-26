/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:51:01 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/26 08:24:37 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <sstream>
#include "http.hpp"

class Request {
	string _path;
	HttpMethod _method;
	vector<string> _headers;
	string _body;

public:
	Request(const string& raw);

	const string& getPath() const { return _path; }
	HttpMethod getMethod() const { return _method; }
	const vector<string>& getHeaders() const { return _headers; }
	const string& getBody() const { return _body; }
};