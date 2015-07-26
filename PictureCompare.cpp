/*
 * PictureCompare.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: Andreas
 */

#include "PictureCompare.h"
#include "Picture.h"

namespace andreasvh {




    Picture PictureCompare::comparePictures(Picture picture1, Picture picture2){
		std::vector<int>::iterator start1 = picture1.getImageData().begin();
		std::vector<int>::iterator end1 = picture1.getImageData().end();
        std::vector<int>::iterator start2 = picture2.getImageData().begin();
        std::vector<int>::iterator end2 = picture2.getImageData().end();

        //picture1 - previous picture
        //pciture2 - new picture
        //newpic - new pic with bullet hole marked

        Picture newPic;
        newPic = picture2;
        newPic.getImageData().clear();
        newPic.getImageData().reserve(newPic.getSize());
        //we compare the 2 picture values and then return one with black marked where it was shot

        std::vector<int>::iterator start3 = newPic.getImageData().begin();



        //copying over any old bullet holes
        while(start1 != end1){

            int diff = std::abs(*start1 - *start2);

            if (diff > threshold){//hopefully this means a extreme change in colour meaning we have a shot here
				std::cout << "found pixel" << std::endl;
				*start3 = 150;//assuming 0 is black
			}
            else if (*start1 == 0){//assuming this is black?
                *start3 = 0;
            }

            else{
                *start3 = *start1;
            }

            start1++;
            start2++;
            start3++;
        }
        return newPic;

	}


} /* namespace VHLAND002 */
