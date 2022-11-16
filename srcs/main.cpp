/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 08:43:59 by arudy             #+#    #+#             */
/*   Updated: 2022/11/16 15:20:54 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../includes/Server.hpp"
#include "../includes/Exception.hpp"

#define ARG_ERR "ircserv: 2 arguments needed:\n\nUsage is: ./ircserv <port> <password>\n\t<port>: port number on wich your IRC server will be listening to for incoming IRC connections\n\t<password>: it will be needed by any IRC client that tries to connect to your server"
#define PORT_ERR  "Invalid port, use only number between "
#define PASSWORD_ERR "Password must not be empty"

bool	check_input(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << ARG_ERR << std::endl;
		return false;
	}
	const std::string port(av[1]);
	std::string::const_iterator it = port.begin();
	for (; it != port.end(); it++)
	{
		if (!std::isdigit(*it))
		{
			std::cerr << PORT_ERR << std::endl;
			return false;
		}
	}
	const std::string password(av[1]);
	if (password.empty())
	{
		std::cerr << PASSWORD_ERR << std::endl;
		return false;
	}
	return true;
}

int	main(int ac, char **av)
{
	if (!check_input(ac, av))
		return 1;
	Server *server = new Server(av[1], av[2]);
	try
	{
		server->setup();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		delete server;
		return 1;
	}
	delete server;
	return 0;
}
