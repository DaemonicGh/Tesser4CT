/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 17:16:20 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/02 01:41:40 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	update_player_rotation(t_tsr *tsr)
{
	tsr->player.rotation.x = tsr->camera.rotation.x;
	tsr->player.forward = vec3(
			-sin(tsr->player.rotation.x), 0, -cos(tsr->player.rotation.x));
	tsr->player.right = vec3(
			-tsr->player.forward.z, 0, tsr->player.forward.x);
	tsr->player.up = vec3(0, 1, 0);
}

static void	update_player_chunk(t_tsr *tsr)
{
	const t_vec3	target = vec3_add(
			tsr->player.position, tsr->player.velocity);
	t_tsr_chunk_id	chunk;
	int				axis;

	axis = 3;
	while (axis--)
	{
		chunk = tsr->player.chunk;
		if (target.v[axis] < 0.1)
			chunk = tsr->world.chunk_refs[chunk].neighbors[axis * 2];
		else if (target.v[axis] >= 3.9)
			chunk = tsr->world.chunk_refs[chunk].neighbors[axis * 2 + 1];
		if (!chunk)
		{
			tsr->player.position.v[axis] = fclamp(target.v[axis], 0.1, 3.9);
			tsr->player.velocity.v[axis] = 0;
			continue ;
		}
		if (target.v[axis] < 0 || target.v[axis] >= 4)
			tsr->player.chunk = chunk;
		tsr->player.position.v[axis] = fwrap(target.v[axis], 0, 4);
	}
}

static void	tsr_player_movement(t_tsr *tsr)
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

void	tsr_update_player(t_tsr *tsr)
{
	update_player_rotation(tsr);
	if (!tsr_player_update_prompt(tsr))
	{
		tsr_player_movement(tsr);
		tsr_player_actions(tsr);
	}
}
