/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:38:06 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/24 13:08:14 by arudy            ###   ########.fr       */
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
	//msg les channels ?
	_sendExecuted(user, RPL_QUIT((user->getNick()), user->getHostName(), args));
	return _disconnectUser(user, 0);
}
