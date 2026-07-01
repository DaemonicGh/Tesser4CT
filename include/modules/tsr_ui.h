/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_ui.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 02:45:41 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/12 02:47:33 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "tsr_constants.h"
#include "tsr_core.h"

typedef struct s_tsr_chunk_explore
{
	t_tsr_chunk_id	id;
	uint			cycle;
	bool			error;
}	t_tsr_explore;

void
button_init(t_tsr_button *button, t_vec2i position, const char *message);

bool
button_update(t_tsr *tsr, t_tsr_button *button);

void
button_draw(t_tsr *tsr, t_tsr_button *button);

void
toggle_init(t_tsr_toggle *toggle,
	t_vec2i position, const char *message, bool value);

bool
toggle_update(t_tsr *tsr, t_tsr_toggle *toggle);

void
toggle_draw(t_tsr *tsr, t_tsr_toggle *toggle);

void
slider_init(t_tsr_slider *slider,
	t_vec2i position, const char *message, t_vec4 value_bounds_step);

double
slider_update(t_tsr *tsr, t_tsr_slider *slider);

void
slider_draw(t_tsr *tsr, t_tsr_slider *slider);

void
prompt_init(t_tsr *tsr, t_vec2i	position, const char *message);

bool
prompt_update(t_tsr *tsr);

void
prompt_draw(t_tsr *tsr);

void
set_hotbar_offset(t_tsr_hotbar *hotbar, double target, t_vec2 limits);

void
draw_hotbar(t_tsr *tsr);

void
draw_tool_hotbar(t_tsr *tsr);

void
draw_chunk(t_tsr *tsr,
	t_tsr_explore map[MINIMAP_SIZE][MINIMAP_SIZE][MINIMAP_SIZE], t_vec3i pos);

void
draw_chunk_error(t_tsr *tsr, t_vec3i pos);

void
update_and_draw_minimap(t_tsr *tsr);

void
draw_ui(t_tsr *tsr);
