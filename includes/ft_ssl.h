/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jackson <jbeall@student.42.us.org>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/14 16:10:08 by jbeall            #+#    #+#             */
/*   Updated: 2018/12/27 10:29:13 by jackson          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H
# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include "libft.h"
# include "ft_printf.h"

# define MD5_Q_FLAG 1
# define MD5_R_FLAG 2
# define MD5_S_FLAG 4
# define MD5_P_FLAG 8
# define MD5_CMD_FLAG 16
# define MD5_REAL_ARGS_FLAG 32

# define MD5_MODE_STR 1
# define MD5_MODE_FILE 2
# define MD5_MODE_STDIN 3

typedef union		u_md5_block
{
	unsigned char	c[64];
	unsigned int	i[16];
	uint64_t		u64[8];
}					t_md5_block;

typedef union		u_sha512_block
{
	unsigned char	c[128];
	uint64_t		i[16];
}					t_sha512_block;

typedef struct		s_md5_in
{
	char			*str;
	int				fd;
	int				flags;
	uint64_t		(*ssl_len_endian)(uint64_t*);
}					t_md5_in;

typedef struct		s_md5_vars
{
	unsigned int	a;
	unsigned int	b;
	unsigned int	c;
	unsigned int	d;
	unsigned int	f;
	unsigned int	g;
	unsigned int	i;
}					t_md5_vars;

typedef struct		s_sha256_vars
{
	unsigned int	a;
	unsigned int	b;
	unsigned int	c;
	unsigned int	d;
	unsigned int	e;
	unsigned int	f;
	unsigned int	g;
	unsigned int	h;
	unsigned int	bigs0;
	unsigned int	bigs1;
	unsigned int	ch;
	unsigned int	maj;
	unsigned int	temp1;
	unsigned int	temp2;
}					t_sha256_vars;

typedef struct		s_sha512_vars
{
	uint64_t		a;
	uint64_t		b;
	uint64_t		c;
	uint64_t		d;
	uint64_t		e;
	uint64_t		f;
	uint64_t		g;
	uint64_t		h;
	uint64_t		bigs0;
	uint64_t		bigs1;
	uint64_t		ch;
	uint64_t		maj;
	uint64_t		temp1;
	uint64_t		temp2;
}					t_sha512_vars;

/*
** FT_SSL
*/
int					parse_cmd(char *arg);
int					call_handler(int cmd, int args, char **argv);
/*
** MD5
*/
int					md_arg_parse(char *arg, char **str, char *name,
	void(*md)(char*, int, int));
int					md5_handle(int args, char **argv);
void				md5_dispatch(char *str, int mode, int flags);
int					get_next_block_str(t_md5_in in, t_md5_block *block,
	uint64_t *len, int *close_flag);
int					get_next_block_fd(t_md5_in in, t_md5_block *block,
	uint64_t *len, int *close_flag);
unsigned int		md5_fb(unsigned int digest);
void				md5_exit(char *arg, int i, char *name);
int					md5_set_string(char **str, char *arg, int i);
void				md5_no_str_err(void);
int					md5_set_flags_dispatch(int i, char **argv,
	int flags, char *str);
unsigned int		md5_fb(unsigned int digest);
void				md5_block(unsigned int *dgst, t_md5_block *block);
void				md5_buf(t_md5_in in, unsigned int *digest);
void				md5_print(char *str, unsigned int *digest,
	int flags, int mode);
uint64_t			md5_len_endian(uint64_t *len);
void				gnb_helper1(t_md5_in *in, t_md5_block *block,
	int *count, int *i);
unsigned int		get_md5_s(int i);
unsigned int		get_md5_k(int i);
unsigned int		md5_leftrotate(unsigned int x, unsigned int c);
unsigned int		md5_f(unsigned int b, unsigned int c, unsigned int d);
unsigned int		md5_g(unsigned int b, unsigned int c, unsigned int d);
unsigned int		md5_h(unsigned int b, unsigned int c, unsigned int d);
unsigned int		md5_i(unsigned int b, unsigned int c, unsigned int d);
void				md5_init_vars(unsigned int *dgst, t_md5_vars *vars);
void				md5_add_vars(unsigned int *dgst, t_md5_vars *vars);
void				md5_do_op(int i, t_md5_vars *vars);

/*
** SHA-256
*/
int					sha256_handle(int args, char **argv);
void				sha256_dispatch(char *str, int mode, int flags);
void				sha256_buf(t_md5_in in, unsigned int *digest);
uint64_t			sha256_len_endian(uint64_t *len);
void				sha256_block(unsigned int *digest, t_md5_block *block);
int					sha256_set_flags_dispatch(int i, char **argv, int flags,
	char *str);
unsigned int		sha256_rightrotate(unsigned int x, unsigned int c);
void				sha256_build_w(t_md5_block *block, unsigned int *w);
void				sha256_block_init(unsigned int *digest,
	t_sha256_vars *vars);
void				sha256_add_digest(unsigned int *digest,
	t_sha256_vars *vars);
unsigned int		get_sha256_k(int i);
void				sha256_rotate_vars(t_sha256_vars *vars);
void				sha256_no_str_err(void);
uint64_t			sha256_len_endian(uint64_t *len);
void				sha256_init_digest(unsigned int *digest);
void				sha256_print(char *str, unsigned int *digest,
	int flags, int mode);

/*
** SHA-512
*/
int					sha512_handle(int args, char **argv);
void				sha512_block(uint64_t *digest, t_sha512_block *block);
void				sha512_buf(t_md5_in in, uint64_t *digest);
void				sha512_dispatch(char *str, int mode, int flags);
int					get_next_sha512block_str(t_md5_in in, t_sha512_block *block,
	uint64_t *len, int *close_flag);
int					get_next_sha512block_fd(t_md5_in in, t_sha512_block *block,
	uint64_t *len, int *close_flag);
void				sha512_init_digest(uint64_t *digest);
void				sha512_no_str_err(void);
int					sha512_set_flags_dispatch(int i, char **argv, int flags,
	char *str);
void				sha512_print(char *str, uint64_t *digest, int flags,
	int mode);
uint64_t			sha512_rightrotate(uint64_t x, unsigned int c);
uint64_t			get_sha512_k(int i);
uint64_t			sha512_fb(uint64_t digest);
void				sha512_build_w(t_sha512_block *block, uint64_t *w);
void				sha512_block_init(uint64_t *digest, t_sha512_vars *vars);
void				sha512_add_digest(uint64_t *digest, t_sha512_vars *vars);
void				sha512_rotate_vars(t_sha512_vars *vars);
void				gnb_sha512_close_block(t_sha512_block *block, int i,
	int *close_flag);
void				gnb_sha512_pad_block(t_sha512_block *block, int *close_flag,
	int i);

#endif
