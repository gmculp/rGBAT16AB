#include <Rcpp.h>
using namespace Rcpp;

#if defined(WIN32)
#define ROLE __declspec(dllimport) __stdcall
//#define ROLE __declspec(dllimport)
#elif defined(x64)
#define ROLE __declspec(dllimport) __stdcall
#elif defined(__linux__)
#define ROLE __stdcall
#endif

//header for function in NYCgeo.dll which lives in a folder called Bin
#include <NYCgeo.h>
//header for formatting returned fields
#include <pac.h>

// [[Rcpp::export]]
String GC_test() {
  //eventually, the argument will be a data frame of addresses

  //notes from DCP developer...
  //What this does is create a union for each work area 2 that combines the structured data with a character string.
  //Geosupport calls expect two character strings as parameters.
  //The pac.h include file is basically the structure of the work area 2’s.
  //This union just makes it easier to work with the structure when you are filling in data before the call and accessing the data after the call.
  //The character array makes it easy to pass in to Geosupport.

  union {
    C_WA1 wa1;
    char cwa1[1200];
  } uwa1;
  union {
    C_WA2_F1 wa2_f1;
    char cwa2f1[sizeof(C_WA2_F1)];
  } uwa2f1;
  union {
    C_WA2_F1EX wa2_f1ex;
    char cwa2f1ex[sizeof(C_WA2_F1EX)];
  } uwa2f1ex;
  union {
    C_WA2_FAP wa2_fap;
    char cwa2fap[sizeof(C_WA2_FAP)];
  } uwa2fap;
  union {
    C_WA2_FAPX wa2_fapx;
    char cwa2fapx[sizeof(C_WA2_FAPX)];
  } uwa2fapx;
  union {
    C_WA2_F1A wa2_f1a;
    char cwa2f1a[sizeof(C_WA2_F1A)];
  } uwa2f1a;
  union {
    C_WA2_F1AX wa2_f1ax;
    char cwa2f1ax[sizeof(C_WA2_F1AX)];
  } uwa2f1ax;
  union {
    C_WA2_F1B wa2_f1B;
    char cwa2f1b[sizeof(C_WA2_F1B)];
  } uwa2f1b;
  union {
    C_WA2_F2 wa2_f2;
    char cwa2f2[sizeof(C_WA2_F2)];
  } uwa2f2;
  union {
    C_WA2_F2W wa2_f2w;
    char cwa2f2w[sizeof(C_WA2_F2W)];
  } uwa2f2w;
  union {
    C_WA2_F3 wa2_f3;
    char cwa2f3[sizeof(C_WA2_F3)];
  } uwa2f3;
  union {
    C_WA2_F3C wa2_f3c;
    char cwa2f3c[sizeof(C_WA2_F3C)];
  } uwa2f3c;
  union {
    C_WA2_F3_AUXSEG wa2_f3as;
    char cwa2f3as[sizeof(C_WA2_F3_AUXSEG)];
  } uwa2f3as;
  union {
    C_WA2_F3C_AUXSEG wa2_f3cas;
    char cwa2f3cas[sizeof(C_WA2_F3C_AUXSEG)];
  } uwa2f3cas;
  union {
    C_WA2_F3X wa2_f3x;
    char cwa2f3x[sizeof(C_WA2_F3X)];
  } uwa2f3x;
  union {
    C_WA2_F3CX wa2_f3cx;
    char cwa2f3cx[sizeof(C_WA2_F3CX)];
  } uwa2f3cx;
  union {
    C_WA2_F3X_AUXSEG wa2_f3xas;
    char cwa2f3xas[sizeof(C_WA2_F3X_AUXSEG)];
  } uwa2f3xas;
  union {
    C_WA2_F3CX_AUXSEG wa2_f3cxas;
    char cwa2f3xcas[sizeof(C_WA2_F3CX_AUXSEG)];
  } uwa2f3cxas;
  union {
    C_WA2_F3S wa2_f3s;
    char cwa2f3s[sizeof(C_WA2_F3S)];
  } uwa2f3s;

  char cwa2fhr[5000];

  //using function 1E
  memset(uwa1.cwa1, ' ', 1200);
  memcpy(uwa1.wa1.input.func_code, "1E", 2);
  uwa1.wa1.input.platform_ind = 'C';

  //test inputs
  //char in_stcode_str[11] = "1061002010";

  //borough code
  char in_boro_str[2] = "1";
  uwa1.wa1.input.sti[0].boro = in_boro_str[0];

  //house number
  char in_housenum_str[16] = "22";
  memcpy(uwa1.wa1.input.hse_nbr_disp, in_housenum_str, strlen(in_housenum_str));

  //street name
  char in_stname_str[32] = "reade st";
  memcpy(uwa1.wa1.input.sti[0].Street_name, in_stname_str, 32);

  //extended work area
  uwa1.wa1.input.mode_switch = 'X';

  //geocoding function ... fails
  //NYCgeo(uwa1.cwa1, uwa2f1ex.cwa2f1ex);

  //check the return code

  //if successful, process data

  //if not successful, display error message

  return("testing");
}
