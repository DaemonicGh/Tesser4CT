/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:46:46 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/26 22:09:11 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdlib.h>
#include <math.h>

#include "tsr_core.h"
#include "tsr_context.h"
#include "tsr_renderer.h"
#include "tsr_utils.h"

void
init_textures(t_tsr *tsr);

t_tsr
*tsr_init(void);

void
*tsr_render_thread_init(void *data);

void
tsr_update_camera(t_tsr *tsr);

void
tsr_player_movement(t_tsr *tsr);

void
tsr_player_actions(t_tsr *tsr);

void
tsr_update_player(t_tsr *tsr);

t_mbx_color
draw_ray(t_tsr *tsr, t_vec2 uv);

void
trace_ray(t_tsr *tsr, t_tsr_ray *ray);

void
get_ray_position(t_tsr_ray *ray);

void
tsr_exit(t_tsr *tsr, t_tsr_status status, const char *message);
