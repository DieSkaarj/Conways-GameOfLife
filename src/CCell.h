/************************************************************************/
/*																		*/
/*																		*/
/*	Class:	CCell														*/
/*																		*/
/*																		*/
/*																		*/
/************************************************************************/

#pragma once

class CCell{

    public:

	    static int	s_cellId;

    public:

	    CCell():
	    m_id(++s_cellId),b_isExtant(false)
	    {}
	    CCell(bool extant):
	    m_id(++s_cellId),b_isExtant(extant)
	    {}
        ~CCell(){}

        void    setIsExtant(bool t_isExtant)	{b_isExtant=t_isExtant;}
        bool    getIsExtant()					{return b_isExtant;}

        int		getId()							{return m_id;}
        static void	resetId()					{s_cellId=0;}

    private:

	    int		m_id;
	    bool	b_isExtant{false};
};

inline int CCell::s_cellId=0;
