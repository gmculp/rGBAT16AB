
#include <Rcpp.h>
#include <boost/algorithm/string/trim.hpp>

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
DataFrame GC_test(DataFrame x, std::string id_col, std::string add_col, std::string boro_col) {

  //array of county codes for calculating census geographies
  String cty_code[] = {"na", "061", "005", "047", "081", "085"};

  CharacterVector id_vec = x[id_col];
  CharacterVector add_vec = x[add_col];
  CharacterVector boro_vec = x[boro_col];

  // create new variables for export
  std::vector<std::string> hse_nbr (id_vec.size());
  std::vector<std::string> BOE_St (id_vec.size());
  std::vector<std::string> USPS_city (id_vec.size());
  std::vector<std::string> z_c (id_vec.size());

  std::vector<std::string> a_p (id_vec.size());
  std::vector<std::string> out_bin (id_vec.size());
  std::vector<std::string> out_bbl (id_vec.size());
  std::vector<std::string> out_CD (id_vec.size());

  std::vector<std::string> out_lat (id_vec.size());
  std::vector<std::string> out_lng (id_vec.size());
  std::vector<std::string> out_x (id_vec.size());
  std::vector<std::string> out_y (id_vec.size());

  std::vector<std::string> ct_2000 (id_vec.size());
  std::vector<std::string> cb_2000 (id_vec.size());
  std::vector<std::string> ct_2010 (id_vec.size());
  std::vector<std::string> cb_2010 (id_vec.size());
  std::vector<std::string> out_NTA (id_vec.size());

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

    char cwa2fhr[5000];

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

    //check the return code
    //printf("\n\nFunction 1E GRC = %.2s\n"  "Error Message = %.80s\n", uwa1.wa1.output.ret_code, uwa1.wa1.output.msg);

    //if successful, process data
    if ((memcmp(uwa1.wa1.output.ret_code, "00", 2) == 0)  || (memcmp(uwa1.wa1.output.ret_code, "01", 2) == 0)) {

      //AP viable
      if ((memcmp(uwa1_ap.wa1_ap.output.ret_code, "00", 2) == 0)  ||  (memcmp(uwa1_ap.wa1_ap.output.ret_code, "01", 2) == 0)) {

        //printf("AP = %.9s\n", uwa2fapx.wa2_fapx.ap_id);
        std::string a_p_var;
        a_p_var = uwa2fapx.wa2_fapx.ap_id;
        a_p_var = a_p_var.substr (0,9);
        a_p[i] = a_p_var;

        //printf("BIN1 = %.7s\n", uwa2fapx.wa2_fapx.bld_id);
      }

      //printf("latitude = %.9s\n", uwa2f1ex.wa2_f1ex.latitude);
      std::string out_lat_var;
      out_lat_var = uwa2f1ex.wa2_f1ex.latitude;
      out_lat_var = out_lat_var.substr (0,9);
      out_lat[i] = out_lat_var;

      //printf("longitude = %.11s\n", uwa2f1ex.wa2_f1ex.longitude);
      std::string out_lng_var;
      out_lng_var = uwa2f1ex.wa2_f1ex.longitude;
      out_lng_var = out_lng_var.substr (0,10);
      out_lng[i] = out_lng_var;

      //printf("CITY = %.25s\n", uwa2f1ex.wa2_f1ex.USPS_city_name);
      std::string USPS_city_var;
      USPS_city_var = uwa2f1ex.wa2_f1ex.USPS_city_name;
      USPS_city_var = USPS_city_var.substr (0,25);
      boost::algorithm::trim(USPS_city_var);
      USPS_city[i] = USPS_city_var;

      //printf("BBL = %.10s\n", uwa1.wa1.output.bblo);
      std::string out_bbl_var;
      out_bbl_var = uwa1.wa1.output.bblo.bbl;
      out_bbl_var = out_bbl_var.substr (0,10);
      out_bbl[i] = out_bbl_var;

      //printf("BIN2 = %.7s\n", uwa1.wa1.output.bin);
      std::string out_bin_var;
      out_bin_var = uwa1.wa1.output.bin;
      out_bin_var = out_bin_var.substr (0,7);
      out_bin[i] = out_bin_var;

      //printf("HOUSE NUMBER = %.16s\n", uwa1.wa1.output.hse_nbr_disp);
      std::string hse_nbr_var;
      hse_nbr_var = uwa1.wa1.output.hse_nbr_disp;
      hse_nbr_var = hse_nbr_var.substr (0,16);
      boost::algorithm::trim(hse_nbr_var);
      hse_nbr[i] = hse_nbr_var;

      //printf("STREET = %.32s\n", uwa2f1ex.wa2_f1ex.BOE_st_name);
      std::string BOE_St_var;
      BOE_St_var = uwa2f1ex.wa2_f1ex.BOE_st_name;
      BOE_St_var = BOE_St_var.substr (0,32);
      boost::algorithm::trim(BOE_St_var);
      BOE_St[i] = BOE_St_var;

      //printf("zip code = %.5s\n", uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.zip_code);
      std::string z_c_var;
      z_c_var = uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.zip_code;
      z_c_var = z_c_var.substr (0,5);
      z_c[i] = z_c_var;

      //printf("NTA = %.4s\n", uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.nta);
      std::string out_NTA_var;
      out_NTA_var = uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.nta;
      out_NTA_var = out_NTA_var.substr (0,4);
      out_NTA[i] = out_NTA_var;

      //printf("CD = %.3s\n", uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.com_dist);
      std::string out_CD_var;
      out_CD_var = uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.com_dist;
      out_CD_var = out_CD_var.substr (0,3);
      out_CD[i] = out_CD_var;

      //printf("x_coor = %.7s\n", uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.coord[0]);
      std::string out_x_var;
      out_x_var = uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.coord[0];
      out_x_var = out_x_var.substr (0,7);
      out_x[i] = out_x_var;

      //printf("y_coor = %.7s\n", uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.coord[1]);
      std::string out_y_var;
      out_y_var = uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.coord[1];
      out_y_var = out_y_var.substr (0,7);
      out_y[i] = out_y_var;

      //get census county id
      int boro_id = uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.boro_of_cen_tract - '0';

      //formatting 2000 census tract
      std::string ct2000;
      ct2000 = uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.cen_tract_2000;
      ct2000 = ct2000.substr (0,6);
      replace(ct2000.begin(), ct2000.end(), ' ', '0');
      ct_2000[i] = std::string("36") + std::string(cty_code[boro_id]) + ct2000;

      //formatting 2000 census block
      std::string cb2000;
      cb2000 = uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.cen_blk_2000;
      cb2000 = cb2000.substr (0,4);
      replace(cb2000.begin(), cb2000.end(), ' ', '0');
      cb_2000[i] = std::string("36") + std::string(cty_code[boro_id]) + ct2000 + cb2000;

      //formatting 2010 census tract
      std::string ct2010;
      ct2010 = uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.cen_tract_10;
      ct2010 = ct2010.substr (0,6);
      replace(ct2010.begin(), ct2010.end(), ' ', '0');
      ct_2010[i] = std::string("36") + std::string(cty_code[boro_id]) + ct2010;

      //formatting 2010 census block
      std::string cb2010;
      cb2010 = uwa2f1ex.wa2_f1ex.cwa2f1v.c_wa2_f1.cen_blk_10;
      cb2010 = cb2010.substr (0,4);
      replace(cb2010.begin(), cb2010.end(), ' ', '0');
      cb_2010[i] = std::string("36") + std::string(cty_code[boro_id]) + ct2010 + cb2010;

  }

  //if not successful, display error message
  //  else{

  //  }
  }
  //return(x);
  return Rcpp::DataFrame::create( Named(id_col)= id_vec, Named(add_col)= add_vec, Named(boro_col) = boro_vec, Named("out_house_num") = hse_nbr, Named("out_street") = BOE_St, Named("out_city") = USPS_city,
                                  Named("out_zip_code") = z_c, Named("out_AP_ID") = a_p, Named("out_BIN") = out_bin, Named("out_BBL") = out_bbl, Named("out_CD") = out_CD,
                                  Named("out_x") = out_x, Named("out_y") = out_y, Named("out_lng") = out_lng, Named("out_lat") = out_lat,
                                  Named("out_cb_2000") = cb_2000, Named("out_ct_2000") = ct_2000, Named("out_cb_2010") = cb_2010, Named("out_ct_2010") = ct_2010, Named("out_NTA") = out_NTA);
}
