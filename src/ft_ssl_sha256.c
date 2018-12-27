/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_sha256.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 20:32:07 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/26 13:14:50 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	sha256_block(unsigned int *digest, t_md5_block *block)
{
	int				i;
	unsigned int	w[64];
	t_sha256_vars	vars;

	i = 0;
	ft_bzero(&vars, sizeof(vars));
	sha256_build_w(block, w);
	sha256_block_init(digest, &vars);
	while (i < 64)
	{
		vars.bigs1 = sha256_rightrotate(vars.e, 6) ^
			sha256_rightrotate(vars.e, 11) ^ sha256_rightrotate(vars.e, 25);
		vars.ch = (vars.e & vars.f) ^ ((~vars.e) & vars.g);
		vars.temp1 = vars.h + vars.bigs1 + vars.ch + get_sha256_k(i) + w[i];
		vars.bigs0 = sha256_rightrotate(vars.a, 2) ^
			sha256_rightrotate(vars.a, 13) ^ sha256_rightrotate(vars.a, 22);
		vars.maj = (vars.a & vars.b) ^ (vars.a & vars.c) ^ (vars.b & vars.c);
		vars.temp2 = vars.bigs0 + vars.maj;
		sha256_rotate_vars(&vars);
		i++;
	}
	sha256_add_digest(digest, &vars);
}

int		sha256_set_flags_dispatch(int i, char **argv, int flags, char *str)
{
	if (argv[i][0] == '-' && !(flags & MD5_REAL_ARGS_FLAG))
	{
		flags |= md_arg_parse(argv[i], &str, "sha256", &sha256_dispatch);
		if (flags & MD5_S_FLAG && str)
		{
			sha256_dispatch(str, MD5_MODE_STR, flags);
			flags ^= MD5_S_FLAG;
			flags |= MD5_CMD_FLAG;
		}
	}
	else if (flags & MD5_S_FLAG && !str)
	{
		str = argv[i];
		sha256_dispatch(str, MD5_MODE_STR, flags);
		flags ^= MD5_S_FLAG;
		flags |= MD5_CMD_FLAG;
	}
	else
	{
		sha256_dispatch(argv[i], MD5_MODE_FILE, flags);
		flags |= MD5_CMD_FLAG + MD5_REAL_ARGS_FLAG;
	}
	return (flags);
}

int		sha256_handle(int args, char **argv)
{
	static int	flags = 0;
	char		*str;
	int			i;

	i = 0;
	str = NULL;
	while (i < args)
	{
		flags = sha256_set_flags_dispatch(i, argv, flags, str);
		i++;
	}
	if (flags & MD5_S_FLAG)
		sha256_no_str_err();
	if (!(flags & MD5_CMD_FLAG) && !(flags & MD5_P_FLAG)
		&& !(flags & MD5_S_FLAG))
		sha256_dispatch(NULL, MD5_MODE_STDIN, flags);
	return (0);
}

void	sha256_dispatch(char *str, int mode, int flags)
{
	unsigned int	digest[8];
	t_md5_in		in;

	ft_bzero(&in, sizeof(in));
	in.ssl_len_endian = &sha256_len_endian;
	if (mode == MD5_MODE_STR && (in.str = str))
		sha256_buf(in, digest);
	else if (mode == MD5_MODE_FILE)
	{
		if ((in.fd = open(str, O_RDONLY)) == -1)
		{
			ft_printf("sha256: %s: ", str);
			perror(0);
			return ;
		}
		sha256_buf(in, digest);
		close(in.fd);
	}
	else if (mode == MD5_MODE_STDIN)
	{
		in.flags = flags;
		sha256_buf(in, digest);
	}
	sha256_print(str, digest, flags, mode);
}

void	sha256_buf(t_md5_in in, unsigned int *digest)
{
	t_md5_block	block;
	uint64_t	len;
	int			count;
	int			close_flag;

	len = 0;
	close_flag = 0;
	sha256_init_digest(digest);
	if (in.str)
	{
		while ((count = get_next_block_str(in, &block, &len, &close_flag)))
		{
			sha256_block(digest, &block);
			in.str += count;
		}
	}
	else
	{
		while ((count = get_next_block_fd(in, &block, &len, &close_flag)))
			sha256_block(digest, &block);
	}
	sha256_block(digest, &block);
}
