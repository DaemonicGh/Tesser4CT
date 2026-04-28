/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_renderer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:33:47 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/25 13:32:29 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "tsr_world.h"

typedef struct s_tsr_render_ray
{
	t_vec3		origin;
	t_vec3		forward;
	t_vec3		delta;
	t_vec3		abs_delta;
	t_vec3		dist;
	t_vec3		position;
	t_vec3i		tile_position;
	t_vec3i		delta_sign;
	int			axis;
	int			lifetime;
	t_tsr_tile	*tile;
	t_vec2		uv;
	t_vec2i		texture_uv;
}	t_tsr_ray;
