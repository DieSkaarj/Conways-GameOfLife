#include <ncurses.h>

#include "CGame.h"
#include "CCluster.h"

void CCluster::evolve(){

	VCluster	nextGen(m_cells);

	for(size_t i=0;i<nextGen.size();++i){

		switch(getCell(i)->getIsExtant()){

			case true:{

				if(getLivingNeighbours(i)>1
				&& getLivingNeighbours(i)<4)
					nextGen[i].setIsExtant(true);
				else
					nextGen[i].setIsExtant(false);
			}	break;

			case false:{

				if(getLivingNeighbours(i)==3)
					nextGen[i].setIsExtant(true);
			}	break;
		}
	}

	m_cells=nextGen;
}

int CCluster::getLivingNeighbours(int t_cellId){

	int	count{0},
		n{0},
		nX{0},
		nY{0};

	int	xX=((t_cellId)/m_factor);
	int	yY=((t_cellId)%m_factor);

	for(int y=-1;y<=1;++y){
		
		nY=(y+yY);

		if(nY>=m_factor)	nY-=m_factor;
		if(nY<0)			nY+=m_factor;					

		for(int x=-1;x<=1;++x){

			if(!x&&!y)continue;

			nX=(xX+x);

			if(nX>=m_factor)	nX-=m_factor;
			if(nX<0)			nX+=m_factor;

			n=(nX*m_factor)+nY;

			if(getCell(n)->getIsExtant()==true)	++count;
		}
	}

	return count;
}

void CCluster::clear(){

	for(auto& cell:m_cells){
		
		cell.setIsExtant(false);
	};
}

void CCluster::destroy(){

}
