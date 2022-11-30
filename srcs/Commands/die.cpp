/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:39:54 by arudy             #+#    #+#             */
/*   Updated: 2022/11/30 15:36:13 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int Server::_die(User *user, std::string buff) {
	static_cast<void>(buff);
	static_cast<void>(user);
	if (user->isOp())
		return 4;
	return _sendError(user, ERR_NOPRIVILEGES(user->getClient(), user->getNick()));
}
