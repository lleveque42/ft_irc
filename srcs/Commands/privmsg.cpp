/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:17:10 by arudy             #+#    #+#             */
/*   Updated: 2022/11/23 22:37:07 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

static std::pair<std::string, std::string>	_splitPrivMsg(std::string buff) {
	size_t i = buff.find(':');
	return std::make_pair(std::string(buff.begin(), buff.begin() + i - 1), std::string(buff.begin() + i + 1, buff.end()));
}

void	Server::_sendPrivMsg(User *sender, User *target, std::string chan_name, std::string msg) {
	std::string rpl = ":" + sender->getNick() + " PRIVMSG " + chan_name + " " + msg + "\r\n";
	// _sendAll(target->getUserSd(), rpl.c_str(), rpl.length(), 0);
	_sendExecuted(target, rpl);
}

int		Server::_privmsg(User *user, std::string buff) {
	std::pair<std::string, std::string> recip = _splitPrivMsg(buff); // Nedd better check if privmsg for user ?
	std::map<std::string, User *> targets;

	if (recip.first[0] == '#') {
		if (!_channels.count(recip.first))
			return _sendError(user, ERR_CANNOTSENDTOCHAN(user->getClient(), recip.first));
		Channel *chan = _channels.find(recip.first)->second;
		targets = chan->getUsers();
	}
	else {
		std::cout << "Recip is a user\n";
		std::map<int, User *>::iterator it;
		for (it = _users.begin(); it != _users.end(); it++)
			if ((*it).second->getNick() == recip.first)
				targets.insert(std::pair<std::string, User *> ((*it).second->getNick(), (*it).second));
		if (it == _users.end())
			return _sendError(user, ERR_NOSUCHNICK(user->getClient(), user->getNick()));
	}
	for (std::map<std::string, User *>::iterator it = targets.begin(); it != targets.end(); it++) {
		if (it->second != user) { // Check chan mod ??
			_sendPrivMsg(user, it->second, recip.first, recip.second);
		}
	}
	return 0;
}
