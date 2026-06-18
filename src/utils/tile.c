/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tile.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 23:38:59 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/19 17:14:25 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

t_vec3	reflect(t_vec3 ray_dir, t_vec3 normal)
{
	return (vec3_sub(ray_dir, vec3_mult_d(
				normal, 2.0 * vec3_dot(normal, ray_dir))));
}

static uint8_t	rotation_lut(uint8_t rotation, uint8_t face)
{
	const uint8_t	rotations[6 * 4][6] = {
	{N270, S90, E0, W0, U90, D270}, {D270, U90, E90, W90, N270, S90},
	{S90, N270, E180, W180, D270, U90}, {U90, D270, E270, W270, S90, N270},
	{S270, N90, W0, E0, U270, D90}, {D90, U270, W90, E90, S270, N90},
	{N90, S270, W180, E180, D90, U270}, {U270, D90, W270, E270, N90, S270},
	{E180, W180, U0, D0, N180, S180}, {S180, N180, U90, D90, E180, W180},
	{W180, E180, U180, D180, S180, N180}, {N180, S180, U270, D270, W180, E180},
	{W0, E0, D0, U0, N0, S0}, {S0, N0, D90, U90, W0, E0},
	{E0, W0, D180, U180, S0, N0}, {N0, S0, D270, U270, E0, W0},
	{E270, W90, S0, N0, U0, D0}, {D0, U0, S90, N90, E270, W90},
	{W90, E270, S180, N180, D0, U0}, {U0, D0, S270, N270, W90, E270},
	{W270, E90, N0, S0, U180, D180}, {D180, U180, N90, S90, W270, E90},
	{E90, W270, N180, S180, D180, U180}, {U180, D180, N270, S270, E90, W270}};

	return (rotations[rotation][face]);
}

uint8_t	get_tile_texture(
	t_tsr_tile *tile, uint8_t face, t_vec2 *uv)
{
	const uint8_t	rotation = rotation_lut(tile->rotation, face);
	const uint8_t	roll = rotation & 0b11;

	if (roll == 1)
		*uv = vec2(1 - uv->y, uv->x);
	else if (roll == 2)
		*uv = vec2(1 - uv->x, 1 - uv->y);
	else if (roll == 3)
		*uv = vec2(uv->y, 1 - uv->x);
	return (rotation >> 2);
}

uint8_t	get_tile_texture_dir(
	t_tsr_tile *tile, uint8_t face, t_vec2i *dir)
{
	const uint8_t	rotation = rotation_lut(tile->rotation, face);
	const uint8_t	roll = rotation & 0b11;

	if (roll == 1)
		*dir = vec2i(-dir->y, dir->x);
	else if (roll == 2)
		*dir = vec2i(-dir->x, -dir->y);
	else if (roll == 3)
		*dir = vec2i(dir->y, -dir->x);
	return (rotation >> 2);
}
