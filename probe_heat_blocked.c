/*
	StencilProbe Heat Equation (Cache Blocked version)
	Impelements 7pt stencil from Chombo's heattut example,
	with cache blocking.
*/
#include "common.h"
#define MIN(x,y) (x < y ? x : y)
#define TI tx
#define TJ ty
#define TK tz

void StencilProbe(double* A0, double* Anext, int nx, int ny, int nz,
                  int tx, int ty, int tz, int timesteps)
{
  /* Fool compiler so it doesn't insert a constant here */
  double fac = A0[0];
  double *myA0, *myAnext;
  int t, i, ii, j, jj, k;


for (t = 0; t < timesteps; t++)
  {
    if (!(t%2))
    { myA0 = A0; myAnext = Anext; }
    else
    { myA0 = Anext; myAnext = A0; }

  /* Cache blocked stencil (due to Rivera) */
for (jj = 1; jj < ny-1; jj+=TJ)
  for (ii = 1; ii < nx - 1; ii+=TI)
  for (k = 1; k < nz - 1; k++)
    {
      for (j = jj; j < MIN(jj+TJ-1,ny - 1); j++)
	{
	  for (i = ii; i < MIN(ii+TI-1,nx - 1); i++)
	    {
		Anext[Index3D (nx, ny, i, j, k)] = 
			A0[Index3D (nx, ny, i, j, k + 1)] +
		    A0[Index3D (nx, ny, i, j, k - 1)] +
		    A0[Index3D (nx, ny, i, j + 1, k)] +
		    A0[Index3D (nx, ny, i, j - 1, k)] +
		    A0[Index3D (nx, ny, i + 1, j, k)] +
		    A0[Index3D (nx, ny, i - 1, j, k)]
			- 6.0 * A0[Index3D (nx, ny, i, j, k)] / (fac*fac);
	    }
	}
    }
  }
}
