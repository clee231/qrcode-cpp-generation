#include <iostream>
#include <string>
#include "qrencode.h"

// QRCode Generation Test
// Author: clee231

using namespace std;

int margin = 2;

// This function just creates lines of blank space.
string writeASCII_margin(int realwidth, int invert)
{
  int y, h;

  h = margin; // # of horizontal lines

  string output;
  string oneline;
  for(y = 0; y < realwidth; y++) {
    oneline += (invert?"\u2588":" ");
  }
  oneline += "\n";
  for(y = 0; y < h; y++) {
    output += oneline;
  }
  return output;
}

// Draw QR Code. Also pads with margin.
string makeASCII(const QRcode *qrcode, int invert) {
  string image;
  unsigned char *row;
  string black = "\u2588"; // This is a full black box in Unicode
  string white = " ";
  int y, x; // counter variables

  if (invert) {
    black = " ";
    white = "\u2588";
  }

  int realwidth = (qrcode->width + margin * 2) * 2;
  string buffer;

  /* top margin*/
  image += writeASCII_margin(realwidth, invert);

  /* data */
  for(y = 0; y < qrcode->width; y++) {
    row = qrcode->data+(y*qrcode->width);

    for (x = 0; x < margin*2; x++) {
      image += white;
    }

    for(x = 0; x < qrcode->width; x++) {
      if(row[x]&0x1) {
        image += black;
        image += black;
      } else {
        image += white;
        image += white;
      }
    }

    for (x = 0; x < margin*2; x++) {
      image += white;
    }
    image += "\n";
  }

  /* bottom margin*/
  image += writeASCII_margin(realwidth, invert);

  return image;

}

int main() {
  cout << "--QR Code Generation Test--" << endl;

  QRcode *qrcode;

  const char* stringData = "Hello World";

  qrcode = QRcode_encodeString(stringData, 1, QR_ECLEVEL_M, QR_MODE_8, 0);
  qrcode->width = 21;
  cout << "QR Size:" << qrcode->width << endl;

  if (qrcode == NULL) {
    cout << "qrcode generation failed" << endl;
    if(errno == ERANGE) {
      fprintf(stderr, "Failed to encode the input data: Input data too large\n");
    } else {
      perror("Failed to encode the input data");
    }

  } else {
    string output = makeASCII(qrcode, 1);
    cout << output << endl;
  }

  return 0;
}
