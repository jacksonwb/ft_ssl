/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_md5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/17 16:09:26 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/26 13:06:47 by jbeall           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void		md5_block(unsigned int *dgst, t_md5_block *block)
{
	t_md5_vars	vars;
	int			i;

	i = 0;
	md5_init_vars(dgst, &vars);
	while (i < 64)
	{
		md5_do_op(i, &vars);
		vars.f += vars.a + get_md5_k(i) + block->i[vars.g];
		vars.a = vars.d;
		vars.d = vars.c;
		vars.c = vars.b;
		vars.b += md5_leftrotate(vars.f, get_md5_s(i));
		++i;
	}
	md5_add_vars(dgst, &vars);
}

void		md5_buf(t_md5_in in, unsigned int *digest)
{
	t_md5_block	block;
	uint64_t	len;
	int			count;
	int			close_flag;

	len = 0;
	close_flag = 0;
	digest[0] = 0x67452301;
	digest[1] = 0xefcdab89;
	digest[2] = 0x98badcfe;
	digest[3] = 0x10325476;
	if (in.str)
	{
		while ((count = get_next_block_str(in, &block, &len, &close_flag)))
		{
			md5_block(digest, &block);
			in.str += count;
		}
	}
	else
	{
		while ((count = get_next_block_fd(in, &block, &len, &close_flag)))
			md5_block(digest, &block);
	}
	md5_block(digest, &block);
}

void		md5_print(char *str, unsigned int *digest, int flags, int mode)
{
	char	*c;

	c = (mode == MD5_MODE_STR) ? "\"" : "";
	if (str && !(flags & MD5_Q_FLAG) && !(flags & MD5_R_FLAG))
		ft_printf("MD5 (%s%s%s) = ", c, str, c);
	ft_printf("%08x%08x%08x%08x", md5_fb(digest[0]), md5_fb(digest[1]),
		md5_fb(digest[2]), md5_fb(digest[3]));
	if (str && !(flags & MD5_Q_FLAG) && (flags & MD5_R_FLAG))
		ft_printf(" %s%s%s", c, str, c);
	ft_printf("\n");
}

uint64_t	md5_len_endian(unsigned long long int *len)
{
	return (*len) * 8;
}

void		md5_dispatch(char *str, int mode, int flags)
{
	unsigned int	digest[4];
	t_md5_in		in;

	ft_bzero(&in, sizeof(in));
	in.ssl_len_endian = &md5_len_endian;
	if (mode == MD5_MODE_STR && (in.str = str))
		md5_buf(in, digest);
	else if (mode == MD5_MODE_FILE)
	{
		if ((in.fd = open(str, O_RDONLY)) == -1)
		{
			ft_printf("md5: %s: ", str);
			perror(0);
			return ;
		}
		md5_buf(in, digest);
		close(in.fd);
	}
	else if (mode == MD5_MODE_STDIN)
	{
		in.flags = flags;
		md5_buf(in, digest);
	}
	md5_print(str, digest, flags, mode);
}
