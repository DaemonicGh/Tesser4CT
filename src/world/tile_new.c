/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tile_new.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 23:06:58 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/18 23:06:58 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

t_tsr_tile	tsr_tile(t_tsr_tile_id id, uint8_t face, uint8_t roll)
{
	return ((t_tsr_tile){
		.type = id,
		.rotation = face << 2 | roll,
		.light = 0
	});
}

t_tsr_tile	tsr_tile_r(t_tsr_tile_id id, uint8_t rotation)
{
	return ((t_tsr_tile){
		.type = id,
		.rotation = rotation,
		.light = 0
	});
}

t_tsr_tile	tsr_tile_l(
	t_tsr_tile_id id, uint8_t rotation, uint8_t light)
{
	return ((t_tsr_tile){
		.type = id,
		.rotation = rotation,
		.light = light
	});
}
