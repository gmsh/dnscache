#include "dc_bitmap.h"

int main()
{
  _dc_bitmap bm1;
  int8 a =  first_of_1(bm1);
  bm1 = bm_set(1, bm1);
  bm1 = bm_set(5, bm1);
  bm1 = bm_set(3, bm1);
  uint8 e = bm_get(1, bm1);
  uint8 f = bm_get(3, bm1);
  uint8 g = bm_get(5, bm1);
  int8 b = num_of_1(bm1);
  int8 c = next_of_1(1, bm1);
  int8 d = next_of_1(5, bm1);
}
