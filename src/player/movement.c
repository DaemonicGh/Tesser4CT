/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:26:30 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/26 16:53:06 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

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
	tsr->player.position = vec3_clamp(vec3_add(
				tsr->player.position, tsr->player.velocity),
			vec3_d(0.2), vec3_sub_d(vec3_vi(tsr->wworld.size), 0.2));
}
