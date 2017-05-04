#ifndef _myImage_H_
#define _myImage_H_

#include <iostream>
#include <string.h>
#include <string>
#include <string.h>

using namespace std;

class myImage{
	private:
	    int index;
	    string label;
	    string path;
	public:
		void setIndex(int id);
		void setLabel(string lb);
		void setPath(string ph);
	    int getIndex();
	    string getLabel();
	    string getPath();
};

#endif