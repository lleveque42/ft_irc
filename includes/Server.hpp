/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:07:23 by arudy             #+#    #+#             */
/*   Updated: 2022/11/17 15:15:08 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <csignal>
#include <fcntl.h>
#include <cerrno>
#include <map>
#include <map>
#include <vector>
#include <poll.h>
#include "../includes/Exception.hpp"
#include "../includes/User.hpp"

#define BUFFER_SIZE 4096

class Server
{
	private:
		std::string				_password;
		char					*_port;
		int						_sd;
		std::map<int, User*>	_users;
		std::vector<pollfd>		_pfds;
		int						_fdCount;
		char					_buff[BUFFER_SIZE + 1];

	public:
		Server(char *port, char *password);
		~Server();
		void	setup();
		void	launch();
		void	acceptUser();
		int		manageRequest(pollfd pfds);

	private:
		// Private func
};
