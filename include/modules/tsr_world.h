/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_world.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:20:44 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/15 16:09:38 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_WORLD_H
# define TSR_WORLD_H

# include <stdlib.h>
# include "../lib/MacroBoX/lib/VecLibC/include/veclc.h"
# include "types/tsr_s_world.h"

void	world_create(t_world *world, size_t width, size_t height, size_t depth);
void	world_destroy(t_world *world);
bool	inbound(t_world *world, t_vec3i pos);
t_type	block_get(t_world *world, t_vec3i pos);
void	block_set(t_world *world, t_vec3i pos, t_type type);

#endif
