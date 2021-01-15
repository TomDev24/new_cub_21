#ifndef VECTOR_H
# define VECTOR_H

typedef struct vector
{
    int x;
    int y;
}              t_vector;

t_vector add_vector(t_vector vec1, t_vector vec2);

#endif