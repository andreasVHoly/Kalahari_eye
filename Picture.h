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



namespace andreasvh {

class Picture {
public:
	Picture():pathName(""){}
	Picture(std::string pathname):pathName(pathname){}


	virtual ~Picture();



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
	Picture loadPicture(std::string pathname);

	Picture loadPicture();

    bool savePicture(std::string name);

	std::vector<int> & getImageData();






private:
	std::string name, pathName;
	std::vector<int> imageData;
	float size;//size in mb
	int vectorSize;//vector size



};

} /* namespace andreasvh */

#endif /* PICTURE_H_ */
