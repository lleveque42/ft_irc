/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:41:05 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/21 11:41:54 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int Server::_pong(User *user, std::string buff) {
	(void)buff;
	std::string msg = "PONG irc.server\r\n";
	return _sendExecuted(user, msg);
}
