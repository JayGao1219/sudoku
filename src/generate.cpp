#include "generate.h"
#include <QDebug>

void sudoku::map(){
    srand(time(0));
	bool flag=false;
	int index=0;
	while(index<9){
		int t=rand()%9+1;
        for(int i=0;i<index+1;++i){
			if(t==mapper[i]){
				flag=true;
				break;
			}
		}
        if(flag==false){
			++index;
			mapper[index]=t;
		}
		flag=false;
	}
	for(int i=0;i<81;++i){
		sudo[i]=mapper[sudo[i]];
	}
}

bool * sudoku::getLevel(int index){
    index=index*4+10;
    srand(time(0));
    bool flag=false;
    int i=0;
    while(i<index){
        int t=rand()%81;
        for(int k=0;k<a.size();++k)
            if(t==a[k]){
                flag=true;
                break;
            }
            if(!flag){
                ++i;
                a.push_back(t);
            }
            flag=false;
        }
    memset(filled,true,sizeof(filled));
    for(int j=0;j<index;++j)
        filled[a[j]]=false;
    return filled;
}
