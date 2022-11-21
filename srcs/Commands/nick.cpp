/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:41:21 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/21 11:42:08 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int	Server::_nick(User *user, std::string buff) {
	if (buff.empty())
		return _sendError(user, ":431  \033[91mNick: No nickname provided\033[00m\r\n");
	else if (!_validChars(buff))
		return _sendError(user, ":432  \033[91mNick: Erroneus nickname\033[00m\r\n");
	else if (_nickAlreadyUsed(user, buff)) {
		if (user->getFirstTry()) {
			user->setNick(buff);
			buff.insert(buff.end(), '_');
		}
		else
			return _sendError(user, ":433  \033[91mNick: Nickname is already in use\033[00m\r\n");
	}
	std::string old_nick;
	if (user->getNick().empty())
		old_nick = buff;
	else
		old_nick = user->getNick();
	user->setNick(buff);
	std::string msg = ":" + old_nick + " NICK " + user->getNick() + "\r\n";
	return _sendExecuted(user, msg);
}
