/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 09:03:50 by emarrot           #+#    #+#             */
/*   Updated: 2026/06/11 17:04:19 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

/*
static bool	tile_collision(t_tsr_player *player, t_vec3i tile)
{
	return ((player->position.x - player->hitbox.x < tile.x + 1)
		&& (player->position.x + player->hitbox.x > tile.x)
		&& (player->position.y - player->hitbox.y < tile.y + 1)
		&& (player->position.y + player->hitbox.y > tile.y)
		&& (player->position.z - player->hitbox.z < tile.z + 1)
		&& (player->position.z + player->hitbox.z > tile.z)
	);
}

static void	player_to_tile(t_tsr *tsr, t_vec3i tile)
{
	t_vec3			dist;
	double			d;
	int				axis;

	dist.x = tile.x + 1 - tsr->player.position.x + tsr->player.hitbox.x;
	dist.y = tile.y + 1 - tsr->player.position.y + tsr->player.hitbox.y;
	dist.z = tile.z + 1 - tsr->player.position.z + tsr->player.hitbox.z;
	d = tile.x - tsr->player.position.x - tsr->player.hitbox.x;
	if (-d < dist.x)
		dist.x = d;
	d = tile.y - tsr->player.position.y - tsr->player.hitbox.y;
	if (-d < dist.y)
		dist.y = d;
	d = tile.z - tsr->player.position.z - tsr->player.hitbox.z;
	if (-d < dist.z)
		dist.z = d;
	if (fabs(dist.z) < fabs(dist.x) && fabs(dist.z) < fabs(dist.y))
		axis = 2;
	else
		axis = (fabs(dist.y) < fabs(dist.x));
	tsr->player.position.v[axis] += dist.v[axis];
	tsr->player.velocity.v[axis] *= 0.5;
}

void	player_collision(t_tsr *tsr)
{
	const t_vec3i		player = vec3i_vd(
			vec3_exec(floor, tsr->player.chunk_position));
	static const int	size = 2;
	t_vec3i				pos;

	pos.x = player.x - size;
	while (pos.x <= player.x + size)
	{
		pos.y = player.y - size;
		while (pos.y <= player.y + size)
		{
			pos.z = player.z - size;
			while (pos.z <= player.z + size)
			{
				if (!tsr_get_tile(tsr, tsr->player.chunk, pos)->skip
					&& tile_collision(&tsr->player, pos))
					player_to_tile(tsr, pos);
				pos.z++;
			}
			pos.y++;
		}
		pos.x++;
	}
}
*/
