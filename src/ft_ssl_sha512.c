/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_sha512.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/24 17:48:49 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/26 15:28:00 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	sha512_block(uint64_t *digest, t_sha512_block *block)
{
	int				i;
	uint64_t		w[80];
	t_sha512_vars	vars;

	i = 0;
	ft_bzero(&vars, sizeof(vars));
	sha512_build_w(block, w);
	sha512_block_init(digest, &vars);
	while (i < 80)
	{
		vars.bigs1 = sha512_rightrotate(vars.e, 14) ^
			sha512_rightrotate(vars.e, 18) ^ sha512_rightrotate(vars.e, 41);
		vars.ch = (vars.e & vars.f) ^ ((~vars.e) & vars.g);
		vars.temp1 = vars.h + vars.bigs1 + vars.ch + get_sha512_k(i) + w[i];
		vars.bigs0 = sha512_rightrotate(vars.a, 28) ^
			sha512_rightrotate(vars.a, 34) ^ sha512_rightrotate(vars.a, 39);
		vars.maj = (vars.a & vars.b) ^ (vars.a & vars.c) ^ (vars.b & vars.c);
		vars.temp2 = vars.bigs0 + vars.maj;
		sha512_rotate_vars(&vars);
		i++;
	}
	sha512_add_digest(digest, &vars);
}

void	sha512_buf(t_md5_in in, uint64_t *digest)
{
	t_sha512_block	block;
	uint64_t		len;
	int				count;
	int				close_flag;

	len = 0;
	close_flag = 0;
	sha512_init_digest(digest);
	if (in.str)
	{
		while ((count = get_next_sha512block_str(in, &block,
			&len, &close_flag)))
		{
			sha512_block(digest, &block);
			in.str += count;
		}
	}
	else
	{
		while ((count = get_next_sha512block_fd(in, &block, &len, &close_flag)))
			sha512_block(digest, &block);
	}
	sha512_block(digest, &block);
}

void	sha512_dispatch(char *str, int mode, int flags)
{
	uint64_t digest[8];
	t_md5_in in;

	ft_bzero(&in, sizeof(in));
	in.ssl_len_endian = &sha256_len_endian;
	if (mode == MD5_MODE_STR && (in.str = str))
		sha512_buf(in, digest);
	else if (mode == MD5_MODE_FILE)
	{
		if ((in.fd = open(str, O_RDONLY)) == -1)
		{
			ft_printf("sha512: %s: ", str);
			perror(0);
			return ;
		}
		sha512_buf(in, digest);
		close(in.fd);
	}
	else if (mode == MD5_MODE_STDIN)
	{
		in.flags = flags;
		sha512_buf(in, digest);
	}
	sha512_print(str, digest, flags, mode);
}

int		sha512_handle(int args, char **argv)
{
	static int	flags = 0;
	char		*str;
	int			i;

	i = 0;
	str = NULL;
	while (i < args)
	{
		flags = sha512_set_flags_dispatch(i, argv, flags, str);
		i++;
	}
	if (flags & MD5_S_FLAG)
		sha512_no_str_err();
	if (!(flags & MD5_CMD_FLAG) && !(flags & MD5_P_FLAG) &&
		!(flags & MD5_S_FLAG))
		sha512_dispatch(NULL, MD5_MODE_STDIN, flags);
	return (0);
}
