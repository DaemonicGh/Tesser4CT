/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   world.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:31:58 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/24 16:05:51 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	world_create(t_world *world, size_t width, size_t height, size_t depth)
{
	world->width = width;
	world->height = height;
	world->depth = depth;
	world->blocks = malloc(
			world->width * world->height * world->depth * sizeof(t_tsr_tile));
}

void	world_destroy(t_world *world)
{
	free(world->blocks);
}

bool	inbound(t_world *world, t_vec3i pos)
{
	return (
		0 <= pos.x && pos.x < (int)world->width
		&& 0 <= pos.y && pos.y < (int)world->height
		&& 0 <= pos.z && pos.z < (int)world->depth
	);
}

t_tsr_tile	block_get(t_world *world, t_vec3i pos)
{
	if (!inbound(world, pos))
		return (VOID);
	return (
		world->blocks[pos.x + world->width * (pos.y + world->height * pos.z)]);
}

void	block_set(t_world *world, t_vec3i pos, t_tsr_tile type)
{
	if (!inbound(world, pos))
		return ;
	world->blocks[
		pos.x + world->width * (pos.y + world->height * pos.z)] = type;
}
