
#include <Rcpp.h>

using namespace Rcpp;

#if defined(WIN32)
//#define ROLE __declspec(dllimport)
#define ROLE __declspec(dllimport) __stdcall
#elif defined(x64)
#define ROLE __declspec(dllimport) __stdcall
#elif defined(__linux__)
#define ROLE __stdcall
#endif

#include <NYCgeo.h>
#include <pac.h>
#include <cstring>


// [[Rcpp::export]]
DataFrame GBAT(DataFrame x, std::string id_col, std::string add_col, std::string third_col, std::string third_col_type = "boro_code") {

  CharacterVector id_vec = x[id_col];
  CharacterVector add_vec = x[add_col];
  //CharacterVector boro_vec = x[boro_col];
  CharacterVector third_vec = x[third_col];

  // create new variables for export
  std::vector<std::string> all_vars1E (id_vec.size());
  std::vector<std::string> all_vars1A (id_vec.size());
  std::vector<std::string> all_varsAP (id_vec.size());


  for (int i = 0; i < id_vec.size() ; i++) {

    //input for function 1A
    union {
      C_WA1 wa1_1a;
      char cwa1_1a[1200];
    } uwa1_1a;

    //input for function 1E
    union {
      C_WA1 wa1_1e;
      char cwa1_1e[1200];
    } uwa1_1e;

    //input for function AP
    union {
      C_WA1 wa1_ap;
      char cwa1_ap[1200];
    } uwa1_ap;

    //output for function 1A
    union {
      C_WA2_F1AX wa2_f1ax;
      char cwa2f1ax[sizeof(C_WA2_F1AX)];
    } uwa2f1ax;

    //output for function 1E
    union {
      C_WA2_F1EX wa2_f1ex;
      char cwa2f1ex[sizeof(C_WA2_F1EX)];
    } uwa2f1ex;

    //output for function AP
    union {
      C_WA2_FAPX wa2_fapx;
      char cwa2fapx[sizeof(C_WA2_FAPX)];
    } uwa2fapx;


    //using function 1A
    memset(uwa1_1a.cwa1_1a, ' ', 1200);
    memcpy(uwa1_1a.wa1_1a.input.func_code, "1A", 2);
    uwa1_1a.wa1_1a.input.platform_ind = 'C';

    //using function 1E
    memset(uwa1_1e.cwa1_1e, ' ', 1200);
    memcpy(uwa1_1e.wa1_1e.input.func_code, "1E", 2);
    uwa1_1e.wa1_1e.input.platform_ind = 'C';

    //using function AP
    memset(uwa1_ap.cwa1_ap, ' ', 1200);
    memcpy(uwa1_ap.wa1_ap.input.func_code, "AP", 2);
    uwa1_ap.wa1_ap.input.platform_ind = 'C';

    std::string in_third_str = Rcpp::as<std::string>(third_vec[i]);
    char *in_third = new char[in_third_str.length() + 1];
    std::strcpy(in_third, in_third_str.c_str());

    //check third column and assign accordingly
    if (third_col_type=="zip_code") {
      //zip code
      memcpy(uwa1_1a.wa1_1a.input.zipin, in_third, strlen(in_third));
      memcpy(uwa1_1e.wa1_1e.input.zipin, in_third, strlen(in_third));
      memcpy(uwa1_ap.wa1_ap.input.zipin, in_third, strlen(in_third));
    }
    else {
      //borough code
      uwa1_1a.wa1_1a.input.sti[0].boro = in_third[0];
      uwa1_1e.wa1_1e.input.sti[0].boro = in_third[0];
      uwa1_ap.wa1_ap.input.sti[0].boro = in_third[0];
    }
    delete[] in_third;


    //freeform address
    std::string in_stname_str = Rcpp::as<std::string>(add_vec[i]);
    char *in_stname = new char[in_stname_str.length() + 1];
    std::strcpy(in_stname, in_stname_str.c_str());
    memcpy(uwa1_1a.wa1_1a.input.sti[0].Street_name, in_stname, strlen(in_stname));
    memcpy(uwa1_1e.wa1_1e.input.sti[0].Street_name, in_stname, strlen(in_stname));
    memcpy(uwa1_ap.wa1_ap.input.sti[0].Street_name, in_stname, strlen(in_stname));
    delete[] in_stname;

    //geocoding functions with extended work area

    //call extended function 1A
    uwa1_1a.wa1_1a.input.mode_switch = 'X';
    NYCgeo(uwa1_1a.cwa1_1a, uwa2f1ax.cwa2f1ax);

    //call extended function 1E
    uwa1_1e.wa1_1e.input.mode_switch = 'X';
    NYCgeo(uwa1_1e.cwa1_1e, uwa2f1ex.cwa2f1ex);

    //call extended function AP
    uwa1_ap.wa1_ap.input.mode_switch = 'X';
    NYCgeo(uwa1_ap.cwa1_ap, uwa2fapx.cwa2fapx);

    //all variables for function 1A
    std::string all_wa1_1a_var;
    //all_wa1_1a_var = uwa1_1a.cwa1_1a;
    all_wa1_1a_var = uwa2f1ax.cwa2f1ax;
    all_vars1A[i] = all_wa1_1a_var;

    //all variables for function 1E
    std::string all_wa1_1e_var;
    all_wa1_1e_var = uwa1_1e.cwa1_1e;
    all_vars1E[i] = all_wa1_1e_var;

    //all variables for function AP
    std::string all_wa1_ap_var;
    //all_wa1_ap_var = uwa1_ap.cwa1_ap;
    all_wa1_ap_var = uwa2fapx.cwa2fapx;
    all_varsAP[i] = all_wa1_ap_var;

  }

  //RCPP limits maxinum number of returned fields to n=10
  //export spacedeliminated results for later parsing within R
  return Rcpp::DataFrame::create( Named(id_col)= id_vec
                                  , Named(add_col)= add_vec
                                  , Named(third_col) = third_vec
                                  , Named("F1A_output") = all_vars1A
                                  , Named("F1E_output") = all_vars1E
                                  , Named("FAP_output") = all_varsAP
                                  );
}
