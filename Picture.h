/*
 * Picture.h
 *
 *  Created on: Jul 21, 2015
 *      Author: Andreas
 */

#ifndef PICTURE_H_
#define PICTURE_H_


#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>


namespace andreasvh {

class Picture {
public:
	Picture():pathName(""),size(0), vectorSize(0), width(0), height(0){}
	Picture(std::string pathname):pathName(pathname),size(0), vectorSize(0), width(0), height(0){}


	~Picture(){
		imageData.clear();
	}



    //copy constructor
    Picture(const Picture & rhs);
    //move constructor
    Picture(Picture && rhs);

    //copy assignment opeerator
    Picture & operator=(const Picture & rhs);

    Picture & operator=(Picture && rhs);




	//getters and setters
	Picture getPicture();

	std::string getName();


	void renamePicture(std::string name);


    //loading and saving
	void loadPicture(std::string pathname);

	void loadPicture();

    bool savePicture(std::string name);

	std::vector<int> & getImageData();


	int getWidth(){
		return width;
	}

	int getHeight(){
		return height;
	}

	int getSize(){
		return width*height;
	}



private:
	std::string name, pathName;
	std::vector<int> imageData;
	float size;//size in mb
	int vectorSize;//vector size
	int width,height;



};

} /* namespace andreasvh */

#endif /* PICTURE_H_ */
