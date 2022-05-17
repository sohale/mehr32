
/*

*/
#define ADDITEM(nm, name) \
  {                       \
    strcat(names, "  ");  \
    strcat(names, #nm);   \
    strcat(names, " = "); \
    strcat(names, #name); \
    strcat(names, "\n");  \
    if (maxi < nm)        \
      maxi = nm;          \
  }
