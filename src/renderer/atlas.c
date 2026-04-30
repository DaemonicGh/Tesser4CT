/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atlas.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:11:23 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/30 16:28:56 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr_atlas.h"

static int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((uint8_t)s1[i] - (uint8_t)s2[i]);
}

void	atlas_init(t_atlas *atlas, t_mbx *mbx)
{
	size_t	i;

	atlas->mbx = mbx;
	i = 0;
	while (i < ATLAS_LENGTH)
		atlas->key[i++] = NULL;
}

t_mbx_region	*atlas_get(t_atlas *atlas, const char *key)
{
	size_t	i;

	if (!atlas || !key)
		return (NULL);
	i = 0;
	while (i < ATLAS_LENGTH && (!atlas->key[i] || ft_strcmp(atlas->key[i], key)))
		i++;
	if (i == ATLAS_LENGTH)
		return (NULL);
	return (atlas->tex[i]);
}

t_mbx_region	*atlas_index(t_atlas *atlas, size_t i)
{
	if (!atlas)
		return (NULL);
	if (i >= ATLAS_LENGTH)
		return (NULL);
	return (atlas->tex[i]);
}

void	atlas_add(t_atlas *atlas, const char *key, const char *path)
{
	size_t	i;

	if (!atlas || !key || !path)
		return ;
	i = 0;
	while (i < ATLAS_LENGTH && atlas->key[i])
		i++;
	if (i == ATLAS_LENGTH)
		return ;
	atlas->tex[i] = mbx_make_region_from_file(atlas->mbx, (char *)path);
	if (atlas->tex[i])
		atlas->key[i] = (char *)key;
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
	mbx_destroy_region(atlas->mbx, atlas->tex[i]);
	atlas->key[i] = NULL;
}
