/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 17:16:20 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/26 16:46:28 by rprieur          ###   ########.fr       */
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

void	tsr_update_player(t_tsr *tsr)
{
	update_player_rotation(tsr);
	tsr_player_movement(tsr);
	tsr_player_actions(tsr);
}
