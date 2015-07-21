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
	PictureHandler();
	virtual ~PictureHandler();




private:

	std::vector<Picture> pictures;

	int pictureAmount;

};

} /* namespace andreasvh */

#endif /* PICTUREHANDLER_H_ */
