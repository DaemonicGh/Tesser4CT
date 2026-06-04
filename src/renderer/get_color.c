/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 15:57:33 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/21 20:49:38 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	check_tile_highlight(
	t_tsr *tsr, t_tsr_ray *ray, t_vec4 *col)
{
	t_mbx_region	*highlight;

	if (tsr->world.tile_highlight_chunk_id != ray->chunk_id
		|| !vec3i_eq(ray->tile_position, tsr->world.tile_highlight_pos))
		return ;
	if (ray->axis == tsr->world.tile_highlight_axis)
		highlight = tsr->textures.tile_face_highlight;
	else
		highlight = tsr->textures.tile_highlight;
	*col = vec4_blend(*col,
			vec4_from_color(mbx_get_pixel_unsafe(highlight, ray->texture_uv)));
}

bool	set_ray_tile_color(t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile)
{
	t_vec4		col;

	col = vec4_from_color(get_texture_color(tsr, ray, tile));
	if (!ray->tile_data->skybox && !ray->is_shadow)
	{
		check_tile_highlight(tsr, ray, &col);
		apply_lighting_effects(tsr, ray, tile, &col);
		/*if (ray->lifetime < 300)
			col = vec4_blend(col, vec3_w(vec3(1, 1, 1),
						fmin(1 - ray->lifetime / 300., 1)));*/
	}
	ray->color = vec4_blend(col, ray->color);
	return (ray->color.a == 1);
}
