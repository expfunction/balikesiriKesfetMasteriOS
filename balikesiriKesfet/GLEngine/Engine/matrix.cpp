#include <string.h>
#include <math.h>
#include "types.h"
#include "matrix.h"
#include "vector.h"
#include "../Logger.h"

mat4 mat4_create(
        float m00,float m01,float m02,float m03,
        float m10,float m11,float m12,float m13,
        float m20,float m21,float m22,float m23,
        float m30,float m31,float m32,float m33
){
    mat4 m;
    m.m[0].x = m00;
    m.m[1].x = m01;
    m.m[2].x = m02;
    m.m[3].x = m03;

    m.m[0].y = m10;
    m.m[1].y = m11;
    m.m[2].y = m12;
    m.m[3].y = m13;

    m.m[0].z = m20;
    m.m[1].z = m21;
    m.m[2].z = m22;
    m.m[3].z = m23;

    m.m[0].w = m30;
    m.m[1].w = m31;
    m.m[2].w = m32;
    m.m[3].w = m33;
    return m;
}

void vec3_multiply_mat3( vec3 *dst, vec3 *v, mat3 *m )
{
    vec3 temp;
	temp.x = ( v->x * m->m[ 0 ].x ) +
			 ( v->y * m->m[ 1 ].x ) +
			 ( v->z * m->m[ 2 ].x );

	temp.y = ( v->x * m->m[ 0 ].y ) +
			 ( v->y * m->m[ 1 ].y ) +
			 ( v->z * m->m[ 2 ].y );

	temp.z = ( v->x * m->m[ 0 ].z ) +
			 ( v->y * m->m[ 1 ].z ) +
			 ( v->z * m->m[ 2 ].z );

    memcpy( dst,&temp,sizeof( vec3 ) );
}


void vec3_multiply_mat4( vec3 *dst, vec3 *v, mat4 *m )
{
	dst->x = ( v->x * m->m[ 0 ].x ) +
			 ( v->y * m->m[ 1 ].x ) +
			 ( v->z * m->m[ 2 ].x );

	dst->y = ( v->x * m->m[ 0 ].y ) +
			 ( v->y * m->m[ 1 ].y ) +
			 ( v->z * m->m[ 2 ].y );

	dst->z = ( v->x * m->m[ 0 ].z ) +
			 ( v->y * m->m[ 1 ].z ) +
			 ( v->z * m->m[ 2 ].z );
}

void vec3_multiply_mat4v2( vec3 *dst, vec3 *v, mat4 *m )
{
	dst->x = ( v->x * m->m[ 0 ].x ) +
			 ( v->y * m->m[ 1 ].x ) +
			 ( v->z * m->m[ 2 ].x ) +
			 ( 1 * m->m[ 3 ].x );

	dst->y = ( v->x * m->m[ 0 ].y ) +
			 ( v->y * m->m[ 1 ].y ) +
			( v->z * m->m[ 2 ].y ) +
			 ( 1 * m->m[ 3 ].y );

	dst->z = ( v->x * m->m[ 0 ].z ) +
			 ( v->y * m->m[ 1 ].z ) +
			 ( v->z * m->m[ 2 ].z ) +
			 ( 1 * m->m[ 3 ].z );
}


void vec4_multiply_mat4( vec4 *dst, vec4 *v, mat4 *m )
{
	dst->x = ( v->x * m->m[ 0 ].x ) +
			 ( v->y * m->m[ 1 ].x ) +
			 ( v->z * m->m[ 2 ].x ) + 
			 ( v->w * m->m[ 3 ].x );

	dst->y = ( v->x * m->m[ 0 ].y ) +
			 ( v->y * m->m[ 1 ].y ) +
			 ( v->z * m->m[ 2 ].y ) + 
			 ( v->w * m->m[ 3 ].y );

	dst->z = ( v->x * m->m[ 0 ].z ) +
			 ( v->y * m->m[ 1 ].z ) +
			 ( v->z * m->m[ 2 ].z ) + 
			 ( v->w * m->m[ 3 ].z );

	dst->w = ( v->x * m->m[ 0 ].w ) +
			 ( v->y * m->m[ 1 ].w ) +
			 ( v->z * m->m[ 2 ].w ) + 
			 ( v->w * m->m[ 3 ].w );
}



void mat3_identity( mat3 *m )
{
	memset( &m[ 0 ], 0, sizeof( mat3 ) );
	
	m->m[ 0 ].x =
	m->m[ 1 ].y =
	m->m[ 2 ].z = 1.0f;

}


