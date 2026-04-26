/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 17:06:58 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/26 16:31:27 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

int	tsr_report_m(
	t_tsr_status status, const char *message, const char *message2)
{
	if (!message)
		return (status <= 0);
	if (status == STATUS_INFO)
		printf(ANSI_INFO TSR_NAME ": %s%s.\n" ANSI_RESET,
			message, message2);
	else if (status == STATUS_ERROR)
		printf(ANSI_ERROR TSR_NAME ": Error, %s%s.\n" ANSI_RESET,
			message, message2);
	else if (status == STATUS_WARNING)
		printf(ANSI_WARNING TSR_NAME ": Warning, %s%s.\n" ANSI_RESET,
			message, message2);
	else if (TSR_DEBUG && status == STATUS_DEBUG)
		printf(ANSI_DEBUG TSR_NAME ": %s%s.\n" ANSI_RESET,
			message, message2);
	return (status <= 0);
}

int	tsr_report(t_tsr_status status, const char *message)
{
	return (tsr_report_m(status, message, ""));
}
