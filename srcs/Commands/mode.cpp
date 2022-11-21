/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:15:43 by arudy             #+#    #+#             */
/*   Updated: 2022/11/21 13:16:42 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int	Server::_mode(User *user, std::string buff) {
	std::cout << "MODE\n";
	std::cout << "Buff : " << buff << std::endl;
	(void)user;
	return 0;
}
