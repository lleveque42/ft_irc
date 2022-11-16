/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 13:20:06 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/16 13:51:02 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

#define ARG_ERR "ircserv: 2 arguments needed:\n\nUsage is: ./ircserv <port> <password>\n\t<port>: port number on wich your IRC server will be listening to for incoming IRC connections\n\t<password>: it will be needed by any IRC client that tries to connect to your server"
#define PORT_ERR  "Invalid port, use only number between "
#define PASSWORD_ERR "Password must not be empty"