void mat3_copy_mat4( mat3 *dst, mat4 *m )
{
	memcpy( &dst->m[ 0 ], &m->m[ 0 ], sizeof( vec3 ) );
	memcpy( &dst->m[ 1 ], &m->m[ 1 ], sizeof( vec3 ) );
	memcpy( &dst->m[ 2 ], &m->m[ 2 ], sizeof( vec3 ) );
}


void mat4_identity( mat4 *m )
{
	memset( &m[ 0 ], 0, sizeof( mat4 ) );
	
	m->m[ 0 ].x =
	m->m[ 1 ].y =
	m->m[ 2 ].z =
	m->m[ 3 ].w = 1.0f;
}


void mat4_copy_mat4( mat4 *dst, mat4 *m )
{ memcpy( dst, m, sizeof( mat4 ) ); }


void mat4_translate( mat4 *dst, mat4 *m, vec3 *v )
{
	dst->m[ 3 ].x = m->m[ 0 ].x * v->x + m->m[ 1 ].x * v->y + m->m[ 2 ].x * v->z + m->m[ 3 ].x;
	dst->m[ 3 ].y = m->m[ 0 ].y * v->x + m->m[ 1 ].y * v->y + m->m[ 2 ].y * v->z + m->m[ 3 ].y;
	dst->m[ 3 ].z = m->m[ 0 ].z * v->x + m->m[ 1 ].z * v->y + m->m[ 2 ].z * v->z + m->m[ 3 ].z;
	dst->m[ 3 ].w = m->m[ 0 ].w * v->x + m->m[ 1 ].w * v->y + m->m[ 2 ].w * v->z + m->m[ 3 ].w;
}


void mat4_rotate_fast( mat4 *m, vec4 *v )
{
	float s = sinf( v->w * DEG_TO_RAD ),
		  c = cosf( v->w * DEG_TO_RAD );		  
	
	mat4 mat;
	
	mat4_identity( &mat );

	if( !v->x )
	{
		if( !v->y )
		{
			if( v->z )
			{
				mat.m[ 0 ].x = c;
				mat.m[ 1 ].y = c;
				
				if( v->z < 0.0f )
				{
					mat.m[ 1 ].x =  s;
					mat.m[ 0 ].y = -s;
				}
				else
				{
					mat.m[ 1 ].x = -s;
					mat.m[ 0 ].y =  s;
				}
			}
		}
		else if( !v->z )
		{
			mat.m[ 0 ].x = c;
			mat.m[ 2 ].z = c;

			if( v->y < 0.0f )
			{
				mat.m[ 2 ].x = -s;
				mat.m[ 0 ].z =  s;
			}
			else
			{
				mat.m[ 2 ].x =  s;
				mat.m[ 0 ].z = -s;
			}
		}
	}
	else if( !v->y )
	{
		if( !v->z )
		{
			mat.m[ 1 ].y = c;
			mat.m[ 2 ].z = c;
			
			if( v->x < 0.0f )
			{
				mat.m[ 2 ].y =  s;
				mat.m[ 1 ].z = -s;
			}
			else
			{
				mat.m[ 2 ].y = -s;
				mat.m[ 1 ].z =  s;
			}
		}
	}

	mat4_multiply_mat4( m, m, &mat );
}


void mat4_rotate( mat4 *dst, mat4 *m, vec4 *v )
{
	float s = sinf( v->w * DEG_TO_RAD ),
		  c = cosf( v->w * DEG_TO_RAD ),
		  xx,
		  yy,
		  zz,
		  xy,
		  yz,
		  zx,
		  xs,
		  ys,
		  zs,
		  c1;
			
	mat4 mat;

	vec3 t = { v->x, v->y, v->z };

	mat4_identity( &mat );
	
	if( !v->w || !vec3_normalize( &t, &t ) ) return;

	xx = t.x * t.x;
	yy = t.y * t.y;
	zz = t.z * t.z;
	xy = t.x * t.y;
	yz = t.y * t.z;
	zx = t.z * t.x;
	xs = t.x * s;
	ys = t.y * s;
	zs = t.z * s;
	c1 = 1.0f - c;

	mat.m[ 0 ].x = ( c1 * xx ) + c;
	mat.m[ 1 ].x = ( c1 * xy ) - zs;
	mat.m[ 2 ].x = ( c1 * zx ) + ys;

	mat.m[ 0 ].y = ( c1 * xy ) + zs;
	mat.m[ 1 ].y = ( c1 * yy ) + c;
	mat.m[ 2 ].y = ( c1 * yz ) - xs;

	mat.m[ 0 ].z = ( c1 * zx ) - ys;
	mat.m[ 1 ].z = ( c1 * yz ) + xs;
	mat.m[ 2 ].z = ( c1 * zz ) + c;
	
	mat4_multiply_mat4( m, m, &mat );	
}


