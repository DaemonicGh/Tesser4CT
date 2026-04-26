/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:26:42 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/26 16:31:27 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"
#include "tsr_core.h"

/*
static void	place_and_destroy(t_tsr *tsr)
{
	t_traversal		traversal;

	if (mbx_key_pressed(tsr->mbx, MBX_MOUSE_LEFT))
	{
		traversal = ray_traversal(
				tsr->camera.forward, tsr->player.position, tsr);
		block_set(&tsr->world, traversal.block, 0);
	}
	if (mbx_key_pressed(tsr->mbx, MBX_MOUSE_RIGHT))
	{
		traversal = ray_traversal(
			tsr->camera.forward, tsr->player.position, tsr);
		traversal.block = vec3i_sub(traversal.block,
			vec3i_vd(get_normal(uniform->forward_dir, traversal.axis)));
		block_set(&tsr->world, traversal.block, *type);
	}
	if (mbx_key_pressed(mbx, MBX_KEY_Q))
	{
		if (*type == 1)
			*type = 6;
		else
			(*type)--;
	}
	if (mbx_key_pressed(mbx, MBX_KEY_E))
		*type = *type % 6 + 1;
}*/

void	tsr_player_actions(t_tsr *tsr)
{
	(void)tsr;
}
