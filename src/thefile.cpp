
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

#include <NYCgeo.h>
#include <pac.h>
#include <cstring>


// [[Rcpp::export]]
DataFrame GBAT(DataFrame x, std::string id_col, std::string add_col, std::string boro_col) {

  CharacterVector id_vec = x[id_col];
  CharacterVector add_vec = x[add_col];
  CharacterVector boro_vec = x[boro_col];

  // create new variables for export
  std::vector<std::string> all_vars1 (id_vec.size());
  std::vector<std::string> all_vars2 (id_vec.size());

  //std::vector<std::string> all_vars1AP (id_vec.size());
  std::vector<std::string> all_vars2AP (id_vec.size());


  for (int i = 0; i < id_vec.size() ; i++) {

    //input for function 1E
    union {
      C_WA1 wa1;
      char cwa1[1200];
    } uwa1;

    //input for function APX
    union {
      C_WA1 wa1_ap;
      char cwa1_ap[1200];
    } uwa1_ap;

    //output
    union {
      C_WA2_F1EX wa2_f1ex;
      char cwa2f1ex[sizeof(C_WA2_F1EX)];
    } uwa2f1ex;

    union {
      C_WA2_FAPX wa2_fapx;
      char cwa2fapx[sizeof(C_WA2_FAPX)];
    } uwa2fapx;

    //char cwa2fhr[5000];

    //using function 1E
    memset(uwa1.cwa1, ' ', 1200);
    memcpy(uwa1.wa1.input.func_code, "1E", 2);
    uwa1.wa1.input.platform_ind = 'C';

    //using function AP
    memset(uwa1_ap.cwa1_ap, ' ', 1200);
    memcpy(uwa1_ap.wa1_ap.input.func_code, "AP", 2);
    uwa1_ap.wa1_ap.input.platform_ind = 'C';

    //borough code
    std::string in_boro_str = Rcpp::as<std::string>(boro_vec[i]);
    char *in_boro = new char[in_boro_str.length() + 1];
    std::strcpy(in_boro, in_boro_str.c_str());
    uwa1.wa1.input.sti[0].boro = in_boro[0];
    uwa1_ap.wa1_ap.input.sti[0].boro = in_boro[0];
    delete[] in_boro;

    //freeform address
    std::string in_stname_str = Rcpp::as<std::string>(add_vec[i]);
    char *in_stname = new char[in_stname_str.length() + 1];
    std::strcpy(in_stname, in_stname_str.c_str());
    memcpy(uwa1.wa1.input.sti[0].Street_name, in_stname, strlen(in_stname));
    memcpy(uwa1_ap.wa1_ap.input.sti[0].Street_name, in_stname, strlen(in_stname));
    delete[] in_stname;

    //geocoding function
    //extended work area
    uwa1.wa1.input.mode_switch = 'X';
    uwa1_ap.wa1_ap.input.mode_switch = 'X';

    NYCgeo(uwa1.cwa1, uwa2f1ex.cwa2f1ex);
    NYCgeo(uwa1_ap.cwa1_ap, uwa2fapx.cwa2fapx);

    //if successful, process data
    if ((memcmp(uwa1.wa1.output.ret_code, "00", 2) == 0)  || (memcmp(uwa1.wa1.output.ret_code, "01", 2) == 0)) {

      //if AP viable
      if ((memcmp(uwa1_ap.wa1_ap.output.ret_code, "00", 2) == 0)  ||  (memcmp(uwa1_ap.wa1_ap.output.ret_code, "01", 2) == 0)) {

        //return variables for function APE
        std::string all_wa2_ap_f1ex_var;
        all_wa2_ap_f1ex_var = uwa2fapx.cwa2fapx;
        all_vars2AP[i] = all_wa2_ap_f1ex_var;

      }

      //return variables for function 1E
      std::string all_wa2_f1ex_var;
      all_wa2_f1ex_var = uwa2f1ex.cwa2f1ex;
      all_vars2[i] = all_wa2_f1ex_var;

  }
    //output variables for function APE
    //std::string all_wa1_ap_var;
    //all_wa1_ap_var = uwa1_ap.cwa1_ap;
    //all_vars1AP[i] = all_wa1_ap_var;

    //output variables for function 1E
    std::string all_wa1_var;
    all_wa1_var = uwa1.cwa1;
    all_vars1[i] = all_wa1_var;


  }
  //RCPP limits maxinum number of returned fields to n=10
  return Rcpp::DataFrame::create( Named(id_col)= id_vec
                                  , Named(add_col)= add_vec
                                  , Named(boro_col) = boro_vec
                                  , Named("out_all1") = all_vars1
                                  , Named("out_all2") = all_vars2
                                  //, Named("out_all1AP") = all_vars1AP
                                  , Named("out_all2AP") = all_vars2AP
                                  );
}
