/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:31:58 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/17 22:18:06 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/tsr_world.h"

void	world_create(t_world *world, t_vec3i size)
{
	t_vec3i	xyz;

	world->size = size;
	world->blocks = malloc(sizeof(t_tsr_tile)
			* world->size.x * world->size.y * world->size.z);
	if (!world->blocks)
		return ;

	xyz.x = 0;
	while (xyz.x < world->size.x)
	{
		xyz.y = 0;
		while (xyz.y < world->size.y)
		{
			xyz.z = 0;
			while (xyz.z < world->size.z)
			{
				block_set(world, xyz, (xyz.y == 0) * 2);
				xyz.z++;
			}
			xyz.y++;
		}
		xyz.x++;
	}
	block_set(world, vec3i(world->size.x / 2, 0, world->size.z / 2), 3);
}

void	world_destroy(t_world *world)
{
	free(world->blocks);
}

bool	inbound(t_world *world, t_vec3i pos)
{
	return (
		0 <= pos.x && pos.x < world->size.x
		&& 0 <= pos.y && pos.y < world->size.y
		&& 0 <= pos.z && pos.z < world->size.z
	);
}

t_tsr_tile_id	block_get(t_world *world, t_vec3i pos)
{
	return (world->blocks[
			pos.x + world->size.x * (pos.y + world->size.y * pos.z)]);
}

void	block_set(t_world *world, t_vec3i pos, t_tsr_tile_id type)
{
	if (!inbound(world, pos))
		return ;
	world->blocks[
		pos.x + world->size.x * (pos.y + world->size.y * pos.z)] = type;
}
