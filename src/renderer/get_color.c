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

static void	get_tile_highlight(
	t_tsr *tsr, t_tsr_ray *ray, t_mbx_color *col)
{
	t_mbx_region	*highlight;
	t_vec2i			highlight_uv;

	if (tsr->player.tile_highlight_chunk != ray->chunk
		|| !vec3i_eq(ray->tile_position, tsr->player.tile_highlight_pos))
		return ;
	if (ray->tile_axis == tsr->player.tile_highlight_axis)
		highlight = tsr->textures.tile_face_highlight;
	else
		highlight = tsr->textures.tile_highlight;
	if (ray->tile_data->skybox)
		highlight_uv = vec2i_mult_vd(highlight->size, get_tile_uv(ray));
	else
		highlight_uv = vec2i_mult_vd(highlight->size, ray->uv);
	*col = color_blend(*col, mbx_get_pixel_unsafe(
				highlight, highlight_uv));
}

static void	get_chunk_highlight(
	t_tsr *tsr, t_tsr_ray *ray, t_mbx_color *col)
{
	t_vec2			chunk_uv;
	t_vec2i			texture_uv;

	if (ray->tile_data->skybox)
		chunk_uv = get_tile_uv(ray);
	else
		chunk_uv = ray->uv;
	texture_uv = vec2i_mult_vd(tsr->textures.tile_highlight->size, chunk_uv);
	*col = color_blend(*col, mbx_get_pixel_unsafe(
				tsr->textures.tile_highlight, texture_uv));
}

bool	set_ray_tile_color(t_tsr *restrict tsr, t_tsr_ray *restrict ray)
{
	t_mbx_color		col;

	col = get_texture_color(ray);
	if (!col.a)
		return (false);
	if (ray->chunk)
		get_tile_highlight(tsr, ray, &col);
	else if (tsr->extras.show_chunks)
		get_chunk_highlight(tsr, ray, &col);
	if (!ray->tile_data->skybox)
		apply_lighting_effects(tsr, ray, &col);
	ray->color = color_blend(col, ray->color);
	return (ray->color.a == 255);
}
