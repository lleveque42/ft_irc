/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:15:43 by arudy             #+#    #+#             */
/*   Updated: 2022/11/23 15:31:57 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"
#include "../../includes/Channel.hpp"

// static int _applyMode(User *user, std::string mode, bool value) {
// 	for (int i = 0; i < mode.length(); i++) {
// 		if (mode[i] == 'i')
// 			// user.setInvisble(value);
// 		else if (mode[i] == 'o' && value == false)
// 			// user.setOp(value);
// 		else
// 			return 1; //ERR_UMODEUNKNOWNFLAG (501)
// 	}
// }

// static int _applyMode(Channel *channel, std::string mode, bool value) {
// 	for (int i = 0; i < mode.length(); i++) {
// 		if (mode[i] == 'l') {

// 		}
// 		else if (mode[i] == 'k') {

// 		}
// 		else
// 			return 1; //ERR_UMODEUNKNOWNFLAG (501)
// 	}
// }

// int	Server::_mode(User *user, std::string buff) {
// 	bool value;
// 	size_t delimiter;
// 	std::string mode;
// 	std::string target;

// 	if (buff[0] != '#' && (delimiter = buff.find(':')) == buff.npos)
// 		return 1; // RPL_UMODEIS (221)
// 	else if ((delimiter = buff.find(':')) == buff.npos)
// 		return 1; // RPL_CHANNELMODEIS (221)
// 	target = std::string(buff.begin(), buff.begin() + delimiter);
// 	mode = std::string(buff.begin() + delimiter + 2, buff.end());
// 	if (mode[0] == '+')
// 		value = true;
// 	else  if (mode[0] != '-')
// 		return 1; //ERR_NOPREFIX (461)
// 	mode.erase(mode.begin());
// 	if (target[0] != '#') {
// 		std::map<int, User *>::iterator it;
// 		for (it = _users.begin(); it != _users.end(); it++)
// 			if (it->second->getNick() == buff)
// 				break;
// 		if (it == _users.end())
// 			return 1; //ERR_NOSUCHNICK (401)
// 		if (user->getNick() != target)
// 			return 1; //ERR_USERDONTMATCH (502)
// 		return _applyMode(user, mode, value);
// 	}
// 	else {
// 		Channel *channel_moded = NULL;
// 		if (!_channels.count(target))
// 			return 1; //ERR_NOSUCHCHANNEL (403)
// 		channel_moded = _channels[target];
// 		if (!channel_moded->isOp(user))
// 			return 1; //ERR_CHANOPRIVSNEEDED (482)
// 		return _applyMode(_channels[target], mode, value);
// 	}
// 	// std::cout << "|" << nick << "|"  << std::endl;
// 	// std::cout << "|" << mode << "|"  << std::endl;
// 	return 0;
// }
