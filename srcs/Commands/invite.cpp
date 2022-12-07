/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/07 16:21:38 by arudy             #+#    #+#             */
/*   Updated: 2022/12/07 16:22:22 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"
#include "../../includes/Channel.hpp"

int	Server::_invite(User *user, std::string buff) {
	(void)user;
	std::cout << "BUFF:" << buff << "|\n";
	return 0;
}