void mat4_scale( mat4 *dst, mat4 *m, vec3 *v )
{
	dst->m[ 0 ].x = m->m[ 0 ].x * v->x;
	dst->m[ 0 ].y = m->m[ 0 ].y * v->x;
	dst->m[ 0 ].z = m->m[ 0 ].z * v->x;
	dst->m[ 0 ].w = m->m[ 0 ].w * v->x;

	dst->m[ 1 ].x = m->m[ 1 ].x * v->y;
	dst->m[ 1 ].y = m->m[ 1 ].y * v->y;
	dst->m[ 1 ].z = m->m[ 1 ].z * v->y;
	dst->m[ 1 ].w = m->m[ 1 ].w * v->y;

	dst->m[ 2 ].x = m->m[ 2 ].x * v->z;
	dst->m[ 2 ].y = m->m[ 2 ].y * v->z;
	dst->m[ 2 ].z = m->m[ 2 ].z * v->z;
	dst->m[ 2 ].w = m->m[ 2 ].w * v->z;
}


unsigned char mat4_invert( mat4 *m )
{
	mat4 mat;
	
	float d = ( m->m[ 0 ].x * m->m[ 0 ].x + 
			    m->m[ 1 ].x * m->m[ 1 ].x + 
			    m->m[ 2 ].x * m->m[ 2 ].x );

	if( !d ) return 0;
	
	d = 1.0f / d;

	mat.m[ 0 ].x = d * m->m[ 0 ].x;
	mat.m[ 0 ].y = d * m->m[ 1 ].x;
	mat.m[ 0 ].z = d * m->m[ 2 ].x;

	mat.m[ 1 ].x = d * m->m[ 0 ].y;
	mat.m[ 1 ].y = d * m->m[ 1 ].y;
	mat.m[ 1 ].z = d * m->m[ 2 ].y;

	mat.m[ 2 ].x = d * m->m[ 0 ].z;	
	mat.m[ 2 ].y = d * m->m[ 1 ].z;
	mat.m[ 2 ].z = d * m->m[ 2 ].z;

	mat.m[ 3 ].x = -( mat.m[ 0 ].x * m->m[ 3 ].x + mat.m[ 1 ].x * m->m[ 3 ].y + mat.m[ 2 ].x * m->m[ 3 ].z );
	mat.m[ 3 ].y = -( mat.m[ 0 ].y * m->m[ 3 ].x + mat.m[ 1 ].y * m->m[ 3 ].y + mat.m[ 2 ].y * m->m[ 3 ].z );
	mat.m[ 3 ].z = -( mat.m[ 0 ].z * m->m[ 3 ].x + mat.m[ 1 ].z * m->m[ 3 ].y + mat.m[ 2 ].z * m->m[ 3 ].z );

	mat.m[ 0 ].w = 
	mat.m[ 1 ].w = 
	mat.m[ 2 ].w = 0.0f;
	mat.m[ 3 ].w = 1.0f;

	mat4_copy_mat4( m, &mat );
	
	return 1;
}


