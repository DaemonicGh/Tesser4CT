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

#include "modules/mbx_utils.h"
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

static void	parkour_movement(t_tsr *tsr)
{
	tsr->player.velocity.y -= 0.8 * tsr->mbx->dt;
	tsr->player.air_time += tsr->mbx->dt;
	if (tsr->player.air_time > 0.2)
		tsr->player.can_jump = false;
	if (tsr->player.can_jump && tsr->mbx->inputs[MBX_KEY_SPACE].press < 0.2)
	{
		tsr->player.can_jump = false;
		tsr->player.is_jumping = true;
		tsr->player.velocity.y += 0.3;
	}
	if (tsr->player.is_jumping && tsr->player.velocity.y < 0)
		tsr->player.is_jumping = false;
	else if (tsr->player.is_jumping && tsr->player.velocity.y > 0
		&& !mbx_btn(tsr->mbx, MBX_KEY_SPACE))
		tsr->player.velocity.y *= 0.7;
}

void	player_movement(t_tsr *tsr)
{
	t_vec3		mv;

	mv = vec3_zero();
	if (mbx_btn(tsr->mbx, MBX_KEY_W))
		mv = vec3_add(mv, tsr->player.forward);
	if (mbx_btn(tsr->mbx, MBX_KEY_S))
		mv = vec3_sub(mv, tsr->player.forward);
	if (mbx_btn(tsr->mbx, MBX_KEY_D))
		mv = vec3_add(mv, tsr->player.right);
	if (mbx_btn(tsr->mbx, MBX_KEY_A))
		mv = vec3_sub(mv, tsr->player.right);
	mv = vec3_normalize(mv);
	if (tsr->player.godmode)
	{
		if (mbx_btn(tsr->mbx, MBX_KEY_SPACE))
			mv = vec3_add(mv, tsr->player.up);
		if (mbx_btn(tsr->mbx, MBX_KEY_LSHIFT))
			mv = vec3_sub(mv, tsr->player.up);
	}
	mv = vec3_mult_d(mv, tsr->player.speed);
	tsr->player.velocity = vec3_len_move_towards(tsr->player.velocity,
			mv, tsr->player.speed * 4 * tsr->mbx->dt);
	if (!tsr->player.godmode)
		parkour_movement(tsr);
	player_collision(tsr);
}
