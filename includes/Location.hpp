/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:42:00 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/24 16:24:54 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <vector>
#include "http.hpp"

class Location {
	vector<HttpMethod> allowedMethods;
	string root;
	//int returnStatus; // TODO: Check when invalid
	//string returnPath; // TODO: allow file / url
};
