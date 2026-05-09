/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atlas.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:11:23 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/09 13:08:24 by emarrot          ###   ########.fr       */
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
	while (i < ATLAS_LENGTH
		&& (!atlas->key[i] || ft_strcmp(atlas->key[i], key)))
		i++;
	if (i == ATLAS_LENGTH)
		return (NULL);
	return (&atlas->pbr[i]);
}

t_pbr	*atlas_index(t_atlas *atlas, size_t i)
{
	if (!atlas)
		return (NULL);
	if (i >= ATLAS_LENGTH)
		return (NULL);
	return (&atlas->pbr[i]);
}

void	atlas_add(t_atlas *atlas, const char *key, const char *path)
{
	size_t	i;
	char	*path_n;

	if (!atlas || !key || !path)
		return ;
	i = 0;
	while (i < ATLAS_LENGTH && atlas->key[i])
		i++;
	if (i == ATLAS_LENGTH)
		return ;
	atlas->pbr[i].col_tex = mbx_make_region_from_file(atlas->mbx, (char *)path);
	if (atlas->pbr[i].col_tex)
		atlas->key[i] = (char *)key;
	path_n = malloc(ft_strlen(path) + 3);
	path_n[0] = 0;
	ft_strcat(path_n, path);
	path_n[ft_strlen(path) - 4] = 0;
	ft_strcat(path_n, "_n");
	ft_strcat(path_n, ".png");
	atlas->pbr[i].nrm_tex = NULL;
	if (!access(path_n, F_OK))
		atlas->pbr[i].nrm_tex = mbx_make_region_from_file(atlas->mbx, path_n);
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
	atlas->key[i] = NULL;
}
