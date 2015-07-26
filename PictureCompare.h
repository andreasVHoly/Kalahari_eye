/*
 * PictureCompare.h
 *
 *  Created on: Jul 21, 2015
 *      Author: Andreas
 */

#ifndef PICTURECOMPARE_H_
#define PICTURECOMPARE_H_
#include "Picture.h"

namespace andreasvh {

class PictureCompare {
public:
	PictureCompare(int value):threshold(value){}
	~PictureCompare(){
		threshold = 0;
	}

    Picture comparePictures(Picture picture1, Picture picture2);


private:
    int threshold;

};

} /* namespace VHLAND002 */

#endif /* PICTURECOMPARE_H_ */
