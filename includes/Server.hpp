/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:07:23 by arudy             #+#    #+#             */
/*   Updated: 2022/11/16 17:47:02 by lleveque         ###   ########.fr       */
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
#include "../includes/Exception.hpp"

class Server
{
	private:
		std::string			_password;
		char				*_port;
		int					_sd;
		// int					_new_fd;


	public:
		Server(char *port, char *password);
		~Server();
		void	setup();
		void	launch();

	private:
		// Private func
};
