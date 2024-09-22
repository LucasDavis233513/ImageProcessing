#ifndef IMAGE_TYPE_H
#define IMAGE_TYPE_H
// test
class ImageType {
  private:
    int N, M, Q;
    int **pixelValue;
  public:
    ImageType();
    ImageType(int, int, int);
    ImageType(ImageType&);
    ~ImageType();
    void GetImageInfo(int&, int&, int&);
    void SetImageInfo(int, int, int);
    void SetPixelVal(int, int, int);
    void GetPixelVal(int, int, int&);
};

#endif
