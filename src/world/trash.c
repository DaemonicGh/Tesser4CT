/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trash.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 17:43:18 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/15 17:43:18 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"
#include "tsr_world.h"
#include <stdlib.h>

static void	explore_chunk(
	t_tsr *tsr, t_tsr_chunk_id chunk, t_tsr_trash_data *data)
{
	t_tsr_chunk_id	neighbor;
	size_t			i;

	data[chunk].explored = true;
	i = 0;
	while (i < 6)
	{
		neighbor = tsr->world.chunks[chunk].neighbors[i];
		if (!data[neighbor].explored)
			explore_chunk(tsr, neighbor, data);
		i++;
	}
}

static void	reassign_chunks(t_tsr *tsr, t_tsr_trash_data *explored)
{
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	while (i < tsr->world.chunk_count)
	{
		if (explored[i].explored)
		{
			explored[i].new_id = j;
			tsr->world.chunks[j] = tsr->world.chunks[i];
			j++;
		}
		i++;
	}
	tsr->world.chunk_count = j;
	while (j < i)
		tsr->world.chunks[j++] = tsr->world.chunks[0];
}

static void	fix_ids(t_tsr *tsr, t_tsr_trash_data *explored)
{
	t_tsr_chunk_id	*neighbors;
	size_t			i;
	size_t			j;

	i = 1;
	while (i < tsr->world.chunk_count)
	{
		neighbors = tsr->world.chunks[i].neighbors;
		j = 0;
		while (j < 6)
		{
			neighbors[j] = explored[neighbors[j]].new_id;
			j++;
		}
		i++;
	}
	tsr->player.chunk = explored[tsr->player.chunk].new_id;
	tsr->camera.chunk = explored[tsr->camera.chunk].new_id;
	tsr->rendering.data.camera.chunk = tsr->camera.chunk;
}

static t_tsr_trash_data	*alloc_trash_data(t_tsr *tsr)
{
	t_tsr_trash_data	*explored;
	size_t				i;

	explored = malloc(tsr->world.chunk_count * sizeof(t_tsr_trash_data));
	if (!explored)
		return (NULL);
	explored[0].explored = true;
	i = 1;
	while (i < tsr->world.chunk_count)
		explored[i++].explored = false;
	return (explored);
}

void	trash_chunks(t_tsr *tsr)
{
	t_tsr_trash_data	*explored;

	explored = alloc_trash_data(tsr);
	if (!explored)
		return ;
	explore_chunk(tsr, tsr->player.chunk, explored);
	reassign_chunks(tsr, explored);
	fix_ids(tsr, explored);
	free(explored);
}
