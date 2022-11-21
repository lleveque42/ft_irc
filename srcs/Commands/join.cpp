/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:35:57 by arudy             #+#    #+#             */
/*   Updated: 2022/11/21 22:39:51 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

static std::vector<std::string> strtovec(std::string buff, char deli) {
	std::vector<std::string> args;
	std::string tmp;
	std::istringstream buffstream(buff);

	while(std::getline(buffstream, tmp, deli))
		args.push_back(tmp);
	return args;
}

int	Server::_join(User *user, std::string buff) {
	(void)user;
	std::vector<std::string> args = strtovec(buff, ',');
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
		std::cout << "Args : |" << *it << "|\n";

	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
	{
		if (!_chans.count(*it)) {
			Channel *new_chan = new Channel(args[0]);
			_chans.insert(std::pair<std::string, Channel *>(args[0], new_chan));
			new_chan->addUser(user);
			std::cout << "New chan\n";
		}
		else {
			std::cout << "Already exist chan : " << *it << std::endl;
		}
		// RPL_TOPIC if chan topic
		// 353
		// 366
		_sendExecuted(user, RPL_NAMEREPLY(user->getNick(), user->getHostName(), (*it), user->getRealName()));
	// _sendExecuted(user, RPL_QUIT((user->getNick()), user->getHostName(), args));

	}

	return 0;
}
