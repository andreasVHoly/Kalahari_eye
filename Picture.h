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
	Picture(const Picture & rhs){
		name = rhs.name;
		pathName = rhs.pathName;
		size = rhs.size;
		vectorSize = rhs.vectorSize;
		copy(rhs.imageData.begin(), rhs.imageData.end(), back_inserter(imageData));
	}
	//move constructor
	Picture(Picture && rhs):name(rhs.name), pathName(rhs.pathName), size(rhs.size), vectorSize(rhs.vectorSize), imageData(std::move(rhs.imageData)) {
		rhs.name == "";
		rhs.pathName = "";
		rhs.size = 0;
		rhs.vectorSize = 0;
	}

	//copy assignment opeerator
	Picture & operator=(const Picture & rhs){
		using namespace std;
		if (this != &rhs){
			name = rhs.name;
			pathName = rhs.pathName;
			size = rhs.size;
			vectorSize = rhs.vectorSize;
			copy(rhs.imageData.begin(), rhs.imageData.end(), back_inserter(imageData));
		}

		return *this;
	}

	Picture & operator=(Picture && rhs){
		using namespace std;
		if (this != &rhs){
			name = rhs.name;
			pathName = rhs.pathName;
			size = rhs.size;
			vectorSize = rhs.vectorSize;
			imageData = move(rhs.imageData);
		}

		return *this;
	}




	//getters and setters
	Picture getPicture();

	std::string getName();


	void renamePicture(std::string name);


	//loaading adn saving
	Picture loadPicture(std::string pathname);

	Picture loadPicture();

	std::vector<int> & getImageData();






private:
	std::string name, pathName;
	std::vector<int> imageData;
	float size;//size in mb
	int vectorSize;//vector size



};

} /* namespace andreasvh */

#endif /* PICTURE_H_ */
