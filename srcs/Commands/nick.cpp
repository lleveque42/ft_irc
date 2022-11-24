/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:41:21 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/24 13:06:49 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int	Server::_nick(User *user, std::string buff) {
	if (buff.empty())
		return _sendError(user, ERR_NONICKNAMEGIVEN(user->getClient(), user->getNick()));
	else if (!_validChars(buff))
		return _sendError(user, ERR_ERRONEUSNICKNAME(user->getClient(), user->getNick()));
	else if (_nickAlreadyUsed(user, buff)) {
		if (user->getFirstTry()) {
			user->setNick(buff);
			while (_nickAlreadyUsed(user, buff))
				buff.insert(buff.end(), '_');
		}
		else
			return _sendError(user, ERR_NICKNAMEINUSE(user->getClient(), user->getNick()));
	}
	std::string old_nick;
	if (user->getNick().empty())
		old_nick = buff;
	else
		old_nick = user->getNick();
	user->setNick(buff);
	return _sendExecuted(user, RPL_NICK(old_nick, user->getNick()));
}
