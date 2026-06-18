/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 17:16:20 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/11 22:00:25 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	raycast(t_tsr *tsr)
{
	tsr->player.ray = setup_ray(tsr,
			tsr->camera.position, tsr->camera.chunk, tsr->camera.forward);
	trace_ray(tsr, &tsr->player.ray, false);
}

void	tsr_update_player(t_tsr *tsr)
{
	player_rotation(tsr);
	raycast(tsr);
	if (tsr->player.prompt_state)
	{
		player_update_prompt(tsr);
		return ;
	}
	player_movement(tsr);
	place_and_destroy(tsr);
	item_select(tsr);
	player_inputs(tsr);
}
