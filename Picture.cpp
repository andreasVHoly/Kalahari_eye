/*
 * Picture.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: Andreas
 */

#include "Picture.h"

namespace andreasvh {


	Picture Picture::getPicture(){
		return *this;
	}


	std::string Picture::getName(){
		return name;
	}


	std::vector<int> & Picture::getImageData(){
		return &imageData;
	}

	void Picture::renamePicture(std::string name){
		this->name = name;
	}

	Picture Picture::loadPicture(std::string pathname){
		//go about loading the picture
		return *this;
	}

	Picture Picture::loadPicture(){
		if (pathName != ""){
			return loadPicture(pathName);
		}
		else{
			throw;
		}

	}



} /* namespace VHLAND002 */
