/*
 * Picture.cpp
 *
 *  Created on: Jul 21, 2015
 *      Author: Andreas
 */

#include "Picture.h"

namespace andreasvh {



    //copy constructor
    Picture::Picture(const Picture & rhs){
        name = rhs.name;
        pathName = rhs.pathName;
        size = rhs.size;
        vectorSize = rhs.vectorSize;
        copy(rhs.imageData.begin(), rhs.imageData.end(), back_inserter(imageData));
    }
    //move constructor
    Picture::Picture(Picture && rhs):name(rhs.name), pathName(rhs.pathName), size(rhs.size), vectorSize(rhs.vectorSize), imageData(std::move(rhs.imageData)) {
        rhs.name == "";
        rhs.pathName = "";
        rhs.size = 0;
        rhs.vectorSize = 0;
    }

    //copy assignment opeerator
    Picture & Picture::operator=(const Picture & rhs){
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

    Picture & Picture::operator=(Picture && rhs){
        using namespace std;
        if (this != &rhs){
            name = rhs.name;
            pathName = rhs.pathName;
            size = rhs.size;
            vectorSize = rhs.vectorSize;
            imageData = std::move(rhs.imageData);
        }

        return *this;
    }






	Picture Picture::getPicture(){
		return *this;
	}


	std::string Picture::getName(){
		return name;
	}


	std::vector<int> & Picture::getImageData(){
        return imageData;
	}

	void Picture::renamePicture(std::string name){
		this->name = name;
	}



	Picture Picture::loadPicture(){
		if (pathName != ""){
			return loadPicture(pathName);
		}
		else{
            throw;//throw exception here
		}

	}



    //load method which loads in a file provided the file name is true
    Picture Picture::loadPicture(std::string fileName){
        using namespace std;
        ifstream in;
        //open the file
        in.open(fileName.c_str(), ios::binary);
        //if the file does not exist
        if (!in){
            cout << "Error reading file: File not found" << endl;
            return;
        }

        string line;
        //we loop until our base case breaks out
        while (true) {
            in >> line;//we read in a peace of data, the beginning char of a line
            if (line == "#"){//comment line
                string temp;
                getline(in, temp);//we discard it
            }
            else if (line == "P5"){//we ignore p5, is the start of the file
                continue;
            }
            else if (line == "255"){//when we arrive here the binary data starts to we break
                break;
            }
            else{//else we have the height and width of the image
                //for the width
                stringstream ss;
                ss << line;
                ss >> width;
                //for the height
                stringstream ss2;
                in >> line;
                ss2 << line;
                ss2 >> height;
            }
        }
        //we process the binary data
        //we make a temp char array
        char * array = new char[height*width];
        //we read the data into a char array
        in.read(array, height*width);
        //we convert the char array into a unsigned char array
        unsigned char * array2 = reinterpret_cast<unsigned char*>(array);
        //we make space for the data
        data.reset(new unsigned char[width*height]);
        //we assign the data
        data.reset(array2);
        //we close our file
        in.close();
        cout << "loaded image " << fileName << endl;
        return *this;
    }

    /**
     * saves and image to the specified image name
     */
    void Picture::savePicture(std::string fileName){
        using namespace std;

        ofstream out;
        out.open(fileName.c_str(), ios::binary);//we make the file

        //we write the nessesary header data to the file
        out << "P5\n";
        out << "# comment line\n";
        out << width << " " << height << "\n";
        out << "255\n";
        //we write the block of binary data after we convert it
        char * array = reinterpret_cast<char*>(data.get());
        out.write(array, width*height);
        //we close the file
        out.close();
        cout << "saved image " << fileName << endl;
    }



} /* namespace VHLAND002 */
