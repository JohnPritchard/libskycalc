/* SKY CALCULATOR PROGRAM

	John Thorstensen, Dartmouth College.

   This program computes many quantities frequently needed by
   the observational astronomer.  It is written as a completely
   self-contained program in standard c, so it should be
   very transportable; the only issue I know of that really affects
   portability is the adequacy of the double-precision floating
   point accuracy on the machine.  Experience shows that c compilers
   on various systems have idiosyncracies, though, so be sure
   to check carefully.

   This is intended as an observatory utility program; I assume the
   user is familiar with astronomical coordinates and nomenclature.
   While the code should be very transportable, I also
   assume it will be installed by a conscientious person who
   will run critical tests before it is released at a new site.
   Experience shows that some c compilers generate unforseen errors
   when the code is ported, so the output should be checked meticulously
   against data from other sites.

   The first part (the almanac) lists the phenomena for a single night (sunset,
   twilight, moonrise, mooset, etc.) in civil clock time.
   The rise-set and twilight times given are good
   to a minute or two; the moon ephemeris used for rise/set is good to
   +- 0.3 degrees or so; it's from the Astronomical Almanac's
   low precision formulae, (with topocentric corrections included).
   The resulting moon rise/set times are generally good to better than
   two minutes.  The moon coordinates for midnight and in the 'calculator
   mode' are from a more accurate routine and are generally better than
   1 arcmin.  The elevation of an observatory above its effective
   horizon can be specified; if it is non-zero, rise/set times are
   corrected approximately for depression of the horizon.

   After displaying the phenomena for one night, the program goes
   into a 'calculator mode', in which one can -

	- enter RA, dec, proper motion, epoch, date, time,
	     new site parameters, etc. ...

	- compute and display circumstances of observation for the
	   current parameters, including precessed coordinates,
	   airmass, interference from moon or twilight, parallactic
	   angle, etc; the program also gives calendar date in
	   both UT and local, Julian date, and barycentric corrections.

	- compute and display a table of airmasses (etc) at
	   hourly intervals through the night.  This is very useful
	   at the telescope.  Also, if one has a modest number of
	   objects, it may be convenient (using system utilities)
	   to redirect the output and print a hard copy of these
	   tables for ready reference.

	- compute and display galactic and ecliptic coordinates.

	- compute and display rough (of order 0.1 degree, but often
	  much better) positions of the major planets.

	- display the almanac for the current night.

    The program is self-contained.  It was developed on a VMS system,
   but should adapt easily to any system with a c compiler.  It has
   been ported to, and tested on, several popular workstations.

	** BUT CAUTION ... **
   Because many of the routines take a double-precision floating point
   Julian Date as their time argument, one must be sure that the machine
   and compiler carry sufficient mantissa to reach the desired accuracy.
   On VAX/VMS, the time resolution is of order 0.01 second.  This has also
   proven true on Sun and IBM workstations.

LEGALITIES:

   I make no guarantee as to the accuracy, reliability, or
   appropriateness of this program, though I have found it to be
   reasonably accurate and quite useful to the working astronomer.

   The program is COPYRIGHT 1993 BY JOHN THORSTENSEN.
   Permission is hereby granted for non-profit scientific or educational use.
   For-profit use (e. g., by astrologers!) must be through negotiated
   license.  The author requests that observatories and astronomy
   departments which install this as a utility notify the author
   by paper mail, just so I know how widely it is used.

   Credits:
    * The julian date and sidereal time routines were
    originally coded in PL/I by  Steve Maker of Dartmouth College.
    They were based on routines in the old American Ephemeris.
    * The conversion from julian date to calendar date is adapted
    from Numerical Recipes in c, by Press et al. (Cambridge University
    Press). I highly recommend this excellent, very useful book.


    APOLOGIES/DISCLAIMER:
    I am aware that the code here does not always conform to
    the best programming practices.  Not every possible error condition
    is anticipated, and no guarantee is given that this is bug-free.
    Nonetheless, most of this code has been shaken down at several
    hundred sites for several years, and I have never received any
    actual bug reports.  Many users have found this program
    to be useful.

    CHANGES SINCE THE ORIGINAL DISTRIBUTION ....

	The program as listed here is for the most part similar to that
	posted on the IRAF bulletin board in 1990.  Some changes
	include:

	01 In the original code, many functions returned structures, which
	   some c implementations do not like.  These have been eliminated.

	02 The original main() was extremely cumbersome; much of it has
	   been broken into smaller (but still large) functions.

	03 The hourly airmass includes a column for the altitude of the
	   sun, which is printed if it is greater than -18 degrees.

	04 The planets are included (see above).  As part of this, the
	   circumstances calculator issues a warning when one is within
	   three degrees of a major planet.  This warning is now also
	   included in the hourly-airmass table.

	05 The changeover from standard to daylight time has been rationalized.
	   Input times between 2 and 3 AM on the night when DST starts (which
	   are skipped over and  hence don't exist) are now disallowed; input
	   times between 1 and 2 AM on the night when DST ends (which are
	   ambiguous) are interpreted as standard times.  Warnings are printed
	   in both the almanac and calculator mode when one is near to the
	   changeover.

	06 a much more accurate moon calculation has been added; it's used
	   when the moon's coordinates are given explicitly, but not for
	   the rise/set times, which iterate and for which a lower precision
	   is adequate.

	07 It's possible now to set the observatory elevation; in a second
	   revision there are now two separate elevation parameters specified.
	   The elevation above the horizon used only in rise/set calculations
	   and adjusts rise/set times assuming the parameter is the elevation
	   above flat surroundings (e. g., an ocean).  The true elevation above
	   sea level is used (together with an ellipsoidal earth figure) in
	   determining the observatory's geocentric coordinates for use in
	   the topocentric correction of the moon's position and in the
	   calculation of the diurnal rotation part of the barycentric velocity
	   correction.  These refinements are quite small.

	08 The moon's altitude above the horizon is now printed in the
	   hourly airmass calculation; in the header line, its illuminated
	   fraction and angular separation from the object are included,
	   as computed for local midnight.

	09 The helio/barycentric corrections have been revised and improved.
	   The same routines used for planetary positions are used to
	   compute the offset from heliocentric to solar-system
	   barycentric positions and velocities.  The earth's position
	   (and the sun's position as well) have been improved somewhat
	   as well.

	10 The printed day and date are always based on the same truncation
	   of the julian date argument, so they should now always agree
	   arbitrarily close to midnight.

	11 A new convention has been adopted by which the default is that the
	   date specified is the evening date for the whole night.  This way,
	   calculating an almanac for the night of July 3/4 and then specifying
	   a time after midnight gives the circumstances for the *morning of
	   July 4*.  Typing 'n' toggles between this interpretation and a
	   literal interpretation of the date.

	12 The planetary proximity warning is now included in the hourly airmass
	   table.

	13 A routine has been added which calculates how far the moon is from
	   the nearest cardinal phase (to coin a phrase) and prints a
	   description.  This information is now included in both the almanac
	   and the calculator mode.

	14 The output formats have been changed slightly; it's hoped this
	   will enhance comprehensibility.

	15 A substantial revision affecting the user interface took place
	   in September of 1993.  A command 'a' has been added to the
	   'calculator' menu, which simply prints the almanac (rise, set,
	   and so on) for the current night.  I'd always found that it was
	   easy to get disoriented using the '=' command -- too much
	   information about the moment, not enough about the time
	   context.  Making the almanac info *conveniently* available
	   in the calculator mode helps your get oriented.

	   When the 'a' almanac is printed, space is saved over the
	   almanac printed on entry, because there does not need
	   to be a banner introducing the calculator mode.  Therefore some
	   extra information is included with the 'a' almanac; this includes
	   the length of the night from sunset to sunrise, the number of
	   hours the sun is below -18 degrees altitude, and the number of hours
	   moon is down after twilight.  In addition, moonrise and moonset
	   are printed in the order in which they occur, and the occasional
	   non-convergence of the rise/set algorithms at high latitude are
	   signalled more forcefully to the user.

	16 I found this 'a' command to be convenient in practice, and never
	   liked the previous structure of having to 'quit' the calculator
	   mode to see almanac information for a different night.  D'Anne
	   Thompson of NOAO also pointed out how hokey this was, especially the
	   use of a negative date to exit. So, I simply removed the outer
	   'almanac' loop and added a 'Q' to the main menu for 'quit'.  The
	   use of upper case -- for this one command only --  should guard
	   against accidental exit.

	17 The menu has been revised to be a little more readable.

	18 More error checking was added in Nov. 1993, especially for numbers.
	   If the user gives an illegal entry (such as a number which isn't
	   legal), the rest of the command line is thrown away (to avoid
	   having scanf simply chomp through it) and the user is prompted
	   as to what to do next.  This seems to have stopped all situations
	   in which execution could run away.  Also, typing repeated carriage
	   returns with nothing on the line -- which a frustrated novice
	   user may do because of the lack of any prompts -- causes a
	   little notice to be printed to draw attention to the help and menu
	   texts.

	19 I found in practice that, although the input parameters and
	   conditions are deducible *in principle* from such things as the
	   'a' and '=' output, it takes too much digging to find them.  So
	   I instituted an 'l' command to 'look' at the current parameter
	   values.  To make room for this I put the 'Cautions and legalities'
	   into the 'w' (inner workings) help text.  This looks as though
	   it will be be very helpful to the user.

	20 The more accurate moon calculation is used for moonrise and
	   moonset; the execution time penalty appears to be trivial.
	   Low precision moon is still used for the summary moon information
	   printed along with the hourly airmass table.

	21 A calculation of the expected portion of the night-sky
	   brightness due to moonlight has been added.  This is based on
	   Krisciunas and Schaefer's analytic fits (PASP, 1991).  Obviously,
	   it's only an estimate which will vary considerably depending on
	   atmospheric conditions.

	22 A very crude estimator of the zenith sky brightness in twilight
	   has been added.

	23 A topocentric correction has been added for the sun, in anticipation
	   of adding eclipse prediction.

	24 The code now checks for eclipses of the sun and moon, by making
	   very direct use of the predicted positions.  If an eclipse is
	   predicted, a notice is printed in print_circumstances; also, a
	   disclaimer is printed for the lunar sky brightness if a lunar
	   eclipse is predicted to be under way.

	25 In the driver of the main calculator loop, a provision has been
	   added for getting characters out of a buffer rather than reading
	   them directly off the command line.  This allows one to type any
	   valid command character (except Q for quit) directly after a number
	   in an argument without generating a complaint from the program
	   (see note 18).  This had been an annoying flaw.

	26 In 1993 December/1994 January, the code was transplanted
	   to a PC and compiled under Borland Turbo C++, with strict
	   ANSI rules.  The code was cut into 9 parts -- 8 subroutine
	   files, the main program, and an incude file containing
	   global variables and function prototypes.

	27 An "extra goodies" feature has been added -- at present it
	   computes geocentric times of a repeating phenomenon as
	   viewed from a site.  This can be used for relatively little-used
           commands to save space on the main menu.

	28 The sun and moon are now included in the "major planets"
	   printout.  This allows one to find their celestial positions
	   even when they are not visible from the selected site.

	29 A MAJOR new feature was added in February 1994, which computes
           the observability of an object at new and full moon over a
           range of dates.  The galactic/ecliptic coordinate converter
           was moved to the extra goodies menu to make room for this.

	30 Inclusion of a season-long timescale means that it's not
           always necessary to specify a date on entry to the program.
           Accordingly, the program immediately starts up in what used
           to be called "calculator" mode -- only the site is prompted
           for.  It is thought that the site will be relevant to nearly
           all users.

	31 Because the user is not led by the hand as much as before, the
           startup messages were completely revised to direct new users
           toward a short `guided tour' designed to show the program's
	   command structure and capabilities very quickly.  Tests on
	   volunteers showed that users instinctively go for anything
	   called the `menu', despite the fact that that's a slow way to
	   learn, so all mention of the menu option is removed from the
	   startup sequence; they'll catch on soon enough.

	32 Code has been added to automatically set the time and
           date to the present on startup.  A menu option 'T' has been
           added to set the time and date to the present plus a settable
           offset.  This should be very useful while observing.

	33 Because Sun machines apparently do not understand ANSI-standard
           function declarations, the code has been revised back to K&R
           style.  It's also been put back in a monolithic block for
           simplicity in distribution.

	34 The startup has been simplified still further, in that the
           coordinates are set automatically to the zenith on startup.
	   An 'xZ' menu item sets to the zenith for the currently specified
           time and date (not necessarily the real time and date.)

	35 Another MAJOR new capability was added in early 1994 --
           the ability to read in a list of objects and set the current
	   coordinates to an object on the list.  The list can be sorted
           in a number of ways using information about the site, date
           and time.

	35 Calculator-like commands were added to the extra goodies menu
           to do precessions and to convert Julian dates to calendar
           dates.  An option to set the date and time to correspond to
           a given julian date was also added.

	36 Another substantial new capability was added Aug 94 -- one can
           toggle open a log file (always named "skyclg") and keep
           a record of the output.  This is done simply by replacing
           most occurrences of "printf" with "oprintf", which mimics
           printf but writes to a log file as well if it is open.
	   This appears to slow down execution somewhat.

	37 12-degree twilight has been added to the almanac.  While the
	   awkward "goto" statements have been retained, the statement
           labels have been revised to make them a little clearer.
*/

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

