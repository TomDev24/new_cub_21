#include <stdio.h>
#include <mlx.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "cube.h"
#include "./libft/libft.h"
#include "./gnl/get_next_line.h"

int global_map[24][24] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void fill_map(int map[M_WIDTH][M_HEIGTH])
{
    for (int i = 0; i < M_WIDTH; i++)
    {
        for (int j = 0; j < M_HEIGTH; j++)
            map[i][j] = global_map[i][j];
    }
}

void print_map(int map[M_WIDTH][M_HEIGTH])
{
    for (int i = 0; i < M_WIDTH; i++)
    {
        for (int j = 0; j < M_HEIGTH; j++)
            printf("%i", map[i][j]);
        printf("\n");
    }
}
void print_map2(char **map)
{
    while (map && *map)
    {
        printf("%s\n", *map);
        map++;
    }
}


char **gen_map(int rows, int cols)
{
    char** map = (char**)ft_calloc(cols + 1, sizeof(char*));
    for (int y = 0; y < cols; y++)
    {
            map[y] = (char*)ft_calloc(rows+1, sizeof(char));
    }
    for (int i = 0; i < cols; i++)
    {
            map[i] = "fyck";
    }
    return (map);
}

void open_print(char *name)
{
    int fd;
    char *line;

    fd = open(name, O_RDONLY);
    while(get_next_line(fd, &line) > 0)
        printf("%s\n", line);
}

int digit_len(int num)
{
    int len = 1;
    while (num / 10 != 0)
    {
        num /= 10;
        len++;
    }
    return (len);
}

int count_spaces(char *line)
{
    int c;
    
    c = 0;
    while (*line++ == ' ')
        c++;
    return (c);
}

void print_map_info(t_map *map)
{
    // resolution
    printf("X val of res %d\n", map->resolution.x);
    printf("Y val of res %d\n", map->resolution.y);

    //colors for floor and ceilling
    printf("Floor color %x \n", map->floor_c);
    printf("Ceilling color %x \n", map->ceilling_c);

    //sprite
    printf("Sprite location %s \n", map->sprite);

    //map in form of string
    printf("MAP:\n%s \n", map->map);
}

int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

void init_map_struct(t_map *map)
{
    map->map = "";
}

void fill_struct(char *line, t_map *map_info, int *flag)
{
    int value= 0;
    int r = 0;
    int g = 0;
    int b = 0;
    int offset;
    size_t line_length;

    line_length = 0;
    value++;
    if (ft_strlen(line) <= 0)
        return ;
    //its not absolutelry correct check
    if (*flag == 4)
    {
        if (ft_strlen(line) > line_length)
        {
            line_length = ft_strlen(line);
            map_info->line_length = line_length;
        }
        map_info->map = ft_strjoin(map_info->map, line);
        map_info->map = ft_strjoin(map_info->map, "\n");
        //free mem here !!!
    }
    //simple approach, might be enough
    if (line[0] == 'R')
    {
        map_info->resolution.x = ft_atoi(line+1);
        offset = digit_len(map_info->resolution.x) + count_spaces(line + 1);
        map_info->resolution.y = ft_atoi(line+offset + 1);
        *flag += 1;
    }
    if (line[0] == 'S' && line[1] != 'O') // fucking costel
    {
        map_info->sprite = ft_strtrim(line+1, " ");
        if (map_info->sprite == NULL)
        {
            offset = 0; // throw error here
        }
        *flag += 1;
    }
    if (line[0] == 'F')
    {
        r = ft_atoi(line + 1);
        g = ft_atoi(line + 3 + digit_len(r));
        printf("what is g %d", g);
        b = ft_atoi(line + 3 + digit_len(g) + digit_len(r));
        map_info->floor_c = create_trgb(0,r,g,b);
        *flag += 1;
    }
    if (line[0] == 'C')
    {
        r = ft_atoi(line + 1);
        g = ft_atoi(line + 3 + digit_len(r));
        b = ft_atoi(line + 3 + digit_len(g) + digit_len(r));
        map_info->ceilling_c = create_trgb(0,r,g,b);
        *flag += 1;
    }
}

int parse_map(char *file, t_map *map_info)
{
    int fd;
    char *line;
    int is_full;

    is_full = 0;
    init_map_struct(map_info);
    fd = open(file, O_RDONLY);
    if (fd == -1)
        return (fd);
    while(get_next_line(fd, &line) > 0)
    {
        fill_struct(line, map_info, &is_full);
        //good for debug
        //printf("\n Line: %s", line);
        //printf("  Flag %d", is_full);
    }
    print_map_info(map_info);
    return (0);
}