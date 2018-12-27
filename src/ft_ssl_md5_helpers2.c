/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5_helpers2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/26 12:40:29 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/26 15:47:29 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

inline unsigned int	md5_leftrotate(unsigned int x, unsigned int c)
{
	return ((x << c) | (x >> (32 - c)));
}

inline unsigned int	md5_f(unsigned int b,
	unsigned int c, unsigned int d)
{
	return ((b & c) | ((~b) & d));
}

inline unsigned int	md5_g(unsigned int b,
	unsigned int c, unsigned int d)
{
	return ((d & b) | ((~d) & c));
}

inline unsigned int	md5_h(unsigned int b,
	unsigned int c, unsigned int d)
{
	return (b ^ c ^ d);
}

inline unsigned int	md5_i(unsigned int b,
	unsigned int c, unsigned int d)
{
	return (c ^ (b | (~d)));
}
