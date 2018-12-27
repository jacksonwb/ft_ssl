/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/26 12:36:20 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/26 15:49:32 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void			md5_exit(char *arg, int i, char *name)
{
	ft_printf("%s: illegal option -- %c\nusage:\
	%s [-pqr] [-s string] [files ...]\n", name, arg[i], name);
	exit(0);
}

int				md5_set_string(char **str, char *arg, int i)
{
	*str = arg[i + 1] ? arg + i + 1 : NULL;
	return (MD5_S_FLAG);
}

void			md5_no_str_err(void)
{
	ft_printf("md5 option requires an argument -- s\n");
	ft_printf("usage: md5 [-pqr] [-s string] [files...]\n");
	exit(0);
}

int				md5_set_flags_dispatch(int i, char **argv, int flags, char *str)
{
	if (argv[i][0] == '-' && !(flags & MD5_REAL_ARGS_FLAG))
	{
		flags |= md_arg_parse(argv[i], &str, "md5", &md5_dispatch);
		if (flags & MD5_S_FLAG && str)
		{
			md5_dispatch(str, MD5_MODE_STR, flags);
			flags ^= MD5_S_FLAG;
			flags |= MD5_CMD_FLAG;
		}
	}
	else if (flags & MD5_S_FLAG && !str)
	{
		str = argv[i];
		md5_dispatch(str, MD5_MODE_STR, flags);
		flags ^= MD5_S_FLAG;
		flags |= MD5_CMD_FLAG;
	}
	else
	{
		md5_dispatch(argv[i], MD5_MODE_FILE, flags);
		flags |= MD5_CMD_FLAG + MD5_REAL_ARGS_FLAG;
	}
	return (flags);
}

/*
** reverse endianess
*/

unsigned int	md5_fb(unsigned int digest)
{
	int				j;
	unsigned int	temp;

	j = 0;
	temp = 0;
	while (j < 4)
	{
		temp <<= 8;
		temp += (digest & 255);
		digest >>= 8;
		++j;
	}
	return (temp);
}
