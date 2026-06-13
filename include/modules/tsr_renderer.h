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

typedef struct s_tsr_pbr_properties
{
	double		specular;
	double		f0;
	double		emissive;
}	t_tsr_pbr;

t_vec3
reflect(t_vec3 ray_dir, t_vec3 normal);

void
get_normal(t_tsr *tsr, t_tsr_ray *ray);

t_vec2
get_tile_uv(t_tsr_ray *ray);

void
apply_lighting_effects(
	t_tsr *tsr, t_tsr_ray *ray, t_vec4 *col);

t_mbx_color
get_texture_color(t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile);

bool
set_ray_tile_color(t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile);

t_mbx_color
draw_ray(t_tsr *tsr, t_vec2 uv);

void
trace_ray(t_tsr *tsr, t_tsr_ray *ray);

t_tsr_ray
setup_ray(t_tsr *tsr, t_vec3 origin, t_tsr_chunk_id chunk_id, t_vec3 forward);

void
*tsr_render_thread_init(void *data);

void
draw_title(t_tsr *tsr, char *str, t_vec2i pos, t_vec2 scale);
