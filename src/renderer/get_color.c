/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:57:33 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/18 16:45:22 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	check_tile_highlight(
	t_tsr *tsr, t_tsr_ray *ray, t_vec4 *col)
{
	t_mbx_region	*highlight;

	if (!vec3i_eq(ray->tile_position, tsr->world.tile_highlight_pos))
		return ;
	if (ray->axis == tsr->world.tile_highlight_axis)
		highlight = tsr->textures.tile_face_highlight;
	else
		highlight = tsr->textures.tile_highlight;
	*col = vec4_blend(*col,
			vec4_from_color(mbx_get_pixel_unsafe(highlight, ray->texture_uv)));
}

t_vec4	ray_tile_color(t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile)
{
	t_vec4		col;

	col = vec4_from_color(get_texture_color(ray, tile));
	if (tile->skybox || ray->is_shadow)
		return (col);
	check_tile_highlight(tsr, ray, &col);
	apply_lighting_effects(tsr, ray, tile, &col);
	return (col);
}