/* a couple of the system-dependent magic numbers are defined here */

#define SYS_CLOCK_OK 1    /* 1 means ANSI-standard time libraries do work,
   2 means they don't.  This is used by compiler switches in file 5 and
   the main program.  */

#define LOG_FILES_OK 1  /* 1 means that log files are enabled.
			Any other value means they're not.  */

#define MAX_OBJECTS 500
#define MINSHORT -32767   /* min, max short integers and double precision */
#define MAXSHORT 32767
#define MAXDOUBLE 1.0e38
#define MINDOUBLE -1.0e38
#define BUFSIZE 150

/* some (not all) physical, mathematical, and astronomical constants
   used are defined here. */

#define  PI                3.14159265358979
#define  ARCSEC_IN_RADIAN  206264.8062471
#define  DEG_IN_RADIAN     57.2957795130823
#define  HRS_IN_RADIAN     3.819718634205
#define  KMS_AUDAY         1731.45683633   /* km per sec in 1 AU/day */
#define  SS_MASS           1.00134198      /* solar system mass in solar units */
#define  J2000             2451545.        /* Julian date at standard epoch */
#define  SEC_IN_DAY        86400.
#define  FLATTEN           0.003352813   /* flattening of earth, 1/298.257 */
#define  EQUAT_RAD         6378137.    /* equatorial radius of earth, meters */
#define  ASTRO_UNIT        1.4959787066e11 /* 1 AU in meters */
#define  RSUN              6.96000e8  /* IAU 1976 recom. solar radius, meters */
#define  RMOON             1.738e6    /* IAU 1976 recom. lunar radius, meters */
#define  PLANET_TOL        3.          /* flag if nearer than 3 degrees
						to a major planet ... */
