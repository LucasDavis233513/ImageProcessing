#include "ImageType.h"

using namespace std;

// Create an empty image
ImageType::ImageType() {
    this->N = 0;
    this->M = 0;
    this->Q = 0;
    this->pixelValue = NULL;
}

// Create a blank/white image
ImageType::ImageType(int tmpN, int tmpM, int tmpQ) {
    int i, j;
    this->N = tmpN;
    this->M = tmpM;
    this->Q = tmpQ;
    this->pixelValue = new int *[N];

    for (i = 0; i < N; i++) {
        this->pixelValue[i] = new int[M];
        for (j = 0; j < M; j++)
            this->pixelValue[i][j] = 0;
    }
}

// Create a copy of the image
ImageType::ImageType(ImageType &oldImage) {
    int i, j;
    this->N = oldImage.N;
    this->M = oldImage.M;
    this->Q = oldImage.Q;
    this->pixelValue = new int *[N];

    for (i = 0; i < N; i++) {
        this->pixelValue[i] = new int[M];
        for (j = 0; j < M; j++)
            this->pixelValue[i][j] = oldImage.pixelValue[i][j];
    }
}

// Delete the image
ImageType::~ImageType() {
    int i;
    for (i = 0; i < N; i++)
        delete[] this->pixelValue[i];

    delete[] this->pixelValue;
}

// Private Methods
// Asks the user for the path of the image
char* ImageType::FindImage() {
    // Alocate memory for the imageName
    char* imageName = (char*)malloc(MAX_PATH_LENGTH * sizeof(char));

    // Exit if we failed to allocate memory for the image path
    if (imageName == NULL) {
        cerr << "Failed to alocate memory for the image path\n";
        return NULL;
    }

    printf("Path of the image and its name: ");
    cin >> imageName;

    return imageName;
}

// Public Methods
void ImageType::GetImageInfo(int &rows, int &cols, int &levels) {
    rows = this->N;
    cols = this->M;
    levels = this->Q;
}

void ImageType::SetImageInfo(int rows, int cols, int levels) {
    // If pixelValue was already allocated, free it to avoid memory leaks
    if (this->pixelValue != NULL) {
        for (int i = 0; i < this->N; i++) {
            delete[] this->pixelValue[i];
        }
        delete[] this->pixelValue;
    }

    // Set the new image dimensions and pixel depth
    this->N = rows;
    this->M = cols;
    this->Q = levels;

    // Allocate memory for pixelValue array
    this->pixelValue = new int*[N];
    for (int i = 0; i < N; i++) {
        this->pixelValue[i] = new int[M];
    }

    // Optionally initialize pixel values (e.g., set to 0)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            this->pixelValue[i][j] = 0;  // Initialize all pixels to 0
        }
    }
}

void ImageType::GetPixelVal(int i, int j, int &val) {
    val = this->pixelValue[i][j];
}

void ImageType::SetPixelVal(int i, int j, int val) {
    this->pixelValue[i][j] = val;
}

// Write an image to a file
int ImageType::WriteImage() {
    int N, M, Q, val;
    unsigned char *charImage;
    ofstream ofp;

    char* fname = this->FindImage();
    if (fname == NULL) {
        cerr << "Unable to allocate memory for the filepath";
        return 1;
    }

    this->GetImageInfo(N, M, Q);

    charImage = (unsigned char *)new unsigned char [M*N];
    if (charImage == NULL) {
        cerr << "Failed to allocate memory for charImage\n";
        return 2;
    }

    // Convert integer values to unsigned char
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            this->GetPixelVal(i, j, val);
            charImage[i*M+j] = (unsigned char)val;
        }
    }

    ofp.open(fname, ios::out | ios::binary);
    if(!ofp) { // Couldn't open the file
        cerr << "Can't Open file " << fname << endl;
        delete[] charImage; // Freeing the allocated memory
        return 3;
    }

    ofp << "P5" << endl;
    ofp << N << " " << M << endl;
    ofp << Q << endl;

    ofp.write(reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    if(ofp.fail()) { // Couldn't write the image
        cerr << "Can't Write Image " << fname << endl;
        delete[] charImage; // Freeing the allocated memory
        return 4;
    }

    ofp.close();
    delete[] charImage; // Freeing the allocated memory
    return 0; // Return 0 on success
}

// Read an image to a file
int ImageType::ReadImage() {
    int N, M, Q, val;               // Rows, Colummns, Gery Level, and pixel value variables
    unsigned char *charImage;
    char header [100], *ptr;
    ifstream ifp;

    char* fname = this->FindImage();
    if (fname == NULL) {
        cerr << "Unable to allocate memory for the filepath";
        return 1;
    }

    ifp.open(fname, ios::in | ios::binary);

    if (!ifp) {
        cerr << "Can't read image: " << fname << endl;
        return 2;
    }

    // read header
    ifp.getline(header,100,'\n');
    if ( (header[0]!=80) ||    /* 'P' */
        (header[1]!=53) ) {   /* '5' */
        cerr << "Image " << fname << " is not PGM\n";
        return 3;
    }

    ifp.getline(header,100,'\n');
    while(header[0]=='#')
        ifp.getline(header,100,'\n');

    M=strtol(header,&ptr,0);
    N=atoi(ptr);

    ifp.getline(header,100,'\n');
    Q=strtol(header,&ptr,0);

    charImage = (unsigned char *) new unsigned char [M*N];
    if (charImage == nullptr) {
        cerr << "Memory allocation for charImage failed!\n";
        return 4;
    }

    ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));
    cout << "Bytes read: " << ifp.gcount() << endl;

    if (ifp.fail()) {
        cerr << "Image " << fname << " has wrong size\n";
        delete[] charImage;
        return 5;
    }

    ifp.close();

    this->SetImageInfo(N, M, Q);

    // Convert the unsigned characters to integers
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            val = static_cast<int>(charImage[i * M + j]);
            this->SetPixelVal(i, j, val);
        }
    }

    delete[] charImage;

    return 0;
}