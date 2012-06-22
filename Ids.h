#ifndef IDS
#define IDS

// #define LANGUAGE_SPANISH

#define ID_VERSION_INFO         1
#define ID_ICON                 10
#define ID_SOUNDBOING           100
#define ID_SOUNDLASER           101
#define ID_SOUNDLAUGH           102
#define ID_SOUNDERROR           103
#define ID_BALLGREENR           2001
#define ID_BALLGREENL           2002
#define ID_BALLBLUER            2003
#define ID_BALLBLUEL            2004
#define ID_BALLREDR             2005
#define ID_BALLREDL             2006
#define ID_BALLYELLOWR          2007
#define ID_BALLYELLOWL          2008
#define ID_BALLORANGE1          2009
#define ID_BALLORANGE2          2010
#define ID_BALLORANGE3          2011
#define ID_BALLEXPLOTED         2012
#define ID_BALLSMALLEXPLOTED    2013
#define ID_BALLSKULL            2014
#define ID_STATUSSCORE          2015
#define ID_STATUSLIVES          2016
#define ID_SPLASH_ENGLISH       2017
#define ID_SPLASH_SPANISH       2018
#ifdef LANGUAGE_SPANISH
#define ID_SPLASH ID_SPLASH_SPANISH
#else
#define ID_SPLASH ID_SPLASH_ENGLISH
#endif
#define ID_HIGHSCOREDIALOG      3000
#define ID_HIGHSCOREDIALOGEDIT  3001

#endif
