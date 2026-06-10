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

#include "tsr_core.h"

typedef struct s_ui_button
{
	t_vec2i		pos;
	t_vec2i		size;
	char		*str;
	t_mbx_color	still_col;
	t_mbx_color	hover_col;
	bool		hovered;
}	t_ui_button;

void
draw_button(t_tsr *tsr, t_ui_button *button);

void
update_button(t_tsr *tsr, t_ui_button *button, void (*f)(t_tsr *));

void
prompt_init(t_tsr *tsr, t_vec2i	position, const char *message);

bool
prompt_update(t_tsr *tsr);

void
prompt_draw(t_tsr *tsr);

void
draw_hotbar(t_tsr *tsr);

void
draw_ui(t_tsr *tsr);
