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
	t_tsr *tsr, t_tsr_ray *ray, t_mbx_color *color)
{
	if (!vec3i_eq(ray->tile_position, tsr->world.tile_highlight_pos))
		return ;
	if (ray->axis == tsr->world.tile_highlight_axis)
		*color = color_blend(*color, mbx_get_pixel_unsafe(
					tsr->textures.tile_face_highlight, ray->texture_uv));
	else
		*color = color_blend(*color, mbx_get_pixel_unsafe(
					tsr->textures.tile_highlight, ray->texture_uv));
}

static void	normal_map_transform(t_tsr_ray *ray)
{
	if (ray->axis == 0 && ray->forward.x < 0)
		ray->normal = vec3(-ray->normal.z, ray->normal.y, ray->normal.x);
	else if (ray->axis == 0 && ray->forward.x >= 0)
		ray->normal = vec3(ray->normal.z, ray->normal.y, -ray->normal.x);
	else if (ray->axis == 1 && ray->forward.y < 0)
		ray->normal = vec3(ray->normal.x, -ray->normal.z, ray->normal.y);
	else if (ray->axis == 1 && ray->forward.y >= 0)
		ray->normal = vec3(ray->normal.x, ray->normal.z, -ray->normal.y);
	else if (ray->axis == 2 && ray->forward.z < 0)
		ray->normal = vec3(-ray->normal.x, ray->normal.y, -ray->normal.z);
}

static void	get_hit_normal(t_tsr_ray *ray, t_tsr_tile *tile)
{
	t_mbx_region	*nrm;
	t_vec2i			uv;
	t_mbx_color		col;

	nrm = tile->pbr[ray->axis * 2
		+ (ray->delta_sign.v[ray->axis] > 0)].nrm_tex;
	if (nrm)
	{
		uv = vec2i_mult_vd(nrm->size, ray->uv);
		col = mbx_get_pixel_unsafe(nrm, uv);
		ray->normal = vec3_sub_d(vec3_mult_d(vec3_div_d(
						vec3(255 - col.r, col.g, col.b),
						255), 2.0), 1.0);
		normal_map_transform(ray);
	}
	else
		ray->normal = get_normal(ray->forward, ray->axis);
}

t_mbx_color	get_hit_color(t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile)
{
	t_mbx_color		color;

	color = get_texture_color(ray, tile);
	if (tile->skybox)
		return (color);
	check_tile_highlight(tsr, ray, &color);
	if (ray->is_shadow)
		return (color);
	get_hit_normal(ray, tile);
	do_lighting(tsr, ray, tile, &color);
	return (color);
}