unsigned char mat4_invert_full( mat4 *m )
{
	mat4 inv;

	float d;

	inv.m[ 0 ].x = m->m[ 1 ].y * m->m[ 2 ].z * m->m[ 3 ].w - 
				   m->m[ 1 ].y * m->m[ 2 ].w * m->m[ 3 ].z - 
				   m->m[ 2 ].y * m->m[ 1 ].z * m->m[ 3 ].w +
				   m->m[ 2 ].y * m->m[ 1 ].w * m->m[ 3 ].z + 
				   m->m[ 3 ].y * m->m[ 1 ].z * m->m[ 2 ].w - 
				   m->m[ 3 ].y * m->m[ 1 ].w * m->m[ 2 ].z;
		 
	inv.m[ 1 ].x = -m->m[ 1 ].x * m->m[ 2 ].z * m->m[ 3 ].w +
					m->m[ 1 ].x * m->m[ 2 ].w * m->m[ 3 ].z +
					m->m[ 2 ].x * m->m[ 1 ].z * m->m[ 3 ].w -
					m->m[ 2 ].x * m->m[ 1 ].w * m->m[ 3 ].z -
					m->m[ 3 ].x * m->m[ 1 ].z * m->m[ 2 ].w +
					m->m[ 3 ].x * m->m[ 1 ].w * m->m[ 2 ].z;
		 
	inv.m[ 2 ].x = m->m[ 1 ].x * m->m[ 2 ].y * m->m[ 3 ].w -
				   m->m[ 1 ].x * m->m[ 2 ].w * m->m[ 3 ].y -
				   m->m[ 2 ].x * m->m[ 1 ].y * m->m[ 3 ].w +
				   m->m[ 2 ].x * m->m[ 1 ].w * m->m[ 3 ].y +
				   m->m[ 3 ].x * m->m[ 1 ].y * m->m[ 2 ].w -
				   m->m[ 3 ].x * m->m[ 1 ].w * m->m[ 2 ].y;
		 
	inv.m[ 3 ].x = -m->m[ 1 ].x * m->m[ 2 ].y * m->m[ 3 ].z +
					m->m[ 1 ].x * m->m[ 2 ].z * m->m[ 3 ].y +
					m->m[ 2 ].x * m->m[ 1 ].y * m->m[ 3 ].z -
					m->m[ 2 ].x * m->m[ 1 ].z * m->m[ 3 ].y -
					m->m[ 3 ].x * m->m[ 1 ].y * m->m[ 2 ].z +
					m->m[ 3 ].x * m->m[ 1 ].z * m->m[ 2 ].y;
		 
	inv.m[ 0 ].y = -m->m[ 0 ].y * m->m[ 2 ].z * m->m[ 3 ].w +
					m->m[ 0 ].y * m->m[ 2 ].w * m->m[ 3 ].z +
					m->m[ 2 ].y * m->m[ 0 ].z * m->m[ 3 ].w -
					m->m[ 2 ].y * m->m[ 0 ].w * m->m[ 3 ].z -
					m->m[ 3 ].y * m->m[ 0 ].z * m->m[ 2 ].w +
					m->m[ 3 ].y * m->m[ 0 ].w * m->m[ 2 ].z;
		 
	inv.m[ 1 ].y = m->m[ 0 ].x * m->m[ 2 ].z * m->m[ 3 ].w -
				   m->m[ 0 ].x * m->m[ 2 ].w * m->m[ 3 ].z -
				   m->m[ 2 ].x * m->m[ 0 ].z * m->m[ 3 ].w +
				   m->m[ 2 ].x * m->m[ 0 ].w * m->m[ 3 ].z +
				   m->m[ 3 ].x * m->m[ 0 ].z * m->m[ 2 ].w -
				   m->m[ 3 ].x * m->m[ 0 ].w * m->m[ 2 ].z;
		 
	inv.m[ 2 ].y = -m->m[ 0 ].x * m->m[ 2 ].y * m->m[ 3 ].w +
					m->m[ 0 ].x * m->m[ 2 ].w * m->m[ 3 ].y +
					m->m[ 2 ].x * m->m[ 0 ].y * m->m[ 3 ].w -
					m->m[ 2 ].x * m->m[ 0 ].w * m->m[ 3 ].y -
					m->m[ 3 ].x * m->m[ 0 ].y * m->m[ 2 ].w +
					m->m[ 3 ].x * m->m[ 0 ].w * m->m[ 2 ].y;
		 
	inv.m[ 3 ].y = m->m[ 0 ].x * m->m[ 2 ].y * m->m[ 3 ].z - 
				   m->m[ 0 ].x * m->m[ 2 ].z * m->m[ 3 ].y -
				   m->m[ 2 ].x * m->m[ 0 ].y * m->m[ 3 ].z +
				   m->m[ 2 ].x * m->m[ 0 ].z * m->m[ 3 ].y +
				   m->m[ 3 ].x * m->m[ 0 ].y * m->m[ 2 ].z -
				   m->m[ 3 ].x * m->m[ 0 ].z * m->m[ 2 ].y;
		 
	inv.m[ 0 ].z = m->m[ 0 ].y * m->m[ 1 ].z * m->m[ 3 ].w -
				   m->m[ 0 ].y * m->m[ 1 ].w * m->m[ 3 ].z -
				   m->m[ 1 ].y * m->m[ 0 ].z * m->m[ 3 ].w +
				   m->m[ 1 ].y * m->m[ 0 ].w * m->m[ 3 ].z +
				   m->m[ 3 ].y * m->m[ 0 ].z * m->m[ 1 ].w -
				   m->m[ 3 ].y * m->m[ 0 ].w * m->m[ 1 ].z;
		 
	inv.m[ 1 ].z = -m->m[ 0 ].x * m->m[ 1 ].z * m->m[ 3 ].w +
					m->m[ 0 ].x * m->m[ 1 ].w * m->m[ 3 ].z +
					m->m[ 1 ].x * m->m[ 0 ].z * m->m[ 3 ].w -
					m->m[ 1 ].x * m->m[ 0 ].w * m->m[ 3 ].z -
					m->m[ 3 ].x * m->m[ 0 ].z * m->m[ 1 ].w +
					m->m[ 3 ].x * m->m[ 0 ].w * m->m[ 1 ].z;
		 
	inv.m[ 2 ].z = m->m[ 0 ].x * m->m[ 1 ].y * m->m[ 3 ].w -
				   m->m[ 0 ].x * m->m[ 1 ].w * m->m[ 3 ].y -
				   m->m[ 1 ].x * m->m[ 0 ].y * m->m[ 3 ].w +
				   m->m[ 1 ].x * m->m[ 0 ].w * m->m[ 3 ].y +
				   m->m[ 3 ].x * m->m[ 0 ].y * m->m[ 1 ].w -
				   m->m[ 3 ].x * m->m[ 0 ].w * m->m[ 1 ].y;
		 
	inv.m[ 3 ].z = -m->m[ 0 ].x * m->m[ 1 ].y * m->m[ 3 ].z +
					m->m[ 0 ].x * m->m[ 1 ].z * m->m[ 3 ].y +
					m->m[ 1 ].x * m->m[ 0 ].y * m->m[ 3 ].z -
					m->m[ 1 ].x * m->m[ 0 ].z * m->m[ 3 ].y -
					m->m[ 3 ].x * m->m[ 0 ].y * m->m[ 1 ].z +
					m->m[ 3 ].x * m->m[ 0 ].z * m->m[ 1 ].y;
		 
	inv.m[ 0 ].w = -m->m[ 0 ].y * m->m[ 1 ].z * m->m[ 2 ].w +
					m->m[ 0 ].y * m->m[ 1 ].w * m->m[ 2 ].z +
					m->m[ 1 ].y * m->m[ 0 ].z * m->m[ 2 ].w -
					m->m[ 1 ].y * m->m[ 0 ].w * m->m[ 2 ].z -
					m->m[ 2 ].y * m->m[ 0 ].z * m->m[ 1 ].w +
					m->m[ 2 ].y * m->m[ 0 ].w * m->m[ 1 ].z;
		 
	inv.m[ 1 ].w = m->m[ 0 ].x * m->m[ 1 ].z * m->m[ 2 ].w -
				   m->m[ 0 ].x * m->m[ 1 ].w * m->m[ 2 ].z -
				   m->m[ 1 ].x * m->m[ 0 ].z * m->m[ 2 ].w +
				   m->m[ 1 ].x * m->m[ 0 ].w * m->m[ 2 ].z +
				   m->m[ 2 ].x * m->m[ 0 ].z * m->m[ 1 ].w -
				   m->m[ 2 ].x * m->m[ 0 ].w * m->m[ 1 ].z;
		 
	inv.m[ 2 ].w = -m->m[ 0 ].x * m->m[ 1 ].y * m->m[ 2 ].w +
					m->m[ 0 ].x * m->m[ 1 ].w * m->m[ 2 ].y +
					m->m[ 1 ].x * m->m[ 0 ].y * m->m[ 2 ].w -
					m->m[ 1 ].x * m->m[ 0 ].w * m->m[ 2 ].y -
					m->m[ 2 ].x * m->m[ 0 ].y * m->m[ 1 ].w +
					m->m[ 2 ].x * m->m[ 0 ].w * m->m[ 1 ].y;
		 
	inv.m[ 3 ].w = m->m[ 0 ].x * m->m[ 1 ].y * m->m[ 2 ].z -
				   m->m[ 0 ].x * m->m[ 1 ].z * m->m[ 2 ].y -
				   m->m[ 1 ].x * m->m[ 0 ].y * m->m[ 2 ].z +
				   m->m[ 1 ].x * m->m[ 0 ].z * m->m[ 2 ].y +
				   m->m[ 2 ].x * m->m[ 0 ].y * m->m[ 1 ].z -
				   m->m[ 2 ].x * m->m[ 0 ].z * m->m[ 1 ].y;

	d = m->m[ 0 ].x * inv.m[ 0 ].x + 
		m->m[ 0 ].y * inv.m[ 1 ].x +
		m->m[ 0 ].z * inv.m[ 2 ].x +
		m->m[ 0 ].w * inv.m[ 3 ].x;
	
	if( !d ) return 0;

	d = 1.0f / d;

	inv.m[ 0 ].x *= d;
	inv.m[ 0 ].y *= d;
	inv.m[ 0 ].z *= d;
	inv.m[ 0 ].w *= d;

	inv.m[ 1 ].x *= d;
	inv.m[ 1 ].y *= d;
	inv.m[ 1 ].z *= d;
	inv.m[ 1 ].w *= d;

	inv.m[ 2 ].x *= d;
	inv.m[ 2 ].y *= d;
	inv.m[ 2 ].z *= d;
	inv.m[ 2 ].w *= d;

	inv.m[ 3 ].x *= d;
	inv.m[ 3 ].y *= d;
	inv.m[ 3 ].z *= d;
	inv.m[ 3 ].w *= d;
	
	mat4_copy_mat4( m, &inv ); 
	
	return 1;
}


