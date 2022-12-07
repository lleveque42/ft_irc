/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 16:21:38 by arudy             #+#    #+#             */
/*   Updated: 2022/12/07 17:19:13 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"
#include "../../includes/Channel.hpp"

int	Server::_invite(User *user, std::string buff) {
	if (buff.empty())
		return _sendError(user, ERR_NEEDMOREPARAMS(user->getClient(), user->getNick(), "INVITE"));
	size_t delimiter;
	std::string chan;
	std::string nick;
	if ((delimiter = buff.find(' ')) == buff.npos)
		return _sendError(user, ERR_NEEDMOREPARAMS(user->getClient(), user->getNick(), "INVITE"));
	nick = std::string(buff.begin(), buff.begin() + delimiter);
	chan = std::string(buff.begin() + delimiter + 1, buff.end());
	User *new_user = NULL;
	std::map<int, User *>::iterator it = _users.begin();
	for (; it != _users.end(); it++) {
		if (it->second->getNick() == nick) {
			new_user = it->second;
			break;
		}
	}
	if (it == _users.end())
		return _sendError(user, ERR_NOSUCHNICK(user->getClient(), user->getNick(), nick));
	if (!_channels.count(chan))
		return _sendError(user, ERR_NOSUCHCHANNEL(user->getClient(), user->getNick(), chan));
	if (!user->getJoined().count(chan))
		return _sendError(user, ERR_NOTONCHANNEL(user->getClient(), user->getNick(), chan)));
	Channel *channel = _channels[chan];
	if (channel->getUsers().count(nick))
		return _sendError(user, ERR_USERONCHANNEL(user->getClient(), user->getNick(), chan))); // Recheck l'error

	// RPL invite

	return 0;
}
