/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 22:22:53 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/19 22:22:53 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"

static void	update_timeouts(t_tsr *tsr)
{
	t_tsr_tile		*tile;
	t_tsr_tile_data	*data;
	uint			i;
	uint			j;

	i = 0;
	while (i < tsr->world.chunk_count)
	{
		j = 0;
		while (j < 64)
		{
			tile = &tsr->world.chunks[i].tiles[j];
			data = &tsr->world_data.tiles[tile->type];
			if (data->timeout_to != (t_tsr_tile_id) - 1
				&& tsr->mbx->now - tile->timer > data->timeout_time)
			{
				tile->type = data->timeout_to;
				tile->timer = tsr->mbx->now;
			}
			j++;
		}
		i++;
	}
}

void	tsr_world_update(t_tsr *tsr)
{
	update_timeouts(tsr);
	clear_light(tsr);
}

void	tsr_tile_interact(
	t_tsr *tsr, t_tsr_tile *tile)
{
	const t_tsr_tile_data	*data = &tsr->world_data.tiles[tile->type];

	if (data->interact_to == (t_tsr_tile_id)-1)
		return ;
	tile->type = data->interact_to;
	tile->timer = tsr->mbx->now;
}
