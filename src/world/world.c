/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:31:58 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/26 16:34:36 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/tsr_world.h"

void	world_create(t_world *world, t_vec3i size)
{
	world->size = size;
	world->blocks = malloc(world->size.x * world->size.y * world->size.z
			* sizeof(t_tsr_tile));
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
