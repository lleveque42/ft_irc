/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:35:57 by arudy             #+#    #+#             */
/*   Updated: 2022/11/22 10:54:07 by arudy            ###   ########.fr       */
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

void	Server::_sendJoinMsg(User *user, Channel *chan)
{
	std::string user_list = chan->getUsersList();
	std::cout << DIS_EXEC << user->getUserSd() << DIS_EXECEND("JOIN " + chan->getName()) << std::endl;
	// RPL_TOPIC 332 if chan topic
	_sendExecuted(user, RPL_NAMEREPLY(user->getNick(), user->getHostName(), chan->getName(), user_list)); // 353
	_sendExecuted(user, RPL_ENDOFNAMES(user->getNick(), user->getHostName(), chan->getName())); // 366
}

// Add chanlimit for users ?
// Manage key ?
// manage 0 to disco user from all chans ?

int	Server::_join(User *user, std::string buff) {
	(void)user;
	std::vector<std::string> args = strtovec(buff, ',');
	Channel *new_chan;

	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
		std::cout << "Args : |" << *it << "|\n";

	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
	{
		if (!_chans.count(*it)) {
			new_chan = new Channel(args[0]);
			_chans.insert(std::pair<std::string, Channel *>(args[0], new_chan));
			new_chan->addUser(user);
			std::cout << "New chan\n";
		}
		else {
			new_chan = _chans[*it];
			std::cout << "Already exist chan : " << *it << std::endl;
		}
		_sendJoinMsg(user, new_chan);
	}

	return 0;
}