#define  KZEN              0.172       /* zenith extinction, mag, for use
				     in lunar sky brightness calculations. */
#define FIRSTJD            2415387.  /* 1901 Jan 1 -- calendrical limit */
#define LASTJD             2488070.  /* 2099 Dec 31 */

/* MAGIC NUMBERS which might depend on how accurately double-
   precision floating point is handled on your machine ... */

#define  EARTH_DIFF        0.05            /* used in numerical
   differentiation to find earth velocity -- this value gives
   about 8 digits of numerical accuracy on the VAX, but is
   about 3 orders of magnitude larger than the value where roundoff
   errors become apparent. */

#define  MIDN_TOL          0.00001         /* this is no longer
   used -- it was formerly
   how close (in days) a julian date has to be to midnight
   before a warning flag is printed for the reader.  VAX
   double precision renders a Julian date considerably
   more accurately than this.  The day and date are now based
   on the same rounding of the julian date, so they should
   always agree. */

/*  FUNCTION PROTOTYPES and type definitions ....
    These are used in breaking the code into function libraries.
    They work properly on a strictly ANSI compiler, so they
    apparently comply with the ANSI standard format.  */

struct coord
   {
     short sign;  /* carry sign explicitly since -0 not neg. */
     double hh;
     double mm;
     double ss;
   };

