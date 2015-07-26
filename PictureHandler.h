/*
 * PictureHandler.h
 *
 *  Created on: Jul 21, 2015
 *      Author: Andreas
 */

#ifndef PICTUREHANDLER_H_
#define PICTUREHANDLER_H_

#include "Picture.h"

namespace andreasvh {

class PictureHandler {
public:
	PictureHandler():currentPicture(0), pictureAmount(0){}


	~PictureHandler(){
		pictures.clear();

	}

	void addPicture(Picture pic);

	bool deletePicture(Picture pic);

	bool deletePicture(std::string name);

	Picture getPicture(std::string name);

	Picture getPicture(int position);

	int getPictureAmount();

	Picture getCurrentPicture();

	Picture getPreviousPicture();

	void setPictureIndex(int index);

	void reshuffleData(int index);


private:

	std::vector<Picture> pictures;
	int currentPicture;
	int pictureAmount;

};

} /* namespace andreasvh */

#endif /* PICTUREHANDLER_H_ */
