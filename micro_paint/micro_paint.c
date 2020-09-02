/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbudau <gbudau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 16:37:24 by gbudau            #+#    #+#             */
/*   Updated: 2020/09/02 17:03:39 by gbudau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define ERR_ARG "Error: argument\n"
#define ERR_OTHER "Error: Operation file corrupted\n"

typedef struct	s_zone
{
	int		width;
	int		height;
	char	color;
	char	*buffer;
}				t_zone;

typedef struct	s_rectangle
{
	char	id;
	float	x;
	float	y;
	float	width;
	float	height;
	char	color;
}				t_rectangle;

enum			e_position
{
	OUTSIDE,
	INSIDE,
	BORDER
};

size_t		ft_strlen(const char *str)
{
	const char	*start;

	start = str;
	while (*str)
		str++;
	return (str - start);
}

int			check_zone(t_zone *zone)
{
	return (zone->width <= 0 || zone->height <= 0 || zone->width > 300 || zone->height > 300);
}

int			get_zone(t_zone *zone, FILE *file)
{
	if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->color) != 3)
		return (1);
	if (check_zone(zone) == 1)
		return (1);
	zone->buffer = malloc(zone->width * zone->height);
	if (zone->buffer == NULL)
		return (1);
	memset(zone->buffer, zone->color, zone->width * zone->height);
	return (0);
}

void		print_zone(t_zone zone)
{
	int	y;

	y = 0;
	while (y < zone.height)
	{
		write(1, &zone.buffer[y * zone.width], zone.width);
		write(1, "\n", 1);
		y++;
	}
}

int			check_rectangle(t_rectangle r)
{
	return (r.width <= 0 || r.height <= 0 || (r.id != 'r' && r.id != 'R'));
}

int			check_point(int x, int y, t_rectangle r)
{
	if (x < r.x || y < r.y || x > r.x + r.width || y > r.y + r.height)
		return (OUTSIDE);
	if (x - r.x < 1 || y - r.y < 1 || r.x + r.width - x < 1 || r.y + r.height - y < 1)
		return (BORDER);
	return (INSIDE);
}

void		draw_rectangle(t_zone *zone, t_rectangle r)
{
	int	x;
	int	y;
	int	ret;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			ret = check_point(x, y, r);
			if (ret == BORDER || (ret == INSIDE && r.id == 'R'))
				zone->buffer[y * zone->width + x] = r.color;
			x++;
		}
		y++;
	}
}

int			draw(t_zone *zone, FILE *file)
{
	t_rectangle r;
	int			ret;

	while ((ret = fscanf(file, "%c %f %f %f %f %c\n",
					&r.id, &r.x, &r.y, &r.width, &r.height, &r.color)) == 6)
	{
		if (check_rectangle(r) == 1)
			return (1);
		draw_rectangle(zone, r);
	}
	return (ret == EOF ? 0 : 1);
}

int			micro_paint(const char *path)
{
	t_zone	zone;
	int		ret;
	FILE	*file;

	file = fopen(path, "r");
	if (file == NULL)
		return (1);
	if (get_zone(&zone, file) == 1)
	{
		fclose(file);
		return (1);
	}
	ret = draw(&zone, file);
	if (ret == 0)
		print_zone(zone);
	fclose(file);
	free(zone.buffer);
	return (ret);
}

int			main(int argc, char **argv)
{
	if (argc == 2)
	{
		if (micro_paint(argv[1]) == 0)
			return (0);
		write(1, ERR_OTHER, ft_strlen(ERR_OTHER));
	}
	else
		write(1, ERR_ARG, ft_strlen(ERR_ARG));
	return (1);
}