void mat4_transpose( mat4 *m )
{
	float t;

	t			= m->m[ 0 ].y;
	m->m[ 0 ].y = m->m[ 1 ].x; 
	m->m[ 1 ].x = t;
	
	t			= m->m[ 0 ].z; 
	m->m[ 0 ].z = m->m[ 2 ].x; 
	m->m[ 2 ].x = t;
	
	t			= m->m[ 0 ].w; 
	m->m[ 0 ].w = m->m[ 3 ].x; 
	m->m[ 3 ].x = t;

	t			= m->m[ 1 ].z; 
	m->m[ 1 ].z = m->m[ 2 ].y; 
	m->m[ 2 ].y = t;
	
	t			= m->m[ 1 ].w ; 
	m->m[ 1 ].w = m->m[ 3 ].y ; 
	m->m[ 3 ].y = t;

	t			= m->m[ 2 ].w ; 
	m->m[ 2 ].w = m->m[ 3 ].z ; 
	m->m[ 3 ].z = t;
}


void mat4_ortho( mat4 *dst, float left, float right, float bottom, float top, float clip_start, float clip_end )
{
	mat4 mat;

	mat.m[ 0 ].x = 2.0f / ( right - left  );
	mat.m[ 1 ].x = 0.0f;
	mat.m[ 2 ].x = 0.0f;
	mat.m[ 3 ].x = -( right + left ) / ( right - left );

	mat.m[ 0 ].y = 0.0f;
	mat.m[ 1 ].y = 2.0f / ( top - bottom );
	mat.m[ 2 ].y = 0.0f;
	mat.m[ 3 ].y = -( top + bottom ) / ( top - bottom );

	mat.m[ 0 ].z = 0.0f;
	mat.m[ 1 ].z = 0.0f;
	mat.m[ 2 ].z = -2.0f / ( clip_end - clip_start );
	mat.m[ 3 ].z = -( clip_end + clip_start ) / ( clip_end - clip_start );

	mat.m[ 0 ].w =
	mat.m[ 1 ].w = 
	mat.m[ 2 ].w = 0.0f;
	mat.m[ 3 ].w = 1.0f;		

	mat4_multiply_mat4( dst, dst, &mat );
}


