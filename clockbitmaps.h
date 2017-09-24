#ifndef __CLOCKBITMAP_H__
#define __CLOCKBITMAP_H__

static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 
  },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 
  },
  frown_bmp[] =
  { 
    B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 
  };

static const uint8_t digitoneshifted[] = {
  B00000010,
  B00000110,
  B00000010,
  B00000010,
  B00000010,
  B00000010,
  B00000010,
  B00000111
};

static const uint8_t digittwoshifted[] = {
  B00000000,
  B00011100,
  B00100010,
  B00000010,
  B00011100,
  B00100000,
  B00100000,
  B10111110
};

static const uint8_t digiteleven[] = {
  B00000000,
  B01000100,
  B11001100,
  B01000100,
  B01000100,
  B01000100,
  B01000100,
  B11101110
};

static const uint8_t digitonehigh[] = {
  B00000000,
  B00000010,
  B00000110,
  B00000010,
  B00000010,
  B00000010,
  B00000010,
  B00000111
};

static const uint8_t digitzerocolon[] = {
  B00011100,
  B00100010,
  B00100010,
  B10100010,
  B00100010,
  B10100010,
  B00100010,
  B00011100
};

static const uint8_t digitonecolon[] = {
  B00000100,
  B00001100,
  B00000100,
  B10000100,
  B00000100,
  B10000100,
  B00000100,
  B00001110
};

static const uint8_t digittwocolon[] = {
  B00011100,
  B00100010,
  B00000010,
  B10011100,
  B00100000,
  B10100000,
  B00100000,
  B00011110
};

static const uint8_t digitthreecolon[] = {
  B00011100,
  B00100010,
  B00000010,
  B10001100,
  B00000010,
  B10000010,
  B00100010,
  B00011100
};

static const uint8_t digitfourcolon[] = {
  B00000100,
  B00001100,
  B00010100,
  B10100100,
  B00100100,
  B10111110,
  B00000100,
  B00000100,
};

static const uint8_t digitfivecolon[] = {
  B00111110,
  B00100000,
  B00100000,
  B10111100,
  B00100010,
  B10000010,
  B00100010,
  B00011100
};

static const uint8_t digitsixcolon[] = {
  B00011100,
  B00100010,
  B00100000,
  B10011100,
  B00100010,
  B10100010,
  B00100010,
  B00011100
};
static const uint8_t digitsevencolon[] = {
  B00111110,
  B00000010,
  B00000010,
  B10000010,
  B00100100,
  B10001000,
  B00010000,
  B00100000
};

static const uint8_t digiteightcolon[] = {
  B00011100,
  B00100010,
  B00100010,
  B10011100,
  B00100010,
  B10100010,
  B00100010,
  B00011100,
};

static const uint8_t digitninecolon[] = {
  B00011100,
  B00100010,
  B00100010,
  B10011100,
  B00000010,
  B10000010,
  B00100010,
  B00011100,
};

static const uint8_t digitzero[] = {
  B00011100,
  B00100010,
  B00100010,
  B00100010,
  B00100010,
  B00100010,
  B00100010,
  B00011100,
};

static const uint8_t digitone[] = {
  B00000100,
  B00001100,
  B00000100,
  B00000100,
  B00000100,
  B00000100,
  B00000100,
  B00001110,
};

static const uint8_t digittwo[] = {
  B00011100,
  B00100010,
  B00000010,
  B00011100,
  B00100000,
  B00100000,
  B00100000,
  B00011110,
};

static const uint8_t digitthree[] = {
  B00011100,
  B00100010,
  B00000010,
  B00001100,
  B00000010,
  B00000010,
  B00100010,
  B00011100,
};

static const uint8_t digitfour[] = {
  B00000100,
  B00001100,
  B00010100,
  B00100100,
  B00100100,
  B00111110,
  B00000100,
  B00000100,
};

static const uint8_t digitfive[] = {
  B00111110,
  B00100000,
  B00100000,
  B00111100,
  B00100010,
  B00000010,
  B00100010,
  B00011100,
};

static const uint8_t digitsix[] = {
  B00011100,
  B00100010,
  B00100000,
  B00011100,
  B00100010,
  B00100010,
  B00100010,
  B00011100
};
static const uint8_t digitseven[] = {
  B00111110,
  B00000010,
  B00000010,
  B00000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
};

static const uint8_t digiteight[] = {
  B00011100,
  B00100010,
  B00100010,
  B00011100,
  B00100010,
  B00100010,
  B00100010,
  B00011100
};

static const uint8_t digitnine[] = {
  B00011100,
  B00100010,
  B00100010,
  B00011100,
  B00000010,
  B00000010,
  B00100010,
  B00011100
};

static const uint8_t digitzerolow[] = {
  B00111000,
  B01000100,
  B01000100,
  B01000100,
  B01000100,
  B01000100,
  B01000100,
  B00111000,
};

static const uint8_t digitonelow[] = {
  B00010000,
  B00110000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00111000
};

static const uint8_t digittwolow[] = {
  B00111000,
  B01000100,
  B00000100,
  B00111000,
  B01000000,
  B01000000,
  B01000000,
  B00111100,
};

static const uint8_t digitthreelow[] = {
  B00111000,
  B01000100,
  B00000100,
  B00011000,
  B00000100,
  B00000100,
  B01000100,
  B00111000,
};

static const uint8_t digitfourlow[] = {
  B00001000,
  B00011000,
  B00101000,
  B01001000,
  B01001000,
  B01111100,
  B00001000,
  B00001000,
};

static const uint8_t digitfivelow[] = {
  B01111100,
  B01000000,
  B01000000,
  B01111000,
  B01000100,
  B00000100,
  B01000100,
  B00111000,
};

static const uint8_t digitsixlow[] = {
  B00111000,
  B01000100,
  B01000000,
  B00111000,
  B01000100,
  B01000100,
  B01000100,
  B00111000
};
static const uint8_t digitsevenlow[] = {
  B01111100,
  B00000100,
  B00000100,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
};

static const uint8_t digiteightlow[] = {
  B00111000,
  B01000100,
  B01000100,
  B00111000,
  B01000100,
  B01000100,
  B01000100,
  B00111000
};

static const uint8_t digitninelow[] = {
  B00111000,
  B01000100,
  B01000100,
  B00111000,
  B00000100,
  B00000100,
  B01000100,
  B00111000
};

#endif

