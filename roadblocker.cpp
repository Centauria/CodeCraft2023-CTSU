//
// Created by 蔡恩光 on 2023/4/2.
//

#include "roadblocker.h"
#include "matrix.h"

void RoadBlocker::block_wide_road(DMatrix &Dmap){

}
void RoadBlocker::block_narrow_road(DMatrix &Dmap){
    DMatrix temp = convolve(Dmap, Index{3, 3}, [](View<double> v)->double{
        if(v(1,1) < 0) return v(1, 1);
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(i == j && i != 1) continue;
                if(v(i, 0) == 0 && v(j, 2) == 0) return -1;
                if(v(0, i) == 0 && v(2, j) == 0) return -1;
            }
        }
        return v(1, 1);
    });
    //TODO: 把temp覆盖到原来的Dmap上
    /*for (int j = 1; j < 99; j++)
    {
        for (int i = 1; i < 99; i++)// j,i 是原始障碍物
        {
            if (Dmap(j, i) != 0) continue;
            for (int jj = j - 1; jj <= j + 1; jj++)
            {
                for (int ii = i - 1; ii <= i + 1; ii++)// jj,ii 是原始障碍物外面那一圈玩意
                {
                    if (jj == j && ii == i) continue;
                    if (Dmap(jj, ii) == 0) continue;
                    if (jj == 0 || jj == 99 || ii == 0 || ii == 99)
                    {
                        Dmap(jj, ii) = -1;
                        continue;
                    }
                    for (int jjj = jj - 1; jjj <= jj + 1; jjj++)
                    {
                        for (int iii = ii - 1; iii <= ii + 1; iii++)
                        {
                            if ((jjj - j == 2 || iii - i == 2) && Dmap(jjj, iii) == 0)
                            {
                                Dmap(jj, ii) = -1;
                            }
                        }
                    }
                }
            }
        }
    }*/
}
