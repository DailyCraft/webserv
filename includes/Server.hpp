/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvan-hum <dvan-hum@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 15:38:35 by dvan-hum          #+#    #+#             */
/*   Updated: 2025/03/24 16:24:45 by dvan-hum         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Location.hpp"

class Server {
	vector<sockaddr_in> listens;
	vector<string> serverNames;
	vector<string> index;
	map<string, Location> locations;
};