struct date_time
   {
	short y;
	short mo;
	short d;
	short h;
	short mn;
	float s;
   };

struct elements
   {
	char name[9];
	double incl;
	double Omega;
	double omega;
	double a;
	double daily;
	double ecc;
	double L_0;
	double mass;
   };

struct objct {
	char name[20];
	double ra;
	double dec;
	float ep;
	float xtra;  /* mag, whatever */
};


/* from Line 1553 */
#define IGREG 2299161
/* from Line 5129--5132 */
#define ALT_3  19.47  /* 19.47 degrees altitude => sec z = 3 */
#define ALT_2  30.
#define ALT_15 41.81
#define SID_RATE 1.0027379093  /* sidereal / solar rate */

#ifdef __cplusplus
extern "C" {
#endif
void oprntf(char *fmt, ...);
char getch();
void ungetch(int c);
int legal_num_part(char c);
int legal_int_part(char c);
int legal_command_char(char c);
int parsedouble(char *s,double *d);
int getdouble(double *d,double least,double most,char *errprompt);
int parseshort(char *s,short *d);
int getshort(short *d,short least,short most,char *errprompt);
double bab_to_dec(struct coord bab);
void dec_to_bab (double deci,struct coord *bab);
short get_line(char *s);
double get_coord();
void put_coords(double deci,short precision);
void load_site(double *longit,double *lat,double *stdz,short *use_dst,char *zone_name,char *zabr,double *elevsea,double *elev,double *horiz,char *site_name);
double atan_circ(double x,double y);
void min_max_alt(double lat,double dec,double *min,double *max);
double altit(double dec,double ha,double lat,double *az);
double secant_z(double alt);
double ha_alt(double dec,double lat,double alt);
double subtend(double ra1,double dec1,double ra2,double dec2);
int get_pm(double dec,double *mura,double *mudec);
int get_date(struct date_time date);
int get_time(struct date_time date);
double date_to_jd(struct date_time date);
short day_of_week(double jd);
void caldat(double jdin,struct date_time *date,short *dow);
void print_day(short d);
void print_all(double jdin);
void print_current(struct date_time date,short night_date,short enter_ut);
void print_calendar(double jdin,short *dow);
void print_time(double jdin,short prec);
double frac_part(double x);
double lst(double jd,double longit);
double adj_time(double x);
void lpmoon(double jd,double lat,double sid,double *ra,double *dec,double *dist);
void lpsun(double jd,double *ra,double *dec);
void eclrot(double jd,double *x,double* y,double *z);
double circulo(double x);
void geocent(double geolong,double geolat,double height,double *x_geo,double *y_geo,double *z_geo);
double etcorr(double jd);
void accumoon(double jd,double geolat,double lst,double elevsea,double *geora,double *geodec,double *geodist,double *topora,double *topodec,double *topodist);
void flmoon(int n,int nph,double *jdout);
float lun_age(double jd,int *nlun);
void print_phase(double jd);
double lunskybright(double alpha,double rho,double kzen,double altmoon,double alt,double moondist);
void accusun(double jd,double lst,double geolat,double *ra,double *dec,double *dist,double *topora,double *topodec,double *x,double *y,double *z);
double jd_moon_alt(double alt,double jdguess,double lat,double longit,double elevsea);
double jd_sun_alt(double alt,double jdguess,double lat,double longit);
float ztwilight(double alt);
void find_dst_bounds(short yr,double stdz,short use_dst,double *jdb,double *jde);
double zone(short use_dst,double stdz,double jd,double jdb,double jde);
double true_jd(struct date_time date,short use_dst,short enter_ut,short night_date,double stdz);
void print_tz(double jd,short use,double jdb,double jde,char zabr);
void xyz_cel(double x,double y,double z,double *r,double *d);
void precrot(double rorig, double dorig, double orig_epoch, double final_epoch, double *rf, double *df);
void mass_precess();
void galact(double ra,double dec,double epoch,double *glong,double *glat);
void eclipt(double ra,double dec,double epoch,double jd,double *curep,double *eclong,double *eclat);
double parang(double ha,double dec,double lat);
void comp_el(double jd);
void planetxyz(int p,double jd,double  *x,double  *y,double  *z);
void planetvel(int p,double jd,double *vx,double *vy,double *vz);
void xyz2000(double jd,double x,double y,double z);
void earthview(double *x,double *y,double *z,int i,double *ra,double *dec);
void pposns(double jd,double lat,double sid,short print_option,double *planra,double *plandec);
void barycor(double jd,double *x,double *y,double *z,double *xdot,double *ydot,double *zdot);
void helcor(double jd,double ra,double dec,double ha,double lat,double elevsea,double *tcor,double *vcor);
float overlap(double r1,double r2,double sepn);
void solecl(double sun_moon,double distmoon,double distsun);
short lunecl(double georamoon,double geodecmoon,double geodistmoon,double rasun,double decsun,double distsun);
void planet_alert(double jd,double ra,double dec,double tolerance);
short setup_time_place(struct date_time date,double longit,double lat,double stdz,short use_dst,char *zone_name,
        char zabr,char *site_name,short enter_ut,short night_date,double *jdut,double *jdlocal,double *jdb,double *jde,double *sid,
	double *curepoch);
void print_tonight(struct date_time date,double lat,double longit,double elevsea,double elev,double horiz,char *site_name,double stdz,
	char *zone_name,char zabr,short use_dst,double *jdb,double *jde,short short_long);
void print_circumstances(double objra,double objdec,double objepoch,double jd,double curep,double 
	mura_arcs,double mura_sec,double mudec,double sid,double lat,double elevsea,double horiz);
void hourly_airmass(struct date_time date,double stdz,double lat,double longit,double horiz,short use_dst,double objra,double objdec,
  double objepoch,double  mura_sec,double mura_arcs,double mudec);
void print_params(struct date_time date,short enter_ut,short night_date,double stdz,double lat,double longit,char *site_name,
    double elevsea,double elev,short use_dst,double objra,double objdec,double objepoch,double mura_sec,double mura_arcs,double mudec);
void print_menu();
void print_tutorial();
void print_examples();
void print_accuracy();
void print_legalities();
void ephemgen(double ra,double dec,double ep,double lat,double longit);
double hrs_up(double jdup,double jddown,double jdeve,double jdmorn);
void print_air(double secz,short prec);
void print_ha_air(double ha,double secz,short prec1,short prec2);
void obs_season(double ra,double dec,double epoch,double lat,double longit);
int get_sys_date(struct date_time date,short use_dst,short enter_ut,short night_date,double stdz,double toffset);
void indexx(int n,float arrin[],int indx[]);
int read_obj_list();
int find_by_name(double ra,double dec,double epoch,struct date_time date,short use_dst,short enter_ut,short night_date,double stdz,double lat,double longit);
void type_list(struct date_time date,short use_dst,short enter_ut,short night_date,double stdz,double lat,double longit);
int find_nearest(double *ra,double *dec,double epoch,struct date_time date,short use_dst,short enter_ut,short night_date,double stdz,double lat,double longit);
void set_zenith(struct date_time date,short use_dst,short enter_ut,short night_date,double stdz,double lat,double longit,double epoch,double *ra,double *dec);
void printephase(struct date_time date,short use_dst,short enter_ut,short night_date,double stdz,double lat,double longit,double epoch,double ra,double dec);
int set_to_jd(struct date_time *date,short use_dst,short enter_ut,short night_date,double stdz,double jd);
void skycalcmain();
#ifdef __cplusplus
}
#endif
