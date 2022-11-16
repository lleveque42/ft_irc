/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 08:43:59 by arudy             #+#    #+#             */
/*   Updated: 2022/11/16 11:30:22 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../includes/Server.hpp"
#include "../includes/Exeptions.hpp"

bool	check_input(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Wrong input, usage is : ./ircserv <port> <password>" << std::endl;
		return false;
	}
	for (int i = 0; av[1][i]; i++)
	{
		if (!isdigit(av[1][i]))
		{
			std::cerr << "Invalid port, use only numbers" << std::endl;
			return false;
		}
	}
	return true;

}

int	main(int ac, char **av)
{
	if (!check_input(ac, av))
		return 1;
	Server *server = new Server(atoi(av[1]), av[2]);
	try
	{
		server->launch();
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
