/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 14:14:03 by lleveque          #+#    #+#             */
/*   Updated: 2022/12/02 16:48:26 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int Server::_oper(User *user, std::string buff) {
	size_t delimiter;
	std::string key;
	std::string name;

	if ((delimiter = buff.find(' ')) == buff.npos)
		return _sendError(user, ERR_NEEDMOREPARAMS(user->getClient(), user->getNick(), "OPER"));
	name = std::string(buff.begin(), buff.begin() + delimiter);
	key = std::string(buff.begin() + delimiter + 1, buff.end());
	if (name != _op_name || key != _op_password)
		return _sendError(user, ERR_PASSWDMISMATCH(user->getClient(), user->getNick()));
	user->setOp(1);
	_sendExecuted(user, RPL_YOUREOPER(user->getClient(), user->getNick()));
	return _sendExecuted(user, RPL_UMODEIS(user->getClient(), user->getNick(), user->getModes()));
}
