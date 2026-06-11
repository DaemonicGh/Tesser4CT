/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_player.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 02:45:35 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/12 02:47:33 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "tsr_core.h"

void
player_collision(t_tsr *tsr);

void
tsr_player_actions(t_tsr *tsr);

void
create_chunk(t_tsr *tsr, t_tsr_chunk_id id, bool one_way);

void
update_chunk_prompt(t_tsr *tsr);

bool
tsr_player_update_prompt(t_tsr *tsr);

void
tsr_update_player(t_tsr *tsr);
