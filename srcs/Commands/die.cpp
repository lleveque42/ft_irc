/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:39:54 by arudy             #+#    #+#             */
/*   Updated: 2022/11/29 16:15:16 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int Server::_die(User *user, std::string buff) {
	static_cast<void>(buff);
	static_cast<void>(user);
	// MARCHE PAS
	// if (user->isOp())
	// 	return 2;
	// return _sendError(user, ERR_NOPRIVILEGES(user->getClient(), user->getNick()));
	return 0;
}
