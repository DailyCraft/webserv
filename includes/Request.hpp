/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:51:01 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/24 13:55:50 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include "http.hpp"

class Request {
	string _path;
	HttpMethod _method;
	string _httpVersion;
	vector<string> _headers;
	string _body;

public:
	Request(const ifstream& is);
};