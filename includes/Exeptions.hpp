/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exeptions.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:31:40 by arudy             #+#    #+#             */
/*   Updated: 2022/11/16 10:34:35 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <exception>

class Exeptions
{
	public:
		class example : public std::exception
		{
			const char	*what() const throw()
			{
				return "Exeption example";
			}
		};
};