void mat4_copy_mat3( mat4 *dst, mat3 *m )
{
	memcpy( &dst->m[ 0 ], &m->m[ 0 ], sizeof( vec3 ) );
	memcpy( &dst->m[ 1 ], &m->m[ 1 ], sizeof( vec3 ) );
	memcpy( &dst->m[ 2 ], &m->m[ 2 ], sizeof( vec3 ) );
}


void mat4_multiply_mat3( mat4 *dst, mat4 *m0, mat3 *m1 )
{
	mat3 mat;

	mat.m[ 0 ].x = m0->m[ 0 ].x * m1->m[ 0 ].x + m0->m[ 1 ].x * m1->m[ 0 ].y + m0->m[ 2 ].x * m1->m[ 0 ].z;
	mat.m[ 0 ].y = m0->m[ 0 ].y * m1->m[ 0 ].x + m0->m[ 1 ].y * m1->m[ 0 ].y + m0->m[ 2 ].y * m1->m[ 0 ].z;
	mat.m[ 0 ].z = m0->m[ 0 ].z * m1->m[ 0 ].x + m0->m[ 1 ].z * m1->m[ 0 ].y + m0->m[ 2 ].z * m1->m[ 0 ].z;

	mat.m[ 1 ].x = m0->m[ 0 ].x * m1->m[ 1 ].x + m0->m[ 1 ].x * m1->m[ 1 ].y + m0->m[ 2 ].x * m1->m[ 1 ].z;
	mat.m[ 1 ].y = m0->m[ 0 ].y * m1->m[ 1 ].x + m0->m[ 1 ].y * m1->m[ 1 ].y + m0->m[ 2 ].y * m1->m[ 1 ].z;
	mat.m[ 1 ].z = m0->m[ 0 ].z * m1->m[ 1 ].x + m0->m[ 1 ].z * m1->m[ 1 ].y + m0->m[ 2 ].z * m1->m[ 1 ].z;

	mat.m[ 2 ].x = m0->m[ 0 ].x * m1->m[ 2 ].x + m0->m[ 1 ].x * m1->m[ 2 ].y + m0->m[ 2 ].x * m1->m[ 2 ].z;
	mat.m[ 2 ].y = m0->m[ 0 ].y * m1->m[ 2 ].x + m0->m[ 1 ].y * m1->m[ 2 ].y + m0->m[ 2 ].y * m1->m[ 2 ].z;
	mat.m[ 2 ].z = m0->m[ 0 ].z * m1->m[ 2 ].x + m0->m[ 1 ].z * m1->m[ 2 ].y + m0->m[ 2 ].z * m1->m[ 2 ].z;

	mat4_copy_mat3( dst, &mat );
}


