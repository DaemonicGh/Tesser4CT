/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:26:30 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/10 23:49:24 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	update_chunk_dir(t_tsr *tsr, double target, int axis)
{
	t_tsr_chunk		*neighbor;

	if (target < tsr->player.hitbox.v[axis])
	{
		neighbor = tsr_get_chunk_neighbor(tsr, tsr->player.chunk, axis * 2);
		if (!neighbor)
			target = tsr->player.hitbox.v[axis];
		else if (target < 0)
		{
			tsr->player.chunk = neighbor;
			target += 4;
		}
	}
	else if (target >= 4 - tsr->player.hitbox.v[axis])
	{
		neighbor = tsr_get_chunk_neighbor(tsr, tsr->player.chunk, axis * 2 + 1);
		if (!neighbor)
			target = 4 - tsr->player.hitbox.v[axis];
		else if (target >= 4)
		{
			tsr->player.chunk = neighbor;
			target -= 4;
		}
	}
	tsr->player.chunk_position.v[axis] = target;
}

void	update_player_chunk(t_tsr *tsr)
{
	const t_vec3	target = vec3_add(
			tsr->player.chunk_position, tsr->player.velocity);

	update_chunk_dir(tsr, target.x, 0);
	update_chunk_dir(tsr, target.z, 2);
	update_chunk_dir(tsr, target.y, 1);
}

void	tsr_player_movement(t_tsr *tsr)
{
	const double	speed = 0.3;
	const double	accel = 1;
	t_vec3			mv;

	mv = vec3_zero();
	if (mbx_btn(tsr->mbx, MBX_KEY_W))
		mv = vec3_add(mv, tsr->player.forward);
	if (mbx_btn(tsr->mbx, MBX_KEY_S))
		mv = vec3_sub(mv, tsr->player.forward);
	if (mbx_btn(tsr->mbx, MBX_KEY_D))
		mv = vec3_add(mv, tsr->player.right);
	if (mbx_btn(tsr->mbx, MBX_KEY_A))
		mv = vec3_sub(mv, tsr->player.right);
	if (mbx_btn(tsr->mbx, MBX_KEY_SPACE))
		mv = vec3_add(mv, tsr->player.up);
	if (mbx_btn(tsr->mbx, MBX_KEY_LSHIFT))
		mv = vec3_sub(mv, tsr->player.up);
	mv = vec3_mult_d(vec3_normalize(mv), speed);
	tsr->player.velocity = vec3_len_move_towards(tsr->player.velocity,
			mv, accel * tsr->mbx->dt);
	update_player_chunk(tsr);
	//player_collision(tsr);
}
