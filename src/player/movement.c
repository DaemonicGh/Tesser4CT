/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 22:00:38 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/11 22:00:38 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	player_side(t_tsr *tsr)
{
	const t_vec3	dirs[3] = {
		tsr->camera.right, tsr->camera.up, tsr->camera.forward};
	const t_vec3	abs_dirs[3] = {
		vec3_abs(dirs[0]), vec3_abs(dirs[1]), vec3_abs(dirs[2])};
	int				i;

	i = 0;
	while (i < 3)
	{
		if (abs_dirs[i].z > abs_dirs[i].x
			&& abs_dirs[i].z > abs_dirs[i].y)
			tsr->player.face.v[i] = 4 + (dirs[i].z > 0);
		else if (abs_dirs[i].y > abs_dirs[i].x)
			tsr->player.face.v[i] = 2 + (dirs[i].y > 0);
		else
			tsr->player.face.v[i] = (dirs[i].x > 0);
		i++;
	}
}

void	player_rotation(t_tsr *tsr)
{
	tsr->player.rotation.x = tsr->camera.rotation.x;
	tsr->player.forward = vec3(
			-sin(tsr->player.rotation.x), 0, -cos(tsr->player.rotation.x));
	tsr->player.right = vec3(
			-tsr->player.forward.z, 0, tsr->player.forward.x);
	tsr->player.up = vec3(0, 1, 0);
	player_side(tsr);
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
			chunk = tsr->world.chunks[chunk].neighbors[axis * 2];
		else if (target.v[axis] >= 3.9)
			chunk = tsr->world.chunks[chunk].neighbors[axis * 2 + 1];
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

void	player_movement(t_tsr *tsr)
{
	const double	speed = 0.15;
	const double	accel = 0.8;
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
}
