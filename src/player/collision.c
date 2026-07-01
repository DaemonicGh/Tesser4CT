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

static t_vec3x2	get_bounds(t_tsr *tsr, int axis)
{
	const double	velocity = tsr->player.velocity.v[axis];
	t_vec3x2		bounds;

	bounds.p1 = vec3_add(tsr->player.position, tsr->player.hitbox.p1);
	bounds.p2 = vec3_add(tsr->player.position, tsr->player.hitbox.p2);
	bounds.v[velocity < 0].v[axis] = bounds.v[velocity > 0].v[axis]
		+ fsign(velocity);
	bounds.v[velocity > 0].v[axis] += velocity;
	bounds = vec3x2(vec3_exec(floor, bounds.p1), vec3_exec(ceil, bounds.p2));
	return (bounds);
}

static void	resolve_collision(t_tsr *tsr, t_vec3i pos, int axis, double *t)
{
	const int		dir = fsign(tsr->player.velocity.v[axis]);
	t_tsr_chunk_id	chunk;
	t_tsr_tile_data	*data;
	double			target;
	t_vec3			dpos;

	dpos = vec3_vi(pos);
	chunk = tsr_relocate_chunk(&tsr->world, tsr->player.chunk, &dpos);
	if (!chunk)
		return ;
	data = &tsr->world_data.tiles[tsr->world.chunks[chunk].tiles
	[tsr_get_tile_index(vec3i_vd(dpos))].type];
	if (data->skip)
		return ;
	target = pos.v[axis] + (dir < 0);
	target -= tsr->player.hitbox.v[(dir > 0)].v[axis] + dir * 1e-4;
	target -= tsr->player.position.v[axis];
	*t = target;
}

static void	velocity_collision(t_tsr *tsr, int axis, double *t)
{
	const t_vec3x2	bounds = get_bounds(tsr, axis);
	t_vec3i			pos;

	if (tsr->player.velocity.v[axis] == 0)
		return ;
	pos.x = bounds.p1.x;
	while (pos.x < bounds.p2.x)
	{
		pos.y = bounds.p1.y;
		while (pos.y < bounds.p2.y)
		{
			pos.z = bounds.p1.z;
			while (pos.z < bounds.p2.z)
			{
				resolve_collision(tsr, pos, axis, t);
				pos.z++;
			}
			pos.y++;
		}
		pos.x++;
	}
}

static void	update_chunk(t_tsr *tsr, int axis)
{
	t_tsr_chunk_id	chunk;

	chunk = tsr->player.chunk;
	if (tsr->player.position.v[axis] < 0.1)
		chunk = tsr->world.chunks[chunk].neighbors[axis * 2];
	else if (tsr->player.position.v[axis] >= 3.9)
		chunk = tsr->world.chunks[chunk].neighbors[axis * 2 + 1];
	if (!chunk)
	{
		tsr->player.position.v[axis] = fclamp(
				tsr->player.position.v[axis], 0.1, 3.9);
		tsr->player.velocity.v[axis] *= 0.5;
		return ;
	}
	if (tsr->player.position.v[axis] < 0 || tsr->player.position.v[axis] >= 4)
		tsr->player.chunk = chunk;
	tsr->player.position.v[axis] = fwrap(tsr->player.position.v[axis], 0, 4);
}

void	player_collision(t_tsr *tsr)
{
	int				axis;
	double			t;

	axis = 0;
	while (axis < 3)
	{
		t = tsr->player.velocity.v[axis];
		velocity_collision(tsr, axis, &t);
		if (t != tsr->player.velocity.v[axis])
		{
			tsr->player.velocity.v[axis] *= 0.5;
			if (axis == 1 && tsr->player.velocity.y < 0)
			{
				tsr->player.air_time = 0;
				tsr->player.can_jump = true;
			}
		}
		tsr->player.position.v[axis] += t;
		update_chunk(tsr, axis);
		axis++;
	}
}
