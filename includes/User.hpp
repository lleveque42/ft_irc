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

class Channel;

class User {
	private:
		int			_user_sd;
		bool		_auth;
		bool		_tried_to_auth;
		bool		_first_try;
		bool		_cap;
		std::string	_nick;
		std::string	_mode;
		std::string	_user_name;
		std::string	_real_name;
		std::string	_host_name;
		std::map<std::string, Channel *> _ops;
		std::map<std::string, Channel *> _joined;
		User();

	public:
		User(int sd);
		~User();
		int const			&getUserSd() const;
		std::string const	&getNick() const;
		std::string const	&getUserName() const;
		std::string const	&getHostName() const;
		std::string const	&getRealName() const;
		bool const			&getAuth() const;
		bool const			&getTriedToAuth() const;
		bool const			&getFirstTry() const;
		bool const			&getCap() const;
		void				setNick(std::string value);
		void				setUserName(std::string value);
		void				setHostName(std::string value);
		void				setRealName(std::string value);
		void				setAuth(bool value);
		void				setTriedToAuth(bool value);
		void				setFirstTry(bool value);
		void				setCap(bool value);
};