void mat4_multiply_mat4( mat4 *dst, mat4 *m0, mat4 *m1 )
{
	mat4 mat;

	mat.m[ 0 ].x = m0->m[ 0 ].x * m1->m[ 0 ].x + m0->m[ 1 ].x * m1->m[ 0 ].y + m0->m[ 2 ].x * m1->m[ 0 ].z + m0->m[ 3 ].x * m1->m[ 0 ].w;
	mat.m[ 0 ].y = m0->m[ 0 ].y * m1->m[ 0 ].x + m0->m[ 1 ].y * m1->m[ 0 ].y + m0->m[ 2 ].y * m1->m[ 0 ].z + m0->m[ 3 ].y * m1->m[ 0 ].w;
	mat.m[ 0 ].z = m0->m[ 0 ].z * m1->m[ 0 ].x + m0->m[ 1 ].z * m1->m[ 0 ].y + m0->m[ 2 ].z * m1->m[ 0 ].z + m0->m[ 3 ].z * m1->m[ 0 ].w;
	mat.m[ 0 ].w = m0->m[ 0 ].w * m1->m[ 0 ].x + m0->m[ 1 ].w * m1->m[ 0 ].y + m0->m[ 2 ].w * m1->m[ 0 ].z + m0->m[ 3 ].w * m1->m[ 0 ].w;

	mat.m[ 1 ].x = m0->m[ 0 ].x * m1->m[ 1 ].x + m0->m[ 1 ].x * m1->m[ 1 ].y + m0->m[ 2 ].x * m1->m[ 1 ].z + m0->m[ 3 ].x * m1->m[ 1 ].w;
	mat.m[ 1 ].y = m0->m[ 0 ].y * m1->m[ 1 ].x + m0->m[ 1 ].y * m1->m[ 1 ].y + m0->m[ 2 ].y * m1->m[ 1 ].z + m0->m[ 3 ].y * m1->m[ 1 ].w;
	mat.m[ 1 ].z = m0->m[ 0 ].z * m1->m[ 1 ].x + m0->m[ 1 ].z * m1->m[ 1 ].y + m0->m[ 2 ].z * m1->m[ 1 ].z + m0->m[ 3 ].z * m1->m[ 1 ].w;
	mat.m[ 1 ].w = m0->m[ 0 ].w * m1->m[ 1 ].x + m0->m[ 1 ].w * m1->m[ 1 ].y + m0->m[ 2 ].w * m1->m[ 1 ].z + m0->m[ 3 ].w * m1->m[ 1 ].w;

	mat.m[ 2 ].x = m0->m[ 0 ].x * m1->m[ 2 ].x + m0->m[ 1 ].x * m1->m[ 2 ].y + m0->m[ 2 ].x * m1->m[ 2 ].z + m0->m[ 3 ].x * m1->m[ 2 ].w;
	mat.m[ 2 ].y = m0->m[ 0 ].y * m1->m[ 2 ].x + m0->m[ 1 ].y * m1->m[ 2 ].y + m0->m[ 2 ].y * m1->m[ 2 ].z + m0->m[ 3 ].y * m1->m[ 2 ].w;
	mat.m[ 2 ].z = m0->m[ 0 ].z * m1->m[ 2 ].x + m0->m[ 1 ].z * m1->m[ 2 ].y + m0->m[ 2 ].z * m1->m[ 2 ].z + m0->m[ 3 ].z * m1->m[ 2 ].w;
	mat.m[ 2 ].w = m0->m[ 0 ].w * m1->m[ 2 ].x + m0->m[ 1 ].w * m1->m[ 2 ].y + m0->m[ 2 ].w * m1->m[ 2 ].z + m0->m[ 3 ].w * m1->m[ 2 ].w;

	mat.m[ 3 ].x = m0->m[ 0 ].x * m1->m[ 3 ].x + m0->m[ 1 ].x * m1->m[ 3 ].y + m0->m[ 2 ].x * m1->m[ 3 ].z + m0->m[ 3 ].x * m1->m[ 3 ].w;
	mat.m[ 3 ].y = m0->m[ 0 ].y * m1->m[ 3 ].x + m0->m[ 1 ].y * m1->m[ 3 ].y + m0->m[ 2 ].y * m1->m[ 3 ].z + m0->m[ 3 ].y * m1->m[ 3 ].w;
	mat.m[ 3 ].z = m0->m[ 0 ].z * m1->m[ 3 ].x + m0->m[ 1 ].z * m1->m[ 3 ].y + m0->m[ 2 ].z * m1->m[ 3 ].z + m0->m[ 3 ].z * m1->m[ 3 ].w;
	mat.m[ 3 ].w = m0->m[ 0 ].w * m1->m[ 3 ].x + m0->m[ 1 ].w * m1->m[ 3 ].y + m0->m[ 2 ].w * m1->m[ 3 ].z + m0->m[ 3 ].w * m1->m[ 3 ].w;

	mat4_copy_mat4( dst, &mat );
}

