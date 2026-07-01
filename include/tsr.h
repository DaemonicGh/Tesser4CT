/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:46:46 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/28 22:02:28 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include "tsr_core.h"
#include "tsr_player.h"
#include "tsr_renderer.h"
#include "tsr_ui.h"
#include "tsr_utils.h"

void
load_regions(t_tsr *tsr);

void
load_texture_data(t_tsr *tsr);

void
init_rendering(t_tsr *tsr);

t_tsr
*tsr_init(void);

t_vec3x3
get_view_matrix(t_vec3 rotation);

void
tsr_update_camera(t_tsr *tsr);

void
tsr_init_game(t_tsr *tsr);

void
tsr_update_game(t_tsr *tsr);

void
tsr_init_pause_menu(t_tsr *tsr);

void
tsr_update_pause_menu(t_tsr *tsr);

void
tsr_init_setting_menu(t_tsr *tsr);

void
tsr_update_setting_menu(t_tsr *tsr);

void
tsr_init_main_menu(t_tsr *tsr);

void
tsr_update_main_menu(t_tsr *tsr);

void
prepare_next_render(t_tsr *tsr);

void
tsr_exit(t_tsr *tsr, t_tsr_status status, const char *message);
