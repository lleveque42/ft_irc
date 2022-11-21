/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:35:57 by arudy             #+#    #+#             */
/*   Updated: 2022/11/21 18:58:40 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int	Server::_join(User *user, std::string buff) {
	(void)user;
	std::vector<std::string> args;
	std::string tmp;
	std::istringstream buffstream(buff);

	std::cout << "BUFF : " << buff << std::endl;
	while(std::getline(buffstream, tmp, ','))
		args.push_back(tmp);

	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
		std::cout << "Args : |" << *it << "|\n";
	return 0;
}
