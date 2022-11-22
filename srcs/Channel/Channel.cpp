/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:25:06 by arudy             #+#    #+#             */
/*   Updated: 2022/11/22 10:47:16 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"

Channel::Channel(std::string name) : _name(name){
}

Channel::~Channel(){
}

std::string const &Channel::getName() const {
	return _name;
}

void	Channel::addUser(User *user) {
	_users.push_back(user);
}

std::string	Channel::getUsersList()
{
	std::string list;

	for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it != _users.begin())
			list += " ";
		// check if is admin to add @ ??
		list += (*it)->getNick();
	}
	return list;
}
