/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:38:06 by lleveque          #+#    #+#             */
/*   Updated: 2022/12/07 15:53:32 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int Server::_quit(User *user, std::string args) {
	if (args.length() < 1)
		return _sendError(user, ERR_NEEDMOREPARAMS(user->getClient(), user->getNick(), "QUIT"));
	if (args[0] != ':')
		return _sendError(user, ERR_NOPREFIX(user->getClient(), user->getNick(), "QUIT"));
	args.erase(args.begin());
	_sendExecuted(user, RPL_QUIT((user->getClient()), args));
	std::map<std::string, Channel *> chans(user->getJoined());
	std::map<std::string, Channel *>::iterator ite = chans.end();
	for (std::map<std::string, Channel *>::iterator it = chans.begin(); it != ite; it++) {
		std::map<std::string, User *> users(it->second->getUsers());
		for (std::map<std::string, User *>::iterator it2 = users.begin(); it2 != users.end(); it2++) {
			if (it2->first != user->getNick())
				_sendExecuted(it2->second, RPL_PARTMSG(user->getClient(), user->getNick(), it->first, "has left server"));
		}
	}
	return _disconnectUser(user, 0);
}
