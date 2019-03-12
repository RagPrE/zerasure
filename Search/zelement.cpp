#include "zelement.h"
#include <sys/time.h>
#include <immintrin.h>
#include <cstring>
#include "../utils.h"
#include <stdio.h>
#include "../Algorithm/zoxc.h"
extern "C"{
#include "../Jerasure-1.2A/jerasure.h"
#include "../Jerasure-1.2A/cauchy.h"
}

int ZElement::cpy_weight = 1532;
int ZElement::xor_weight = 2999;
int ZElement::K = 0;
int ZElement::M = 0;
int ZElement::W = 0;
int ZElement::cost_func = 0;
int ZElement::strategy = 0;

long long schedule_weight_3(vector<int*> &sch)
{
    long long ret = 0;
    int i;
    int size = sch.size();
    for(i = 0;i<size;i++)
    {
        switch(ZElement::cost_func)
        {
        case 0:
            if(sch[i][2] == 1 || sch[i][2] == 3 || sch[i][2] == 5) ret++; // consider only xor
            break;
        case 1:
            ret ++;
            break;
        case 2:
            if(sch[i][2] == 1 || sch[i][2] == 3 || sch[i][2] == 5) // add weight 214 to xor
                ret += ZElement::xor_weight;
            else
                ret += ZElement::cpy_weight;
            break;
        }
    }
    return ret;
}

ZElement::ZElement(int *p)
{
    array = vector<int>(p,p+K+M);
    for(int i = 0;i<K+M;i++)
        printf("%d ",array[i]);
    printf("\n");
}

ZElement::ZElement(vector<int> p)
{
    array = p;
    for(int i = 0;i<K+M;i++)
        printf("%d ",array[i]);
    printf("\n");
}

void ZElement::init(int tK, int tM, int tW, int tcost, int tstrategy)
{
    K = tK;
    M = tM;
    W = tW;
    cost_func = tcost;
    strategy = tstrategy;
}

