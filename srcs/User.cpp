/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:09:52 by arudy             #+#    #+#             */
/*   Updated: 2022/11/16 10:10:21 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"

User::User(int sd) : _user_sd(sd), _nick(""), _user_name("")
{}

User::~User()
{
}

int const			&User::getUserSd() const { return _user_sd; }
std::string const	&User::getNick() const { return _nick; }
std::string const	&User::getUserName() const { return _user_name; }