void mat4_Log(char *tag,mat4 *m) {
    /*LOGI("MAT:\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
         m->m[0].x,m->m[0].y,m->m[0].z,m->m[0].w,
         m->m[1].x,m->m[1].y,m->m[1].z,m->m[1].w,
         m->m[2].x,m->m[2].y,m->m[2].z,m->m[2].w,
         m->m[3].x,m->m[3].y,m->m[3].z,m->m[3].w
    );*/
    LOGI("%s:\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",tag,
         m->m[0].x,m->m[1].x,m->m[2].x,m->m[3].x,
         m->m[0].y,m->m[1].y,m->m[2].y,m->m[3].y,
         m->m[0].z,m->m[1].z,m->m[2].z,m->m[3].z,
         m->m[0].w,m->m[1].w,m->m[2].w,m->m[3].w
    );
}

void mat3_log(char *tag,mat3 *m) {
    LOGI("%s:\n%f %f %f\n%f %f %f\n%f %f %f",tag,
         m->m[0].x,m->m[1].x,m->m[2].x,
         m->m[0].y,m->m[1].y,m->m[2].y,
         m->m[0].z,m->m[1].z,m->m[2].z
    );
}

mat3 mat3_rotationYMat(float rot) {
    float r = rot * DEG_TO_RAD;
    float s=sinf(r);
    float c=cosf(r);
    //LOGI("degree:%f sin:%f cos%f",rot,s,c);
    mat3 m = {c,0,-s,0,1,0,s,0,c};

    //mat3_log((char *)"ROT X Mat:",&m);
    return m;
}

mat3 mat3_rotationXMat(float rot){
    float r = rot * DEG_TO_RAD;
    float s=sinf(r);
    float c=cosf(r);
    LOGI("degree:%f sin:%f cos%f",rot,s,c);
    mat3 m = {1,0,0,0,c,s,0,-s,c};

    mat3_log((char *)"ROT X Mat:",&m);
    return m;
}

vec4 mat4_muliply_vec4(mat4 *m, vec4 *v) {
    vec4 r;
    r.x = m->m[0].x*v->x + m->m[1].x*v->y + m->m[2].x*v->z + m->m[3].x*v->w;
    r.y = m->m[0].y*v->x + m->m[1].y*v->y + m->m[2].y*v->z + m->m[3].y*v->w;
    r.z = m->m[0].z*v->x + m->m[1].z*v->y + m->m[2].z*v->z + m->m[3].z*v->w;
    r.w = m->m[0].w*v->x + m->m[1].w*v->y + m->m[2].w*v->z + m->m[3].w*v->w;
    return r;
}

mat3 mat3_multiply_mat3(mat3 *m0, mat3 *m1 )
{
    mat3 mat = {
            m0->m[0].x*m1->m[0].x+m0->m[1].x*m1->m[0].y+m0->m[2].x*m1->m[0].z,
            m0->m[0].y*m1->m[0].x+m0->m[1].y*m1->m[0].y+m0->m[2].y*m1->m[0].z,
            m0->m[0].z*m1->m[0].x+m0->m[2].z*m1->m[0].y+m0->m[2].z*m1->m[0].z,

            m0->m[0].x*m1->m[1].x+m0->m[1].x*m1->m[1].y+m0->m[2].x*m1->m[1].z,
            m0->m[0].y*m1->m[1].x+m0->m[1].y*m1->m[1].y+m0->m[2].y*m1->m[1].z,
            m0->m[0].z*m1->m[1].x+m0->m[2].z*m1->m[1].y+m0->m[2].z*m1->m[1].z,

            m0->m[0].x*m1->m[2].x+m0->m[1].x*m1->m[2].y+m0->m[2].x*m1->m[2].z,
            m0->m[0].y*m1->m[2].x+m0->m[1].y*m1->m[2].y+m0->m[2].y*m1->m[2].z,
            m0->m[0].z*m1->m[2].x+m0->m[2].z*m1->m[2].y+m0->m[2].z*m1->m[2].z
    };

    return mat;
}