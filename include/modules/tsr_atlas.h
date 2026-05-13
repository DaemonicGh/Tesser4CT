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

#ifndef TSR_ATLAS_H
# define TSR_ATLAS_H
# define ATLAS_LENGTH 256

# include <unistd.h>

# include "mbx.h"
# include "tsr_pbr.h"

typedef struct s_atlas
{
	char	*key[ATLAS_LENGTH];
	t_pbr	pbr[ATLAS_LENGTH];
	t_mbx	*mbx;
}	t_atlas;

void	atlas_init(t_atlas *atlas, t_mbx *mbx);
t_pbr	*atlas_get(t_atlas *atlas, const char *key);
t_pbr	*atlas_index(t_atlas *atlas, size_t i);
void	atlas_add(t_atlas *atlas, const char *path);
void	atlas_pop(t_atlas *atlas, const char *key);

#endif
