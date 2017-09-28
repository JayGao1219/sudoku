//To gengerate the random sudoku
#ifndef GENERATE_H
#define GENERATE_H

#include <ctime>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>

class sudoku{
	public:
    int sudo[81]={
				9,7,8,3,1,2,6,4,5,  
                3,1,2,6,4,5,9,7,8,  
                6,4,5,9,7,8,3,1,2,  
                7,8,9,1,2,3,4,5,6,  
                1,2,3,4,5,6,7,8,9,  
                4,5,6,7,8,9,1,2,3,  
                8,9,7,2,3,1,5,6,4,  
                2,3,1,5,6,4,8,9,7,  
                5,6,4,8,9,7,2,3,1
			};
    sudoku(){
            std::memset(mapper,0,sizeof(mapper));
		}
        int *get(){return sudo;}
        void map();
        bool *getLevel(int i);
	private:
        bool filled[81];
		int mapper[10];
		int hash();
		int hashnum;
        std::vector<int>a;

};

#endif
