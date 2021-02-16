/************************************************************************/
/*																		*/
/*																		*/
/*	Class:	CCluster													*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

#pragma once

#include <vector>

#include "CCell.h"

class CCell;

typedef std::vector<CCell> VCluster;

class	CCluster{

	public:
	
		CCluster(int t_factor):
		m_factor(t_factor),m_liveCount(0)
		{CCell::resetId();m_cells.resize(t_factor*t_factor);clear();}

		~CCluster()
		{destroy();}

		/*
		 *	Looks at immediate and adjacent squares and count how many are alive.
		 */

		int			getLivingNeighbours(int cellId);

		CCell*		getCell(int t_id)	{return &m_cells[t_id];}
		VCluster*	getCells()			{return &m_cells;}
		int			getFactor()			{return m_factor;}
		int			getSize()			{return m_cells.size();}
		int			getLiveCount()		{m_liveCount=0; for(auto& n:m_cells) if(n.getIsExtant()) ++m_liveCount; return m_liveCount;}

		void		evolve();
		void		setNextGen(VCluster* cells){m_cells=*cells;}
		void		clear();

	private:

        void 	   destroy();

	private:

		int			m_factor;
		VCluster	m_cells;
		int			m_liveCount;
};
