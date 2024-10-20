#define _POSIX_C_SOURCE 199309L
#include "summ.h"
#include "inits.h"

unsigned long long calc_elapsed_time(const struct timespec *beg, const struct timespec *end)
{
    return ((unsigned long long)
        (end->tv_sec - beg->tv_sec) * 1000 * 1000 * 1000 + (end->tv_nsec - beg->tv_nsec)) / 1000; 
}

void summ_matrix_standart(matrix_t *summ, matrix_t *pleft, matrix_t *pright)
{
    for (size_t i = 0; i < summ->rows; ++i)
    {
        for (size_t j = 0; j < summ->columns; ++j)
        {
            summ->matrix[i][j] = pleft->matrix[i][j] + pright->matrix[i][j];
        }
    }
}

unsigned long long clock_time_summ_matrix_standart(matrix_t *pleft, matrix_t *pright)
{
    srand(time(NULL));
    struct timespec t_beg, t_end;

    matrix_t *summ;
    summ->rows = pleft->rows;
    summ->columns = pleft->columns;
    matrix_alloc(summ);


    clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg);
    summ_matrix_standart(summ, pleft, pright);
    clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);

    unsigned long long time = calc_elapsed_time(&t_beg, &t_end);

    matrix_clean(summ);

    return time;
}

//-------------------------------

void summ_matrix_fast(csc_matrix_t *summ, csc_matrix_t *pleft, csc_matrix_t *pright)
{
    size_t cnt_summ = 0;
    size_t cnt1 = 0;
    size_t cnt2 = 0;
    summ->JA[0] = 0;
    for (size_t i = 1; i < summ->columns; ++i)
    {
        int first = pleft->JA[i] - pleft->JA[i - 1];
        int second = pright->JA[i] - pright->JA[i - 1]; 
        summ->JA[i] = first + second + summ->JA[i - 1];
        while(first > 0 || second > 0)
        {
            if (first > 0 && second > 0 && pleft->IA[cnt1] == pright->IA[cnt2])
            {
                summ->IA[cnt_summ] = pleft->IA[cnt1];
                summ->A[cnt_summ] = pleft->A[cnt1] + pright->A[cnt2]; 
                summ->JA[i]--;
                first--;
                second--;
                cnt1++;
                cnt2++;
            }
            else if ((first > 0 && second > 0 && pleft->IA[cnt1] < pright->IA[cnt2]) || (first > 0 && second <= 0))
            {
                summ->IA[cnt_summ] = pleft->IA[cnt1];
                summ->A[cnt_summ] = pleft->A[cnt1]; 
                first--;
                cnt1++;
            }
            else if (second > 0)
            {
                summ->IA[cnt_summ] = pright->IA[cnt2];
                summ->A[cnt_summ] = pright->A[cnt2]; 
                second--;
                cnt2++;
            }
            cnt_summ++;
        } 
    }
    summ->JA[summ->columns - 1] = cnt_summ;
    
}

unsigned long long clock_time_summ_matrix_fast(csc_matrix_t *pleft, csc_matrix_t *pright)
{
    srand(time(NULL));
    struct timespec t_beg, t_end;

    csc_matrix_t *summ;
    summ->len_A = pleft->len_A;
    summ->rows = pleft->rows;
    summ->columns = pleft->columns;

    csc_matrix_alloc(summ);

    clock_gettime(CLOCK_MONOTONIC_RAW, &t_beg);
    summ_matrix_fast(summ, pleft, pright);
    clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);

    unsigned long long time = calc_elapsed_time(&t_beg, &t_end);

    csc_matrix_clean(summ);

    return time;
}
