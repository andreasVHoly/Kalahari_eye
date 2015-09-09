/*
 * PictureHandler.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: Andreas
 */

#include "PictureHandler.h"

namespace andreasvh {

    void PictureHandler::addPicture(Picture pic){
        pictures.push_back(pic);
    }

    bool PictureHandler::deletePicture(Picture pic){
        return deletePicture(pic.getName());
    }

    bool PictureHandler::deletePicture(std::string name){
        std::vector<Picture>::iterator start = pictures.begin();
        std::vector<Picture>::iterator end = pictures.end();

        //TODO could use an STL alg here?
        //maybe also just use for loop, that way we have a counter and can easily access
        while(start != end){
            if (*start.getName() == name){
                //we delete the image
                return true;
            }
            start++;
        }

        for (int i = 0; i < pictures.size(); i++){
            if (pictures[i].getName() == name){
                //we delete the image
                reshuffleData(i);
                pictureAmount--;
                return true;
            }
        }

        return false;
    }

    void PictureHandler::reshuffleData(int index){


        for (int i = index; i < pictureAmount-2; i++){
            pictures[i] = pictures[i+1];
        }
    }

    Picture PictureHandler::getPicture(std::string name){
        //loop through until we get a picture we want and then return it
    }

    Picture PictureHandler::getPicture(int position){
        return pictures[position];
    }

    int PictureHandler::getPictureAmount(){
        return pictureAmount;
    }

    Picture PictureHandler::getCurrentPicture(){
        return pictures[currentPicture];
    }

    Picture PictureHandler::getPreviousPicture(){
        return pictures[currentPicture-1];
    }

    void PictureHandler::setPictureIndex(int index){
        currentPicture = index;
    }

} /* namespace andreasvh */
