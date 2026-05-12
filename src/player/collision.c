/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 09:03:50 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/07 16:07:02 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

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

static void	player_to_tile(t_tsr_player *player, t_vec3i tile)
{
	t_vec3	dist;
	double	d;

	dist.x = tile.x + 1 - player->position.x + player->hitbox.x;
	dist.y = tile.y + 1 - player->position.y + player->hitbox.y;
	dist.z = tile.z + 1 - player->position.z + player->hitbox.z;
	d = tile.x - player->position.x - player->hitbox.x;
	if (-d < dist.x)
		dist.x = d;
	d = tile.y - player->position.y - player->hitbox.y;
	if (-d < dist.y)
		dist.y = d;
	d = tile.z - player->position.z - player->hitbox.z;
	if (-d < dist.z)
		dist.z = d;
	if (fabs(dist.x) < fabs(dist.y) && fabs(dist.x) < fabs(dist.z))
	{
		player->position.x += dist.x;
		player->velocity.x *= 0.5;
		return ;
	}
	if (fabs(dist.y) < fabs(dist.z))
	{
		player->position.y += dist.y;
		player->velocity.y *= 0.5;
		return ;
	}
	player->position.z += dist.z;
	player->velocity.z *= 0.5;
}

void	player_collision(t_tsr *tsr)
{
	const t_vec3i		player = vec3i_vd(
			vec3_exec(floor, tsr->player.position));
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
				if (block_get(&tsr->wworld, pos)
					&& tile_collision(&tsr->player, pos))
					player_to_tile(&tsr->player, pos);
				pos.z++;
			}
			pos.y++;
		}
		pos.x++;
	}
}
