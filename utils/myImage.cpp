#include "myImage.h"


void myImage::setIndex(int id){
	// cout <<"set index"<< id <<endl; 
	index = id;
}

void myImage::setLabel(string lb){
	// cout <<"set label "<< lb <<endl; 
	label = lb;
}

void myImage::setPath(string ph){
	// cout <<"set path"<< ph <<endl; 
	path = ph;
}


int myImage::getIndex() {  
	//cout <<"got index "<<index<<endl; 
	return index;
}

string myImage::getLabel() {  
	//cout <<"got label "<<label<<endl; 
	return label;
}

string myImage::getPath() { 
	//cout <<"got path "<<path<<endl; 
	return path;
}