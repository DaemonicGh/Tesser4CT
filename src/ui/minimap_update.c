/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 16:00:27 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/24 16:00:27 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_constants.h"

static void	tag_chunk(
	t_tsr_explore map[MINIMAP_SIZE][MINIMAP_SIZE][MINIMAP_SIZE],
	t_tsr_chunk_id chunk, t_vec3i pos, uint cycle)
{
	t_tsr_explore	*node;

	node = &map[pos.x][pos.y][pos.z];
	if (cycle < node->cycle)
	{
		node->id = chunk;
		node->cycle = cycle;
	}
	else if (node->cycle == cycle && node->id != chunk)
	{
		node->id = 0;
		node->error = true;
	}
}

static bool	explore_chunk(t_tsr *tsr,
	t_tsr_explore map[MINIMAP_SIZE][MINIMAP_SIZE][MINIMAP_SIZE],
	t_vec3i pos, uint cycle)
{
	t_tsr_explore	node;
	t_tsr_chunk		*chunk;
	t_vec3i			npos;
	uint			i;

	node = map[pos.x][pos.y][pos.z];
	if (node.cycle != cycle || !node.id)
		return (true);
	chunk = &tsr->world.chunks[node.id];
	i = 0;
	while (i < 3)
	{
		npos = pos;
		npos.v[i] -= 1;
		if (npos.v[i] >= 0)
			tag_chunk(map, chunk->neighbors[i * 2], npos, cycle + 1);
		npos = pos;
		npos.v[i] += 1;
		if (npos.v[i] < MINIMAP_SIZE)
			tag_chunk(map, chunk->neighbors[i * 2 + 1], npos, cycle + 1);
		i++;
	}
	return (false);
}

static bool	explore_minimap(t_tsr *tsr,
	t_tsr_explore map[MINIMAP_SIZE][MINIMAP_SIZE][MINIMAP_SIZE], uint cycle)
{
	t_vec3i		pos;
	bool		explored;

	explored = true;
	pos.x = 0;
	while (pos.x < MINIMAP_SIZE)
	{
		pos.y = 0;
		while (pos.y < MINIMAP_SIZE)
		{
			pos.z = 0;
			while (pos.z < MINIMAP_SIZE)
			{
				explored &= explore_chunk(tsr, map, pos, cycle);
				pos.z++;
			}
			pos.y++;
		}
		pos.x++;
	}
	return (explored);
}

static void	draw_minimap(t_tsr *tsr,
	t_tsr_explore map[MINIMAP_SIZE][MINIMAP_SIZE][MINIMAP_SIZE])
{
	t_vec3i	pos;

	pos.x = 0;
	while (pos.x < MINIMAP_SIZE)
	{
		pos.y = 0;
		while (pos.y < MINIMAP_SIZE)
		{
			pos.z = 0;
			while (pos.z < MINIMAP_SIZE)
			{
				if (map[pos.x][pos.y][pos.z].id)
					draw_chunk(tsr, map, pos);
				else if (map[pos.x][pos.y][pos.z].error)
					draw_chunk_error(tsr, pos);
				pos.z++;
			}
			pos.y++;
		}
		pos.x++;
	}
	mbx_set_region(tsr->ui.target,
		tsr->textures.gui.minimap_cursor, vec2i(312, 172));
}

void	update_and_draw_minimap(t_tsr *tsr)
{
	t_tsr_explore	map[MINIMAP_SIZE][MINIMAP_SIZE][MINIMAP_SIZE];
	uint			i;

	i = 0;
	while (i < MINIMAP_SIZE * MINIMAP_SIZE * MINIMAP_SIZE)
		((t_tsr_explore *)map)[i++] = (t_tsr_explore){.cycle = -1};
	map[MINIMAP_CENTER][MINIMAP_CENTER][MINIMAP_CENTER]
		= (t_tsr_explore){.id = tsr->player.chunk, .cycle = 0};
	i = 0;
	while (!explore_minimap(tsr, map, i))
		i++;
	draw_minimap(tsr, map);
}
