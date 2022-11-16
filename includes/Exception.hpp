/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:31:40 by arudy             #+#    #+#             */
/*   Updated: 2022/11/16 15:19:39 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <exception>

class Exception
{
	public:
		class example : public std::exception
		{
			const char	*what() const throw()
			{
				return "Exeption example";
			}
		};
		class getaddrinfo : public std::exception
		{
			const char	*what() const throw()
			{
				return "Get adress info error";
			}
		};
};
