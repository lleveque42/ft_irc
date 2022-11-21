/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 12:38:06 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/21 16:43:00 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int Server::_quit(User *user, std::string args) {
	if (args.length() < 1)
		return _sendError(user, ERR_NEEDMOREPARAMS("QUIT"));
	if (args[0] != ':')
		return _sendError(user, ERR_NOPREFIX("QUIT"));
	args.erase(args.begin());
	//msg les channels ?
	_sendExecuted(user, RPL_QUIT((user->getNick()), user->getHostName(), args));
	return _disconnectUser(user, 0);
}
