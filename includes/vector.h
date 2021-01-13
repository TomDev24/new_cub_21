#ifndef VECTOR_H
# define VECTOR_H

typedef struct vector
{
    float x;
    float y;
}              t_vector;

t_vector add_vector(t_vector vec1, t_vector vec2);

#endif