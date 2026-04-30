/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_atlas.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:02:47 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/30 17:12:51 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_ATLAS_H
# define TSR_ATLAS_H
# define ATLAS_LENGTH 256

# include "mbx.h"

typedef struct s_atlas
{
	char			*key[ATLAS_LENGTH];
	t_mbx_region	*tex[ATLAS_LENGTH];
	t_mbx			*mbx;
}	t_atlas;

void			atlas_init(t_atlas *atlas, t_mbx *mbx);
t_mbx_region	*atlas_get(t_atlas *atlas, const char *key);
t_mbx_region	*atlas_index(t_atlas *atlas, size_t i);
void			atlas_add(t_atlas *atlas, const char *key, const char *path);
void			atlas_pop(t_atlas *atlas, const char *key);

#endif
