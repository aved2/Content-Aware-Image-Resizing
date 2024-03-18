#include "seam_carver.hpp"

#include "image_ppm.hpp"

// implement the rest of SeamCarver's functions here
const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }

int SeamCarver::GetEnergy(int row, int col) const {
  Pixel row_u(0, 0, 0), row_d(0, 0, 0), col_l(0, 0, 0), col_r(0, 0, 0);
  if (row == 0) {
    row_u = image_.GetPixel(height_ - 1, col);
    row_d = image_.GetPixel(row + 1, col);
  } else if (row == height_ - 1) {
    row_u = image_.GetPixel(row - 1, col);
    row_d = image_.GetPixel(0, col);
  } else {
    row_u = image_.GetPixel(row - 1, col);
    row_d = image_.GetPixel(row + 1, col);
  }

  if (col == 0) {
    col_l = image_.GetPixel(row, width_ - 1);
    col_r = image_.GetPixel(row, col + 1);
  } else if (col == width_ - 1) {
    col_l = image_.GetPixel(row, col - 1);
    col_r = image_.GetPixel(row, 0);
  } else {
    col_l = image_.GetPixel(row, col - 1);
    col_r = image_.GetPixel(row, col + 1);
  }
  int a = (col_l.GetRed() - col_r.GetRed()) * (col_l.GetRed() - col_r.GetRed());
  int b = (col_l.GetGreen() - col_r.GetGreen()) *
          (col_l.GetGreen() - col_r.GetGreen());
  int c =
      (col_l.GetBlue() - col_r.GetBlue()) * (col_l.GetBlue() - col_r.GetBlue());
  int d = (row_u.GetRed() - row_d.GetRed()) * (row_u.GetRed() - row_d.GetRed());
  int e = (row_u.GetGreen() - row_d.GetGreen()) *
          (row_u.GetGreen() - row_d.GetGreen());
  int f =
      (row_u.GetBlue() - row_d.GetBlue()) * (row_u.GetBlue() - row_d.GetBlue());

  return a + b + c + d + e + f;
}

int** SeamCarver::CreateEnergyArr() const {
  ImagePPM image = GetImage();
  int** values = new int*[height_];
  for (int row = 0; row < height_; row++) {
    values[row] = new int[width_];
  }
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_; col++) {
      values[row][col] = GetEnergy(row, col);
    }
  }
  return values;
}

int** SeamCarver::VertCarvingEnergies() const {
  int** values = CreateEnergyArr();
  int best = 0;
  for (int row = height_ - 2; row >= 0; row--) {
    for (int col = 0; col < width_; col++) {
      if (col == 0) {
        best = std::min(values[row + 1][col], values[row + 1][col + 1]);
      } else if (col == width_ - 1) {
        best = std::min(values[row + 1][col - 1], values[row + 1][col]);
      } else {
        best =
            std::min(std::min(values[row + 1][col - 1], values[row + 1][col]),
                     values[row + 1][col + 1]);
        ;
      }
      values[row][col] += best;
    }
  }
  return values;
}

int** SeamCarver::HorizCarvingEnergies() const {
  int** values = CreateEnergyArr();
  int best = 0;
  for (int col = width_ - 2; col >= 0; col--) {
    for (int row = 0; row < height_; row++) {
      if (row == 0) {
        best = std::min(values[row][col + 1], values[row + 1][col + 1]);
      } else if (row == height_ - 1) {
        best = std::min(values[row - 1][col + 1], values[row][col + 1]);
      } else {
        best =
            std::min(std::min(values[row - 1][col + 1], values[row][col + 1]),
                     values[row + 1][col + 1]);
      }
      values[row][col] += best;
    }
  }
  return values;
}

int* SeamCarver::FindVSeam(int** values) const {
  int* seam = new int[height_];
  int min_col = 0;
  for (int col = 0; col < width_; col++) {
    if (values[0][col] < values[0][min_col]) {
      min_col = col;
    }
  }
  seam[0] = min_col;
  for (int row = 1; row < height_; row++) {
    if (min_col == 0) {
      if (values[row][min_col] <= values[row][min_col + 1]) {
        seam[row] = min_col;
      } else {
        seam[row] = min_col + 1;
        min_col++;
      }
    } else if (min_col == width_ - 1) {
      if (values[row][min_col - 1] < values[row][min_col]) {
        seam[row] = min_col - 1;
        min_col--;
      } else {
        seam[row] = min_col;
      }
    } else {
      if (values[row][min_col] <= values[row][min_col - 1] &&
          values[row][min_col] <= values[row][min_col + 1]) {
        seam[row] = min_col;
      } else if (values[row][min_col - 1] <= values[row][min_col + 1] &&
                 values[row][min_col] > values[row][min_col - 1]) {
        seam[row] = min_col - 1;
        min_col--;
      } else {
        seam[row] = min_col + 1;
        min_col++;
      }
    }
  }
  return seam;
}

int* SeamCarver::FindHSeam(int** values) const {
  int* seam = new int[width_];
  int min_row = 0;
  for (int row = 0; row < height_; row++) {
    if (values[row][0] < values[min_row][0]) {
      min_row = row;
    }
  }
  seam[0] = min_row;
  for (int col = 1; col < width_; col++) {
    if (min_row == 0) {
      if (values[min_row][col] <= values[min_row + 1][col]) {
        seam[col] = min_row;
      } else {
        seam[col] = min_row + 1;
        min_row++;
      }
    } else if (min_row == height_ - 1) {
      if (values[min_row - 1][col] < values[min_row][col]) {
        seam[col] = min_row - 1;
        min_row--;
      } else {
        seam[col] = min_row;
      }
    } else {
      if (values[min_row][col] <= values[min_row - 1][col] &&
          values[min_row][col] <= values[min_row + 1][col]) {
        seam[col] = min_row;
      } else if (values[min_row - 1][col] <= values[min_row + 1][col] &&
                 values[min_row][col] > values[min_row - 1][col]) {
        seam[col] = min_row - 1;
        min_row--;
      } else {
        seam[col] = min_row + 1;
        min_row++;
      }
    }
  }
  return seam;
  delete[] seam;
}

int* SeamCarver::GetVerticalSeam() const {
  int** values = VertCarvingEnergies();
  int* seam = FindVSeam(values);
  for (int i = 0; i < height_; i++) {
    delete[] values[i];
  }
  delete[] values;
  values = nullptr;
  return seam;
}

int* SeamCarver::GetHorizontalSeam() const {
  int** values = HorizCarvingEnergies();
  int* seam = FindHSeam(values);
  for (int i = 0; i < height_; i++) {
    delete[] values[i];
  }
  delete[] values;
  return seam;
}

void SeamCarver::RemoveVerticalSeam() {
  int* seam = GetVerticalSeam();
  image_.VSetImage(seam);
  width_--;
  delete[] seam;
  seam = nullptr;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* seam = GetHorizontalSeam();
  image_.HSetImage(seam);
  height_--;
  delete[] seam;
  seam = nullptr;
}

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}