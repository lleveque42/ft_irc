/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:09:18 by arudy             #+#    #+#             */
/*   Updated: 2022/11/16 10:10:33 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>

class User
{
	private:
		int			_user_sd;
		bool		_auth;
		bool		_triedToAuth;
		std::string	_nick;
		std::string	_user_name;
		User();

	public:
		User(int sd);
		~User();
		int const			&getUserSd() const;
		std::string const	&getNick() const;
		std::string const	&getUserName() const;
		bool const			&getAuth() const;
};
