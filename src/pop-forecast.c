#include "grattan.h"

#define NYOB_1900_LEN 200



static void pop_next_yr(unsigned int nYOB_1900[NYOB_1900_LEN],
                        int yr,
                        double fertility,
                        int nom_migration) {
  unsigned int j_yr = yr - 1900u;
  if (j_yr >= NYOB_1900_LEN) {
    return;
  }
  unsigned int n_persons = nYOB_1900[j_yr];
  unsigned int n_female = n_persons >> 1;
  unsigned int n_births = n_female * fertility / 100e3;
  unsigned int n_deaths =
}

static void baseline_pop(unsigned int nYOB_1900[NYOB_1900_LEN],
                         const int * age, const int * year, const int * month, const int * pop,
                         int n) {
  int max_yr = year[0];
  int max_month = month[0];
  for (int i = 0; i < n; ++i) {
    if (max_yr < year[i]) {
      max_yr = year[i];
      max_month = month[i];
    } else if (max_yr == year[i]) {
      if (max_month < month[i]) {
        max_month = month[i];
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    int yr = year[i];
    if (yr != max_yr) {
      continue;
    }
    int mo = month[i];
    if (mo != max_month) {
      continue;
    }
    int yob = year[i] - age[i];
    nYOB_1900[yob - 1900] += pop[i];
  }
}

SEXP Cpop_forecast(SEXP FromToYr, SEXP Age, SEXP Year, SEXP Month, SEXP Sex, SEXP Pop,
                   SEXP Mortality0127, SEXP NOM0127, SEXP Fertility) {
  if (xlength(FromToYr) != 2) {
    error("Internal error(Cpop_forecast): length(FromToYr) != 2.");
  }
  if (!isInteger(FromToYr)) {
    error("Internal error(Cpop_forecast): was type '%s' but must be integer.", type2char(TYPEOF(FromToYr)));
  }

  unsigned int yrto = INTEGER(FromToYr)[1];
  unsigned int yrfrom = INTEGER(FromToYr)[0];
  unsigned int yrs_ahead = yrto - yrfrom;
  if (yrs_ahead > 110) {
    error("yrs_ahead = %u > 100", yrs_ahead);
  }
  int n = length(Age);
  isEquiInt(Age, Age, "Age");
  isEquiInt(Age, Year, "Year");
  isEquiInt(Age, Month, "Month");
  isEquiInt(Age, Sex, "Sex");
  isEquiInt(Age, Pop, "Pop");
  const int * age = INTEGER(Age);
  const int * year = INTEGER(Year);
  const int * month = INTEGER(Month);
  const int * pop = INTEGER(Pop);

  // N is the length of the output vector,
  // For every yrs_ahead, need 100 ages, 2 sexes,
  unsigned int N = 0;
  unsigned int nYOB_1900[NYOB_1900_LEN] = {0};
  baseline_pop(nYOB_1900, age, year, month, pop, n);


  SEXP ans = PROTECT(allocVector(INTSXP, N));

  UNPROTECT(1);
  return ans;
}
