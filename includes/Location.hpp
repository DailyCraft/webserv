/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:42:00 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/26 16:24:17 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "http.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Location {
public: // TODO: temp
	vector<HttpMethod> _allowedMethods;
	string _root;
	//int returnStatus; // TODO: Check when invalid
	//string returnPath; // TODO: allow file / url

public:
	Response request(const Request& request);
};
