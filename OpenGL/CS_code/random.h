
#ifndef RANDOM_H
#define RANDOM_H

#define VECTOR_CONE_1DEGREES	Vector( 0.00873f, 0.00873f, 0.00873f )
#define VECTOR_CONE_2DEGREES	Vector( 0.01745f, 0.01745f, 0.01745f )
#define VECTOR_CONE_3DEGREES	Vector( 0.02618f, 0.02618f, 0.02618f )
#define VECTOR_CONE_4DEGREES	Vector( 0.03490f, 0.03490f, 0.03490f )
#define VECTOR_CONE_5DEGREES	Vector( 0.04362f, 0.04362f, 0.04362f )
#define VECTOR_CONE_6DEGREES	Vector( 0.05234f, 0.05234f, 0.05234f )
#define VECTOR_CONE_7DEGREES	Vector( 0.06105f, 0.06105f, 0.06105f )
#define VECTOR_CONE_8DEGREES	Vector( 0.06976f, 0.06976f, 0.06976f )
#define VECTOR_CONE_9DEGREES	Vector( 0.07846f, 0.07846f, 0.07846f )
#define VECTOR_CONE_10DEGREES	Vector( 0.08716f, 0.08716f, 0.08716f )
#define VECTOR_CONE_15DEGREES	Vector( 0.13053f, 0.13053f, 0.13053f )
#define VECTOR_CONE_20DEGREES	Vector( 0.17365f, 0.17365f, 0.17365f )

int UTIL_SharedRandomLong(unsigned int seed, int low, int high);
float UTIL_SharedRandomFloat(unsigned int seed, float low, float high);

#endif
