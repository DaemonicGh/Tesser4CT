/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_renderer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:33:47 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/28 22:40:06 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "modules/mbx_structs.h"
#include "tsr_world.h"

typedef struct s_tsr_render_ray
{
	t_vec3			origin;
	t_vec3			dir;
	t_vec3			delta;
	t_vec3			abs_delta;
	t_vec3i			dir_sign;
	t_vec3i			iter;
	t_vec3			dist;
	double			distance;
	t_tsr_chunk		*chunk;
	t_vec3			position;
	t_vec3i			tile_chunk_position;
	int				tile_chunk_index;
	int				lifetime;
	int8_t			axis;
	t_tsr_tile		*tile;
	t_tsr_tile		*prev_tile;
	bool			draw_tile;
	bool			draw_prev_tile;
	t_vec2			uv;
	t_tsr_texture	*texture;
	t_vec2i			texture_uv;
	t_vec3			tile_normal;
	t_vec4			color;
	bool			is_shadow;
}	t_tsr_ray;

t_vec3
reflect(t_vec3 ray_dir, t_vec3 normal);

void
get_normal(t_tsr_ray *ray, t_tsr_tile *tile);

void
apply_lighting_effects(
	t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile, t_vec4 *color);

t_mbx_color
get_texture_color(t_tsr_ray *ray, t_tsr_tile *tile);

bool
set_ray_tile_color(t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile);

t_mbx_color
draw_ray(t_tsr *tsr, t_vec2 uv);

void
trace_ray(t_tsr *tsr, t_tsr_ray *ray);

t_tsr_ray
setup_ray(t_tsr *tsr, t_vec3 origin, t_tsr_chunk *chunk, t_vec3 forward);

void
get_ray_position(t_tsr_ray *ray);

void
*tsr_render_thread_init(void *data);

void
draw_title(t_tsr *tsr, char *str, t_vec2i pos, t_vec2 scale);
