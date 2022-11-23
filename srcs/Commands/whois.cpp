/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 13:58:20 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/23 14:09:15 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int Server::_whois(User *user, std::string buff) {
	for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end(); it++)
		if (it->second->getNick() == buff)
			return _sendExecuted(user, RPL_WHOISUSER(user->getClient(), user->getNick(), it->second->getNick(), it->second->getUserName(), it->second->getHostName(), it->second->getRealName()));
	return 0;
}
