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

void	atlas_init(t_atlas *atlas)
{
	size_t	i;

	i = 0;
	while (i < ATLAS_LENGTH)
		atlas->key[i++] = NULL;
}

t_tsr_texture	*atlas_get(t_atlas *atlas, const char *key)
{
	size_t	i;

	if (!atlas || !key)
		return (NULL);
	i = 0;
	while (i < ATLAS_LENGTH && atlas->key[i])
	{
		if (!ft_strcmp(atlas->key[i], key))
			return (&atlas->texture[i]);
		i++;
	}
	return (NULL);
}

static char	*get_extension(const char *path)
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

static char	*get_name(const char *path)
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

void	atlas_add(t_tsr *tsr, t_atlas *atlas, const char *path)
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
	atlas->texture[i].tx = mbx_make_region_from_file(tsr->mbx, (char *)path);
	if (atlas->texture[i].tx)
		atlas->key[i] = key;
	ext = get_extension(path);
	path_n = malloc(ft_strlen(path) + 3);
	path_n[0] = 0;
	ft_strcat(path_n, path);
	path_n[ft_strlen(path) - 4] = 0;
	ft_strcat(path_n, "_n");
	if (ext)
		ft_strcat(path_n, ext);
	atlas->texture[i].nrm = NULL;
	if (!access(path_n, F_OK) && ext)
		atlas->texture[i].nrm = mbx_make_region_from_file(tsr->mbx, path_n);
	free(ext);
	free(path_n);
}
