#include <fstream>
#include <iostream>

#include "image_ppm.hpp"
#include "seam_carver.hpp"

void PrintUsage(const std::string& command) {
  std::cout << "Usage: " << command << " INPUT_FILE [options] -o OUTPUT_FILE\n"
            << "Options:\n"
            << "\t-h times\tcarve times horizontal seams\n"
            << "\t-v times\tcarve times vertical seams\n";
}

int main(int argc, char* argv[]) {
  // int main() {
  std::string out_option = "-o";
  if (argc < 4 || out_option != argv[argc - 2]) {
    PrintUsage(argv[0]);
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile.good()) {
    std::cout << "file '" << argv[1] << "' not found" << std::endl;
    return 1;
  }

  // UNCOMMENT COMMENTED CODE BELOW AS YOU IMPLEMENT

  ImagePPM image;
  infile >> image;
  infile.close();

  SeamCarver carver(image);
  for (int i = 2; i < argc - 2; i += 2) {
    std::string times_string = argv[i + 1];
    int times = 0;
    try {
      times = std::stoi(times_string);
    } catch (const std::exception& e) {
      std::cout << "Malformed option" << std::endl;
      PrintUsage(argv[0]);
      return 1;
    }

    std::string option = argv[i];
    if (option == "-h") {
      for (int t = 0; t < times; t++) {
        carver.RemoveHorizontalSeam();
      }
    } else if (option == "-v") {
      for (int t = 0; t < times; t++) {
        carver.RemoveVerticalSeam();
      }
    } else {
      std::cout << argv[i] << " not an option" << std::endl;
      PrintUsage(argv[0]);
      return 1;
    }
  }

  std::ofstream outfile(argv[argc - 1]);
  outfile << carver.GetImage();
  outfile.close();
  // ***** TEST CASE USING image1.ppm FOR HORIZONTAL/VERTICAL SEAMS ******

  // ImagePPM image("./images/image1.ppm");
  // SeamCarver carver(image);
  // carver.GetHorizontalSeam();

  // ***** TEST CASE REMOVE HORIZONTAL SEAM *******

  /*static int seam[4] = {0, 2, 1, 0};
  static int copy[3][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
  int height_ = 3;
  int width_ = 4;
  int** cpy = new int*[height_ - 1];
  for (int row = 0; row < height_ - 1; row++) {
    cpy[row] = new int[width_];
  }

  for (int col = 0; col < width_; ++col) {
    int removeRow = seam[col];
    for (int row = 0, shift = 0; row < height_ - 1; ++row) {
      if (row == removeRow) {
        shift = 1;
      }
      cpy[row][col] = copy[row + shift][col];
    }
  }
  for (int row = 0; row < height_ - 1; row++) {
    for (int col = 0; col < width_; col++) {
      std::cout << cpy[row][col] << " ";
    }
    std::cout << std::endl;
  }*/

  // ****** TEST CASE REMOVE VERTICAL SEAM *******

  /*static int seam[4] = {0, 2, 1, 0};
  static int copy[3][4] = {{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}};
  int height_ = 3;
  int width_ = 4;
  int** cpy = new int*[height_];
  for (int row = 0; row < height_; row++) {
    cpy[row] = new int[width_];
  }
  for (int row = 0; row < height_; row++) {
    for (int col = 0, shift = 0; col < width_ - 1; col++) {
      if (seam[row] == col) {
        shift = 1;  // Start shifting elements left
      }
      cpy[row][col] = copy[row][col + shift];
    }
  }

  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_ - 1; col++) {
      std::cout << cpy[row][col] << " ";
    }
    std::cout << std::endl;
  }
  */
}
