/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/14 16:10:24 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/26 12:39:31 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int		parse_cmd(char *arg)
{
	if (!ft_strcmp(arg, "md5"))
		return (0);
	else if (!ft_strcmp(arg, "sha256"))
		return (1);
	else if (!ft_strcmp(arg, "sha512"))
		return (2);
	else
	{
		ft_printf("ft_ssl: Error: '%s' is an invalid command.\n\n", arg);
		ft_printf("Standard commands:\n\n");
		ft_printf("Message Digest commands:\nmd5\nsha256\nsha512\n\n");
		ft_printf("Cipher commands:\n");
		exit(0);
	}
}

int		md_arg_parse(char *arg, char **str, char *name,
	void (*md) (char*, int, int))
{
	int i;
	int flags;

	i = 1;
	flags = 0;
	while (arg[i])
	{
		if (arg[i] == 'q')
			flags |= MD5_Q_FLAG;
		else if (arg[i] == 'r')
			flags |= MD5_R_FLAG;
		else if (arg[i] == 's')
			flags |= md5_set_string(str, arg, i);
		else if (arg[i] == 'p')
		{
			md(NULL, MD5_MODE_STDIN, MD5_P_FLAG);
			flags |= MD5_CMD_FLAG;
		}
		else
			md5_exit(arg, i, name);
		if (flags & MD5_P_FLAG || flags & MD5_S_FLAG)
			return (flags);
		i++;
	}
	return (flags);
}

int		md5_handle(int args, char **argv)
{
	static int	flags = 0;
	char		*str;
	int			i;

	i = 0;
	str = NULL;
	while (i < args)
	{
		flags = md5_set_flags_dispatch(i, argv, flags, str);
		i++;
	}
	if (flags & MD5_S_FLAG)
		md5_no_str_err();
	if (!(flags & MD5_CMD_FLAG) && !(flags & MD5_P_FLAG)
		&& !(flags & MD5_S_FLAG))
		md5_dispatch(NULL, MD5_MODE_STDIN, flags);
	return (0);
}

int		call_handler(int cmd, int args, char **argv)
{
	static int (*handlers[3])(int args, char **argv) = {
			&md5_handle,
			&sha256_handle,
			&sha512_handle
	};

	return (handlers[cmd](args, argv));
}

int		main(int argc, char **argv)
{
	int status;

	if (argc == 1)
	{
		ft_printf("usage: ft_ssl command [command opts] [command args]\n");
		exit(0);
	}
	else
		status = call_handler(parse_cmd(argv[1]), argc - 2, argv + 2);
	return (0);
}
