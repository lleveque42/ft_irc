/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:31:40 by arudy             #+#    #+#             */
/*   Updated: 2022/11/16 17:05:48 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <exception>

class Exception
{
	public:
		class getaddrinfo : public std::exception {
			const char	*what() const throw() {
				return "getadddrinfo() error: ";
			}
		};

		class socket : public std::exception {
			const char	*what() const throw() {
				return "socket() error: ";
			}
		};

		class bind : public std::exception {
			const char	*what() const throw() {
				return "bind() error: ";
			}
		};

		class listen : public std::exception {
			const char	*what() const throw() {
				return "listen() error: ";
			}
		};
};
