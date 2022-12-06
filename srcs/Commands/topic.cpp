/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 09:54:50 by arudy             #+#    #+#             */
/*   Updated: 2022/12/06 13:11:22 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int Server::_topic(User *user, std::string args) {
	if (args.empty())
		return _sendError(user, ERR_NEEDMOREPARAMS(user->getClient(), user->getNick(), "TOPIC"));
	std::pair<std::string, std::string> pair = _strToPair(args);

	if (!_channels.count(pair.first))
		return _sendError(user, ERR_NOSUCHCHANNEL(user->getClient(), user->getNick(), pair.first));
	Channel *chan = _channels.find(pair.first)->second;
	if (!chan->getUsers().count(user->getNick()))
		return _sendError(user, ERR_NOTONCHANNEL(user->getClient(), user->getNick(), pair.first)));
	if (args.find(':') != args.npos) {
		if (!user->isOp(chan))
			return _sendError(user, ERR_CHANOPRIVSNEEDED(user->getClient(), user->getNick(), pair.first));
		chan->setTopic(true, pair.second);
		std::map<std::string, User *> targets = chan->getUsers();
		for (std::map<std::string, User *>::iterator it = targets.begin(); it != targets.end(); it++) {
			std::string rpl = ":" + user->getClient() + " TOPIC " + pair.first + " :" + pair.second + "\r\n";
			_sendExecuted(it->second, rpl);
		}
		return 0;
	}
	if (chan->getTopic().second.empty())
		return _sendExecuted(user, RPL_NOTOPIC(user->getClient(), user->getNick(), pair.first));
	return _sendExecuted(user, RPL_TOPIC(user->getClient(), user->getNick(), pair.first, pair.second));;
}
