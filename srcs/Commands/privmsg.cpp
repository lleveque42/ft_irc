/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 16:17:10 by arudy             #+#    #+#             */
/*   Updated: 2022/11/22 18:14:13 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

static std::pair<std::string, std::string>	_splitPrivMsg(std::string buff) {
	size_t i = buff.find(':');
	return std::make_pair(std::string(buff.begin(), buff.begin() + i - 1), std::string(buff.begin() + i + 1, buff.end()));
}

int		Server::_privmsg(User *user, std::string buff) {
	(void)user;
	std::cout << "BUFF privmsg : " << buff << std::endl;
	std::pair<std::string, std::string> recip = _splitPrivMsg(buff);
	// std::cout << "Recip first :" << recip.first << "|" << std::endl;
	// std::cout << "Recip second :" << recip.second << "|" << std::endl;
	if (recip.first[0] == '#')
	{
		Channel *chan = _channels.find(recip.first)->second;
		if (!chan)
			return _sendError(user, ERR_CANNOTSENDTOCHAN(user->getClient(), recip.first));
		
	}

	return 0;
}
