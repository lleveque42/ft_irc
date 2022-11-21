/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:41:27 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/21 11:43:08 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int	Server::_pass(User *user, std::string args) {
	user->setTriedToAuth(true);
	if (user->getAuth())
		return 0; //voir quoi faire quand deja authentifié
	if (args != _password)
		return _disconnectUser(user, 2);
	if (!user->getAuth()) {
		user->setAuth(true);
		std::string ok("\033[92mConnection accepted !\n\033[093mWelcome to our IRC server !\033[00m\r\n");
		_sendAll(user->getUserSd(), ok.c_str(), ok.length(), 0);
		std::cout << GREEN BOLD "[ircserv]" RESET GREEN " Send    -->    " GREEN BOLD "[Client " << user->getUserSd() << "]" RESET GREEN ":    Connection accepted: Password correct" << RESET << std::endl;
	}
	return 0;
}
