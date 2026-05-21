/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_ui_button.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:57:38 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/21 17:11:00 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_UI_BUTTON_H
# define TSR_UI_BUTTON_H
# include "tsr.h"

typedef struct s_ui_button
{
	t_vec2i		pos;
	t_vec2i		size;
	char		*str;
	t_mbx_color	still_col;
	t_mbx_color	hover_col;
	bool		hovered;
}	t_ui_button;

void	draw_button(t_tsr *tsr, t_ui_button *button);

void	update_button(t_tsr *tsr, t_ui_button *button, void (*f)(t_tsr *));

#endif
