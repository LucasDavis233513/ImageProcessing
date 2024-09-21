# ComputerVisionRepo
This repo will hold the tools to preform computer vision related tasks.

## Image Processing
This section will explain the various image processing tasks needed to be performed before any computer vison related operation can be preformed.

### ImageProcessing.h
This class will hold all the functions necessary to processes an image

#### Write Image
This is a method of the ImageProcessing class that writes an image to a file.
   - Parameters:
     - char *fname: C-style string representing the filename where the image is to be saved.
     - ImageType &image: A reference to an image object which is an instance of the ImageType class).
   - Returns:
     - 0 when it successfully wrote an image to a file
     - 1 when it failed to open/create the file
     - 2 when it failed to write the image

#### Read Image
This is a method of the ImageProcessing class that Reads an image to a file.
   - Parameters:
     - char *fname: C-style string representing the filename of the image.
     - ImageType &image: A reference to an image object which is an instance of the ImageType class.
   - Returns:
     - 0 when it successfully read an image to a file
     - 1 when it failed to open the file
     - 2 when it failed to Read the image