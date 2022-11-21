/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:26:42 by arudy             #+#    #+#             */
/*   Updated: 2022/11/21 15:34:06 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Exception.hpp"
#include "../includes/Server.hpp"
#include "../includes/User.hpp"

class Channel
{
	private:
		std::string	_name;
		std::vector<User *>	_users;

	public:
		Channel(std::string name);
		~Channel();
		std::string const	&getName() const;
};
