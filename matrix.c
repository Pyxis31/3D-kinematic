/**********************************************
 ********** Directives d'inclusions ***********
 ************ (du préprocesseur) **************
 **********************************************
*/

// Des Headers situés dans les répertoires listés dans le makefile
#include <math.h>


// Des Headers situés dans le répertoire de l'application


// Des définitions locales


/**********************************************
 ********** Déclarations (globales) ***********
 **********************************************
*/

// Externes

// Types de base

// Pointeurs (sur tableaux, sur chaînes, etc.)

// Pointeurs de widgets de l'interface


#ifndef MATRICES

void mat_frustum(float* pMatrix,float angle_of_view,float aspect_ratio,float z_near,float z_far)
{
	pMatrix[0]=1,	pMatrix[4]=0,									pMatrix[8]=0,								pMatrix[12]=0;
	pMatrix[1]=0,	pMatrix[5]=aspect_ratio/tanf(angle_of_view),	pMatrix[9]=0,								pMatrix[13]=0;
	pMatrix[2]=0,	pMatrix[6]=0,									pMatrix[10]=(z_far+z_near)/(z_far-z_near),	pMatrix[14]=-2*z_far*z_near/(z_far-z_near);
	pMatrix[3]=0,	pMatrix[7]=0,									pMatrix[11]=1,								pMatrix[15]=0;
}

void mat_translate(float* pMatrix,float dx,float dy,float dz)
{
	pMatrix[0]=1,	pMatrix[4]=0,	pMatrix[8]=0,	pMatrix[12]=dx;
	pMatrix[1]=0,	pMatrix[5]=1,	pMatrix[9]=0,	pMatrix[13]=dy;
	pMatrix[2]=0,	pMatrix[6]=0,	pMatrix[10]=1,	pMatrix[14]=dz;
	pMatrix[3]=0,	pMatrix[7]=0,	pMatrix[11]=0,	pMatrix[15]=1;
}

void mat_scale(float* pMatrix,float dx,float dy,float dz)
{
	pMatrix[0]=dx,	pMatrix[4]=0,	pMatrix[8]=0,	pMatrix[12]=0;
	pMatrix[1]=0,	pMatrix[5]=dy,	pMatrix[9]=0,	pMatrix[13]=0;
	pMatrix[2]=0,	pMatrix[6]=0,	pMatrix[10]=dz,	pMatrix[14]=0;
	pMatrix[3]=0,	pMatrix[7]=0,	pMatrix[11]=0,	pMatrix[15]=1;
}

static void normalize(float* pX,float* pY,float* pZ)
{
	float d=sqrtf((*pX)*(*pX)+(*pY)*(*pY)+(*pZ)*(*pZ));
	*pX/=d;
	*pY/=d;
	*pZ/=d;
}

void mat_rotate(float* pMatrix,float x, float y, float z, float angle)
{
	normalize(&x,&y,&z);

	float s=sinf(angle);
	float c=cosf(angle);
	float m=1-c;

	pMatrix[0]=m*x*x+c,		pMatrix[4]=m*x*y+z*s,	pMatrix[8]=m*z*x-y*s,	pMatrix[12]=0;
	pMatrix[1]=m*x*y-z*s,	pMatrix[5]=m*y*y+c,		pMatrix[9]=m*y*z+x*s,	pMatrix[13]=0;
	pMatrix[2]=m*z*x+y*s,	pMatrix[6]=m*y*z-x*s,	pMatrix[10]=m*z*z+c,	pMatrix[14]=0;
	pMatrix[3]=0,			pMatrix[7]=0,			pMatrix[11]=0,			pMatrix[15]=1;
}

void mat_multiply(float* pMatrix,float* pA,float* pB)
{
	float result[16];
	for (int c=0;c<4;c++)
	{
		for (int r=0;r<4;r++)
		{
			int index=c*4+r;
			float total=0;
			for (int i=0;i<4;i++)
			{
				int p=i*4+r;
				int q=c*4+i;
				total+=pA[p]*pB[q];
			}
			result[index]=total;
		}
	}
	for (int i=0;i<16;i++)
		pMatrix[i]=result[i];
}

#endif
