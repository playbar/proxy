#ifndef _base_h
#define _base_h

struct nocopystruct
{
    nocopystruct(const nocopystruct&);
	nocopystruct& operator=(const nocopystruct&);
    
	nocopystruct(){};
	~nocopystruct(){};

};

#endif //_base_h