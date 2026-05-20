/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_atlas.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:02:47 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/13 18:34:04 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define ATLAS_LENGTH 256

#include "tsr_core.h"

typedef struct s_tsr_pbr_texture_atlas
{
	t_mbx_region	*tx;
	t_mbx_region	*nrm;
}	t_tsr_texture;

typedef struct s_atlas
{
	char			*key[ATLAS_LENGTH];
	t_tsr_texture	texture[ATLAS_LENGTH];
}	t_atlas;

void			atlas_init(t_atlas *atlas);
t_tsr_texture	*atlas_get(t_atlas *atlas, const char *key);
void			atlas_add(t_tsr *tsr, t_atlas *atlas, const char *path);