long long ZElement::value()
{
    long long xor0_natual_dumb;
    long long xor1_natual_smart;
    long long xor2_natual_grouping_unweighted;
    long long xor3_natual_grouping_weighted;
    long long xor4_normal_dumb;
    long long xor5_normal_smart;
    long long xor6_normal_grouping_unweighted;
    long long xor7_normal_grouping_weighted;

    ZOXC *xc;
    int *matrix;
    int *bitmatrix;
    int **schedule;
    matrix = cauchy_xy_coding_matrix(K,M,W,array.data()+K,array.data());

    switch(strategy)
    {
//    case 0:
//        // natual dumb
//        bitmatrix = jerasure_matrix_to_bitmatrix(K,M,W,matrix);
//        schedule = jerasure_dumb_bitmatrix_to_schedule(K,M,W,bitmatrix);
//        xor0_natual_dumb = schedule_len_5(schedule);
//        free2dSchedule5(schedule);
//        free(bitmatrix);
//        free(matrix);
//        return xor0_natual_dumb;
//        break;
//    case 1:
//        // natual smart
//        bitmatrix = jerasure_matrix_to_bitmatrix(K,M,W,matrix);
//        schedule = jerasure_smart_bitmatrix_to_schedule(K,M,W,bitmatrix);
//        xor1_natual_smart = schedule_len_5(schedule);
//        free2dSchedule5(schedule);
//        free(bitmatrix);
//        free(matrix);
//        return xor1_natual_smart;
//        break;
//    case 2:
//        // natual grouping unweighed
//        bitmatrix = jerasure_matrix_to_bitmatrix(K,M,W,matrix);
//        xc = new ZOXC(K*W, M*W);
//        xc->grouping_1s(bitmatrix, false);
//        xor2_natual_grouping_unweighted = schedule_weight_3(xc->schedule);
//        free(bitmatrix);
//        free(matrix);
//        delete xc;
//        return xor2_natual_grouping_unweighted;
//        break;
//    case 3:
//        // natual grouping weightd
//        bitmatrix = jerasure_matrix_to_bitmatrix(K,M,W,matrix);
//        xc = new ZOXC(K*W, M*W);
//        xc->grouping_1s(bitmatrix, true);
//        xor3_natual_grouping_weighted = schedule_weight_3(xc->schedule);
//        free(bitmatrix);
//        free(matrix);
//        delete xc;
//        return xor3_natual_grouping_weighted;
//        break;
//    case 4:
//        // normal dumb
//        cauchy_improve_coding_matrix(K,M,W,matrix);
//        bitmatrix = jerasure_matrix_to_bitmatrix(K,M,W,matrix);
//        schedule = jerasure_dumb_bitmatrix_to_schedule(K,M,W,bitmatrix);
//        xor4_normal_dumb = schedule_len_5(schedule);
//        free2dSchedule5(schedule);
//        free(bitmatrix);
//        free(matrix);
//        return xor4_normal_dumb;
//        break;
//    case 5:
//        // normal smart
//        cauchy_improve_coding_matrix(K,M,W,matrix);
//        bitmatrix = jerasure_matrix_to_bitmatrix(K,M,W,matrix);
//        schedule = jerasure_smart_bitmatrix_to_schedule(K,M,W,bitmatrix);
//        xor5_normal_smart = schedule_len_5(schedule);
//        free2dSchedule5(schedule);
//        free(bitmatrix);
//        free(matrix);
//        return xor5_normal_smart;
//        break;
//    case 6:
//        // normal unweighted
//        cauchy_improve_coding_matrix(K,M,W,matrix);
//        bitmatrix = jerasure_matrix_to_bitmatrix(K,M,W,matrix);
//        xc = new ZOXC(K*W, M*W);
//        xc->grouping_1s(bitmatrix, false);
//        xor6_normal_grouping_unweighted = schedule_weight_3(xc->schedule);
//        free(bitmatrix);
//        free(matrix);
//        delete xc;
//        return xor6_normal_grouping_unweighted;
//        break;
    case 7:
        // normal weighted
        cauchy_improve_coding_matrix(K,M,W,matrix);
        bitmatrix = jerasure_matrix_to_bitmatrix(K,M,W,matrix);
        jerasure_print_bitmatrix(bitmatrix,M*W,K*W,W);
        xc = new ZOXC(K*W, M*W);
        xc->grouping_1s(bitmatrix, true);
        xor7_normal_grouping_weighted = schedule_weight_3(xc->schedule);
        printf("%d,%d\n",xor7_normal_grouping_weighted, xc->schedule.size());
        for(int i = 0;i<xc->schedule.size();i++)
            printf("\t %d %d %d\n",xc->schedule[i][0],xc->schedule[i][1],xc->schedule[i][2]);
        free(bitmatrix);
        free(matrix);
        delete xc;
        return xor7_normal_grouping_weighted;
        break;
    }

    return 0;
}

// test the weight of memcpy and xor using given size of data, 1GB by default
// test 10 times, in each test run memcpy and xor *loops* times
void ZElement::test_cost_weight(int size, int loops)
{
    printf("Test_cost_weight: size %d, loops %d\n", size, loops);
    cpy_weight = 0;
    xor_weight = 0;
    for(int i = 0;i<10;i++)
    {
        char *dat, *dat1, *dat2;

        posix_memalign((void**)&dat, 32, size);
        posix_memalign((void**)&dat1, 32, size);
        posix_memalign((void**)&dat2, 32, size);
        struct timeval t0,t1;

        gettimeofday(&t0,NULL);
        for(int j = 0;j<loops;j++)
            memcpy(dat1,dat,size);
        gettimeofday(&t1,NULL);
        cpy_weight += diff_us(t0,t1);

        gettimeofday(&t0,NULL);
        for(int j = 0;j<loops;j++)
            fast_xor(dat,dat1,dat1, size);
        gettimeofday(&t1,NULL);
        xor_weight += diff_us(t0,t1);

        free(dat);
        free(dat1);
        free(dat2);
    }
    printf("cpy_weight = %d, xor_weight = %d\n", cpy_weight, xor_weight);

    // scale to 0~9999 to avoid overflow
    while(cpy_weight > 10000)
    {
        cpy_weight /= 10;
        xor_weight /= 10;
    }
    printf("cpy_weight = %d, xor_weight = %d\n", cpy_weight, xor_weight);
}
