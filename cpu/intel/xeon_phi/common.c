#include "common.h"


// global variables
int g_balancer;
int g_m;
int g_n;
int g_nnz;
fptype_t *g_x;
int *g_partition;
fptype_t *g_y;
pthread_mutex_t *g_lock_y;
TreeBarrier tbarrier;
char g_filename[1024];
double g_cpufreq;
__int64 mytime[MAXTHREADS * 8];
char *g_isgprefetch;
int g_nqueue;
int g_ntasksperqueue;


void initvec (fptype_t * v, int size, fptype_t initval)
{
    int i;
    if (initval < 0.0)
    {
        for (i = 0; i < size; i++)
        {
            v[i] = (double)(i % 10000) / 10000;
            // TODO: improve random generator
            // v[i] = RAND01 ();
        }
    }
    else
    {
        for (i = 0; i < size; i++)
        {
            v[i] = initval;
        }
    }
}


void initomp (int nthreads, int verbose)
{
    char schedule[1024];

    if (verbose == 1)
    {
        sprintf (schedule,
                 "KMP_AFFINITY=granularity=fine,compact,verbose");
    }
    else
    {
        sprintf (schedule,
                 "KMP_AFFINITY=granularity=fine,compact");
    }
    kmp_set_defaults (schedule);
    omp_set_num_threads (nthreads);
}


__attribute__((noinline)) void trace1()
{
    printf ("HERE\n");
}


__attribute__((noinline)) void trace2()
{
    printf ("HERE\n");
}


__attribute__((noinline)) void trace3()
{
    printf ("HERE\n");
}


void KNC_printVint (__m512i reg, __mmask16 mask, char *regname)
{
    __declspec (align (64)) int tmp1_[16];
    int i;
    for (i = 0; i < 16; i++)
    {
        tmp1_[i] = 0;
    }
    _mm512_mask_store_epi32 (tmp1_, mask, reg);
    printf ("%s(INT):\n", regname);
    for (i = 0; i < 16; i++)
    {
        printf ("%d ", tmp1_[i]);
    }
    printf ("\n");

}


void KNC_printVfp (__m512 reg, __mmask16 mask, char *regname)
{
    __declspec (align (64)) fptype_t tmp1_[16];
    int i;
    for (i = 0; i < 16; i++)
        tmp1_[i] = -1.0;
    _mm512_mask_store_ps (tmp1_, mask, reg);
    printf ("%s(SP):\n", regname);
    for (i = 0; i < 16; i++)
    {
        printf ("%.3lf ", tmp1_[i]);
    }
    printf ("\n\n");

}


void KNC_printVdp (__m512d reg, __mmask8 mask, char *regname)
{
    int i;
    __declspec (align (64)) double tmp1_[16];
    for (i = 0; i < 16; i++)
        tmp1_[i] = -1.0;
   _mm512_mask_store_pd (tmp1_, mask, reg);
    printf ("%s(DP):\n", regname);
    for (i = 0; i < 8; i++)
    {
        printf ("%.3lf ", tmp1_[i]);
    }
    printf ("\n\n");

}
