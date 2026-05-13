/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atlas.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:11:23 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/13 18:38:27 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	atlas_init(t_atlas *atlas, t_mbx *mbx)
{
	size_t	i;

	atlas->mbx = mbx;
	i = 0;
	while (i < ATLAS_LENGTH)
		atlas->key[i++] = NULL;
}

t_pbr	*atlas_get(t_atlas *atlas, const char *key)
{
	size_t	i;

	if (!atlas || !key)
		return (NULL);
	i = 0;
	while (i < ATLAS_LENGTH && atlas->key[i])
	{
		if (!ft_strcmp(atlas->key[i], key))
			return (&atlas->pbr[i]);
		i++;
	}
	return (NULL);
}

t_pbr	*atlas_index(t_atlas *atlas, size_t i)
{
	if (!atlas)
		return (NULL);
	if (i >= ATLAS_LENGTH)
		return (NULL);
	return (&atlas->pbr[i]);
}

static char *get_extension(const char *path)
{
	char	*ext;
	size_t	i;
	size_t	len;

	i = 0;
	while (path[i] && path[i] != '.')
		i++;
	if (!path[i])
		return (NULL);
	len = ft_strlen(path + i);
	ext = malloc(len + 1);
	if (!ext)
		return (NULL);
	ext[len] = 0;
	while (len--)
		ext[len] = path[i + len];
	return (ext);
}

static char *get_name(const char *path)
{
	char	*name;
	size_t	i;
	size_t	j;
	size_t	len;

	i = 0;
	while (path[i] && path[i] != '.')
		i++;
	j = i;
	while (j > 0 && path[j - 1] != '/')
		j--;
	len = i - j;
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	name[len] = 0;
	while (len--)
		name[len] = path[j + len];
	return (name);
}

void	atlas_add(t_atlas *atlas, const char *path)
{
	size_t	i;
	char	*path_n;
	char	*ext;
	char	*key;

	if (!atlas || !path)
		return ;
	i = 0;
	while (i < ATLAS_LENGTH && atlas->key[i])
		i++;
	if (i == ATLAS_LENGTH)
		return ;
	key = get_name(path);
	if (!key)
		return ;
	atlas->pbr[i].col_tex = mbx_make_region_from_file(atlas->mbx, (char *)path);
	if (atlas->pbr[i].col_tex)
		atlas->key[i] = key;
	ext = get_extension(path);
	path_n = malloc(ft_strlen(path) + 3);
	path_n[0] = 0;
	ft_strcat(path_n, path);
	path_n[ft_strlen(path) - 4] = 0;
	ft_strcat(path_n, "_n");
	if (ext)
		ft_strcat(path_n, ext);
	atlas->pbr[i].nrm_tex = NULL;
	if (!access(path_n, F_OK) && ext)
		atlas->pbr[i].nrm_tex = mbx_make_region_from_file(atlas->mbx, path_n);
	free(ext);
	free(path_n);
}

void	atlas_pop(t_atlas *atlas, const char *key)
{
	size_t	i;

	if (!atlas || !key)
		return ;
	i = 0;
	while (i < ATLAS_LENGTH && ft_strcmp(atlas->key[i], key))
		i++;
	if (i == ATLAS_LENGTH)
		return ;
	mbx_destroy_region(atlas->mbx, atlas->pbr[i].col_tex);
	if (atlas->pbr[i].nrm_tex)
		mbx_destroy_region(atlas->mbx, atlas->pbr[i].nrm_tex);
	free(atlas->key[i]);
	atlas->key[i] = NULL;
}
