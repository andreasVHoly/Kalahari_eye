


#include "Picture.h"
#include "PictureCompare.h"
#include "PictureHandler.h"



/**
 * format:
 * -picture inputfilename outputfilename
 *
 */


int main(int argc, char * argv[]){
	using namespace std;

	string imageName = "";
	string imageName2 = "";
	string outImageName = "";
	bool debug = true;

	andreasvh::PictureHandler picHandler;
	andreasvh::PictureCompare compare(30);

	if (debug){
		imageName = "donkey_mask.pgm";
		imageName2 =  "donkey_mask1.pgm";
	}
	else{
		//imageName = string(argv[1]);//image 1
		//imageName2 = picHandler.getCurrentPicture();//image 1
		//outImageName = string(argv[2]);//output image name

	}

	//usually first image would come from the imagemanager




	andreasvh::Picture firstImage;// new image 1 object
	andreasvh::Picture newImage;// new image 1 object
	andreasvh::Picture outImage; //new image 3 object
	//we load both images from the supplied file names
	newImage.loadPicture(imageName);
	firstImage.loadPicture(imageName2);

	outImage = newImage;
	outImage.getImageData().clear();
	outImage = compare.comparePictures(firstImage, newImage);
	outImage.savePicture("newfile.pgm");

	//picHandler.addPicture(firstImage);



	cout << "test" << endl;


	return 0;
}//main















