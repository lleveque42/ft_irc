/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:41:27 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/22 16:55:36 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int	Server::_pass(User *user, std::string args) {
	user->setTriedToAuth(true);
	if (user->getAuth())
		return _sendError(user, ERR_ALREADYREGISTERED(user->getClient(), "PASS"));
	if (args != _password)
		return _disconnectUser(user, 2);
	user->setAuth(true);
	std::string ok(RPL_PASS);
	_sendAll(user->getUserSd(), ok.c_str(), ok.length(), 0);
	std::cout << DIS_EXEC << user->getUserSd() << DIS_EXECEND(RPL_PASSSEND) << std::endl;
	return 0;
}
