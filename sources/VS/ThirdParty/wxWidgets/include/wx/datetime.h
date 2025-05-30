/////////////////////////////////////////////////////////////////////////////
// Name:        wx/datetime.h
// Purpose:     declarations of time/date related classes (wxDateTime,
//              wxTimeSpan)
// Author:      Vadim Zeitlin
// Modified by:
// Created:     10.02.99
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DATETIME_H
#define _WX_DATETIME_H

#include "wx/defs.h"

#if wxUSE_DATETIME

#include <time.h>

#include <limits.h>             // for INT_MIN

#include "wx/longlong.h"
#include "wx/anystr.h"

class WXDLLIMPEXP_FWD_BASE wxDateTime;
class WXDLLIMPEXP_FWD_BASE wxTimeSpan;
class WXDLLIMPEXP_FWD_BASE wxDateSpan;
#ifdef __WINDOWS__
struct _SYSTEMTIME;
#endif

#include "wx/dynarray.h"

// not all c-runtimes are based on 1/1/1970 being (time_t) 0
// set this to the corresponding value in seconds 1/1/1970 has on your
// systems c-runtime

#define WX_TIME_BASE_OFFSET 0

/*
 * TODO
 *
 * + 1. Time zones with minutes (make TimeZone a class)
 * ? 2. getdate() function like under Solaris
 * + 3. text conversion for wxDateSpan
 * + 4. pluggable modules for the workdays calculations
 *   5. wxDateTimeHolidayAuthority for Easter and other christian feasts
 */

/*
  The three (main) classes declared in this header represent:

  1. An absolute moment in the time (wxDateTime)
  2. A difference between two moments in the time, positive or negative
     (wxTimeSpan)
  3. A logical difference between two dates expressed in
     years/months/weeks/days (wxDateSpan)

  The following arithmetic operations are permitted (all others are not):

  addition
  --------

  wxDateTime + wxTimeSpan = wxDateTime
  wxDateTime + wxDateSpan = wxDateTime
  wxTimeSpan + wxTimeSpan = wxTimeSpan
  wxDateSpan + wxDateSpan = wxDateSpan

  subtraction
  ------------
  wxDateTime - wxDateTime = wxTimeSpan
  wxDateTime - wxTimeSpan = wxDateTime
  wxDateTime - wxDateSpan = wxDateTime
  wxTimeSpan - wxTimeSpan = wxTimeSpan
  wxDateSpan - wxDateSpan = wxDateSpan

  multiplication
  --------------
  wxTimeSpan * number = wxTimeSpan
  number * wxTimeSpan = wxTimeSpan
  wxDateSpan * number = wxDateSpan
  number * wxDateSpan = wxDateSpan

  unitary minus
  -------------
  -wxTimeSpan = wxTimeSpan
  -wxDateSpan = wxDateSpan

  For each binary operation OP (+, -, *) we have the following operatorOP=() as
  a method and the method with a symbolic name OPER (Add, Subtract, Multiply)
  as a synonym for it and another const method with the same name which returns
  the changed copy of the object and operatorOP() as a global function which is
  implemented in terms of the const version of OPEN. For the unary - we have
  operator-() as a method, Neg() as synonym for it and Negate() which returns
  the copy of the object with the changed sign.
*/

// an invalid/default date time object which may be used as the default
// argument for arguments of type wxDateTime; it is also returned by all
// functions returning wxDateTime on failure (this is why it is also called
// wxInvalidDateTime)
class WXDLLIMPEXP_FWD_BASE wxDateTime;

extern WXDLLIMPEXP_DATA_BASE(const char) wxDefaultDateTimeFormat[];
extern WXDLLIMPEXP_DATA_BASE(const char) wxDefaultTimeSpanFormat[];
extern WXDLLIMPEXP_DATA_BASE(const wxDateTime) wxDefaultDateTime;

#define wxInvalidDateTime wxDefaultDateTime


// ----------------------------------------------------------------------------
// conditional compilation
// ----------------------------------------------------------------------------

// if configure detected strftime(), we have it too
#ifdef HAVE_STRFTIME
    #define wxHAS_STRFTIME
// suppose everyone else has strftime
#else
    #define wxHAS_STRFTIME
#endif

// ----------------------------------------------------------------------------
// wxDateTime represents an absolute moment in the time
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxWARN_UNUSED wxDateTime
{
public:
    // types
    // ------------------------------------------------------------------------

        // a small unsigned integer type for storing things like minutes,
        // seconds &c. It should be at least short (i.e. not char) to contain
        // the number of milliseconds - it may also be 'int' because there is
        // no size penalty associated with it in our code, we don't store any
        // data in this format
    typedef unsigned short wxDateTime_t;

    // constants
    // ------------------------------------------------------------------------

        // the timezones
    enum TZ
    {
        // the time in the current time zone
        Local,

        // zones from GMT (= Greenwich Mean Time): they're guaranteed to be
        // consequent numbers, so writing something like `GMT0 + offset` is
        // safe if abs(offset) <= 12

        // underscore stands for minus
        GMT_12, GMT_11, GMT_10, GMT_9, GMT_8, GMT_7,
        GMT_6, GMT_5, GMT_4, GMT_3, GMT_2, GMT_1,
        GMT0,
        GMT1, GMT2, GMT3, GMT4, GMT5, GMT6,
        GMT7, GMT8, GMT9, GMT10, GMT11, GMT12, GMT13,
        // Note that GMT12 and GMT_12 are not the same: there is a difference
        // of exactly one day between them

        // some symbolic names for TZ

        // Europe
        WET = GMT0,                         // Western Europe Time
        WEST = GMT1,                        // Western Europe Summer Time
        CET = GMT1,                         // Central Europe Time
        CEST = GMT2,                        // Central Europe Summer Time
        EET = GMT2,                         // Eastern Europe Time
        EEST = GMT3,                        // Eastern Europe Summer Time
        MSK = GMT3,                         // Moscow Time
        MSD = GMT4,                         // Moscow Summer Time

        // US and Canada
        AST = GMT_4,                        // Atlantic Standard Time
        ADT = GMT_3,                        // Atlantic Daylight Time
        EST = GMT_5,                        // Eastern Standard Time
        EDT = GMT_4,                        // Eastern Daylight Saving Time
        CST = GMT_6,                        // Central Standard Time
        CDT = GMT_5,                        // Central Daylight Saving Time
        MST = GMT_7,                        // Mountain Standard Time
        MDT = GMT_6,                        // Mountain Daylight Saving Time
        PST = GMT_8,                        // Pacific Standard Time
        PDT = GMT_7,                        // Pacific Daylight Saving Time
        HST = GMT_10,                       // Hawaiian Standard Time
        AKST = GMT_9,                       // Alaska Standard Time
        AKDT = GMT_8,                       // Alaska Daylight Saving Time

        // Australia

        A_WST = GMT8,                       // Western Standard Time
        A_CST = GMT13 + 1,                  // Central Standard Time (+9.5)
        A_EST = GMT10,                      // Eastern Standard Time
        A_ESST = GMT11,                     // Eastern Summer Time

        // New Zealand
        NZST = GMT12,                       // Standard Time
        NZDT = GMT13,                       // Daylight Saving Time

        // TODO add more symbolic timezone names here

        // Universal Coordinated Time = the new and politically correct name
        // for GMT
        UTC = GMT0
    };

        // the calendar systems we know about: notice that it's valid (for
        // this classes purpose anyhow) to work with any of these calendars
        // even with the dates before the historical appearance of the
        // calendar
    enum Calendar
    {
        Gregorian,  // current calendar
        Julian      // calendar in use since -45 until the 1582 (or later)

        // TODO Hebrew, Chinese, Maya, ... (just kidding) (or then may be not?)
    };

        // the country parameter is used so far for calculating the start and
        // the end of DST period and for deciding whether the date is a work
        // day or not
        //
        // TODO move this to intl.h

    enum Country
    {
        Country_Unknown, // no special information for this country
        Country_Default, // set the default country with SetCountry() method
                         // or use the default country with any other

        // TODO add more countries (for this we must know about DST and/or
        //      holidays for this country)

        // Western European countries: we assume that they all follow the same
        // DST rules (true or false?)
        Country_WesternEurope_Start,
        Country_EEC = Country_WesternEurope_Start,
        France,
        Germany,
        UK,
        Country_WesternEurope_End = UK,

        Russia,
        USA
    };
        // symbolic names for the months
    enum Month
    {
        Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec, Inv_Month
    };

        // symbolic names for the weekdays
    enum WeekDay
    {
        Sun, Mon, Tue, Wed, Thu, Fri, Sat, Inv_WeekDay
    };

        // invalid value for the year
    enum Year
    {
        Inv_Year = SHRT_MIN    // should hold in wxDateTime_t
    };

        // flags for GetWeekDayName and GetMonthName
    enum NameFlags
    {
        Name_Full = 0x01,       // return full name
        Name_Abbr = 0x02        // return abbreviated name
    };

        // flags for GetWeekOfYear and GetWeekOfMonth
    enum WeekFlags
    {
        Default_First,   // Sunday_First for US, Monday_First for the rest
        Monday_First,    // week starts with a Monday
        Sunday_First     // week starts with a Sunday
    };

    // Currently we assume that DST is always shifted by 1 hour, this seems to
    // be always true in practice. If this ever needs to change, search for all
    // places using DST_OFFSET and update them.
    enum
    {
        DST_OFFSET = 3600
    };


    // helper classes
    // ------------------------------------------------------------------------

        // a class representing a time zone: basically, this is just an offset
        // (in seconds) from GMT
    class WXDLLIMPEXP_BASE TimeZone
    {
    public:
        TimeZone(TZ tz);

        // create time zone object with the given offset
        TimeZone(long offset = 0) { m_offset = offset; }

        static TimeZone Make(long offset)
        {
            TimeZone tz;
            tz.m_offset = offset;
            return tz;
        }

        bool IsLocal() const { return m_offset == -1; }

        long GetOffset() const;

    private:
        // offset for this timezone from GMT in seconds
        long m_offset;
    };

        // standard struct tm is limited to the years from 1900 (because
        // tm_year field is the offset from 1900), so we use our own struct
        // instead to represent broken down time
        //
        // NB: this struct should always be kept normalized (i.e. mon should
        //     be < 12, 1 <= day <= 31 &c), so use AddMonths(), AddDays()
        //     instead of modifying the member fields directly!
    struct WXDLLIMPEXP_BASE Tm
    {
        wxDateTime_t msec, sec, min, hour,
                     mday,  // Day of the month in 1..31 range.
                     yday;  // Day of the year in 0..365 range.
        Month mon;
        int year;

        // default ctor inits the object to an invalid value
        Tm();

        // ctor from struct tm and the timezone
        Tm(const struct tm& tm, const TimeZone& tz);

        // check that the given date/time is valid (in Gregorian calendar)
        bool IsValid() const;

        // get the week day
        WeekDay GetWeekDay() // not const because wday may be changed
        {
            if ( wday == Inv_WeekDay )
                ComputeWeekDay();

            return (WeekDay)wday;
        }

        // add the given number of months to the date keeping it normalized
        void AddMonths(int monDiff);

        // add the given number of months to the date keeping it normalized
        void AddDays(int dayDiff);

    private:
        // compute the weekday from other fields
        void ComputeWeekDay();

        // the timezone we correspond to
        TimeZone m_tz;

        // This value can only be accessed via GetWeekDay() and not directly
        // because it's not always computed when creating this object and may
        // need to be calculated on demand.
        wxDateTime_t wday;
    };

    // static methods
    // ------------------------------------------------------------------------

        // set the current country
    static void SetCountry(Country country);
        // get the current country
    static Country GetCountry();

        // return true if the country is a West European one (in practice,
        // this means that the same DST rules as for EEC apply)
    static bool IsWestEuropeanCountry(Country country = Country_Default);

        // return the current year
    static int GetCurrentYear(Calendar cal = Gregorian);

        // convert the year as returned by wxDateTime::GetYear() to a year
        // suitable for BC/AD notation. The difference is that BC year 1
        // corresponds to the year 0 (while BC year 0 didn't exist) and AD
        // year N is just year N.
    static int ConvertYearToBC(int year);

        // return the current month
    static Month GetCurrentMonth(Calendar cal = Gregorian);

        // returns true if the given year is a leap year in the given calendar
    static bool IsLeapYear(int year = Inv_Year, Calendar cal = Gregorian);

        // acquires the first day of week based on locale and/or OS settings
    static bool GetFirstWeekDay(WeekDay *firstDay);

        // get the century (19 for 1999, 20 for 2000 and -5 for 492 BC)
    static int GetCentury(int year);

        // returns the number of days in this year (356 or 355 for Gregorian
        // calendar usually :-)
    static wxDateTime_t GetNumberOfDays(int year, Calendar cal = Gregorian);

        // get the number of the days in the given month (default value for
        // the year means the current one)
    static wxDateTime_t GetNumberOfDays(Month month,
                                        int year = Inv_Year,
                                        Calendar cal = Gregorian);


        // get the full (default) or abbreviated month name in the current
        // locale, returns empty string on error
    static wxString GetMonthName(Month month,
                                 NameFlags flags = Name_Full);

        // get the standard English full (default) or abbreviated month name
    static wxString GetEnglishMonthName(Month month,
                                        NameFlags flags = Name_Full);

        // get the full (default) or abbreviated weekday name in the current
        // locale, returns empty string on error
    static wxString GetWeekDayName(WeekDay weekday,
                                   NameFlags flags = Name_Full);

        // get the standard English full (default) or abbreviated weekday name
    static wxString GetEnglishWeekDayName(WeekDay weekday,
                                          NameFlags flags = Name_Full);

        // get the AM and PM strings in the current locale (may be empty)
    static void GetAmPmStrings(wxString *am, wxString *pm);

        // return true if the given country uses DST for this year
    static bool IsDSTApplicable(int year = Inv_Year,
                                Country country = Country_Default);

        // get the beginning of DST for this year, will return invalid object
        // if no DST applicable in this year. The default value of the
        // parameter means to take the current year.
    static wxDateTime GetBeginDST(int year = Inv_Year,
                                  Country country = Country_Default);
        // get the end of DST for this year, will return invalid object
        // if no DST applicable in this year. The default value of the
        // parameter means to take the current year.
    static wxDateTime GetEndDST(int year = Inv_Year,
                                Country country = Country_Default);

        // return the wxDateTime object for the current time
    static inline wxDateTime Now();

        // return the wxDateTime object for the current time with millisecond
        // precision (if available on this platform)
    static wxDateTime UNow();

        // return the wxDateTime object for today midnight: i.e. as Now() but
        // with time set to 0
    static inline wxDateTime Today();

    // constructors: you should test whether the constructor succeeded with
    // IsValid() function. The values Inv_Month and Inv_Year for the
    // parameters mean take current month and/or year values.
    // ------------------------------------------------------------------------

        // default ctor does not initialize the object, use Set()!
    wxDateTime() : m_time(wxINT64_MIN) { }

        // from time_t: seconds since the Epoch 00:00:00 UTC, Jan 1, 1970)
    inline wxDateTime(time_t timet);
        // from broken down time/date (only for standard Unix range)
    inline wxDateTime(const struct tm& tm);
        // from broken down time/date (any range)
    inline wxDateTime(const Tm& tm);

        // from JDN (beware of rounding errors)
    inline wxDateTime(double jdn);

        // from separate values for each component, date set to today
    inline wxDateTime(wxDateTime_t hour,
                      wxDateTime_t minute = 0,
                      wxDateTime_t second = 0,
                      wxDateTime_t millisec = 0);
        // from separate values for each component with explicit date
    inline wxDateTime(wxDateTime_t day,             // day of the month
                      Month        month,
                      int          year = Inv_Year, // 1999, not 99 please!
                      wxDateTime_t hour = 0,
                      wxDateTime_t minute = 0,
                      wxDateTime_t second = 0,
                      wxDateTime_t millisec = 0);
#ifdef __WINDOWS__
    wxDateTime(const struct _SYSTEMTIME& st)
    {
        SetFromMSWSysTime(st);
    }
#endif

        // default copy ctor ok

        // no dtor

    // assignment operators and Set() functions: all non const methods return
    // the reference to this object. IsValid() should be used to test whether
    // the function succeeded.
    // ------------------------------------------------------------------------

        // set to the current time
    inline wxDateTime& SetToCurrent();

        // set to given time_t value
    inline wxDateTime& Set(time_t timet);

        // set to given broken down time/date
    wxDateTime& Set(const struct tm& tm);

        // set to given broken down time/date
    inline wxDateTime& Set(const Tm& tm);

        // set to given JDN (beware of rounding errors)
    wxDateTime& Set(double jdn);

        // set to given time, date = today
    wxDateTime& Set(wxDateTime_t hour,
                    wxDateTime_t minute = 0,
                    wxDateTime_t second = 0,
                    wxDateTime_t millisec = 0);

        // from separate values for each component with explicit date
        // (defaults for month and year are the current values)
    wxDateTime& Set(wxDateTime_t day,
                    Month        month,
                    int          year = Inv_Year, // 1999, not 99 please!
                    wxDateTime_t hour = 0,
                    wxDateTime_t minute = 0,
                    wxDateTime_t second = 0,
                    wxDateTime_t millisec = 0);

        // resets time to 00:00:00, doesn't change the date
    wxDateTime& ResetTime();

        // get the date part of this object only, i.e. the object which has the
        // same date as this one but time of 00:00:00
    wxDateTime GetDateOnly() const;

        // the following functions don't change the values of the other
        // fields, i.e. SetMinute() won't change either hour or seconds value

        // set the year
    wxDateTime& SetYear(int year);
        // set the month
    wxDateTime& SetMonth(Month month);
        // set the day of the month
    wxDateTime& SetDay(wxDateTime_t day);
        // set hour
    wxDateTime& SetHour(wxDateTime_t hour);
        // set minute
    wxDateTime& SetMinute(wxDateTime_t minute);
        // set second
    wxDateTime& SetSecond(wxDateTime_t second);
        // set millisecond
    wxDateTime& SetMillisecond(wxDateTime_t millisecond);

        // assignment operator from time_t
    wxDateTime& operator=(time_t timet) { return Set(timet); }

        // assignment operator from broken down time/date
    wxDateTime& operator=(const struct tm& tm) { return Set(tm); }

        // assignment operator from broken down time/date
    wxDateTime& operator=(const Tm& tm) { return Set(tm); }

        // default assignment operator is ok

    // calendar calculations (functions which set the date only leave the time
    // unchanged, e.g. don't explicitly zero it): SetXXX() functions modify the
    // object itself, GetXXX() ones return a new object.
    // ------------------------------------------------------------------------

        // set to the given week day in the same week as this one
    wxDateTime& SetToWeekDayInSameWeek(WeekDay weekday,
                                       WeekFlags flags = Monday_First);
    inline wxDateTime GetWeekDayInSameWeek(WeekDay weekday,
                                           WeekFlags flags = Monday_First) const;

        // set to the next week day following this one
    wxDateTime& SetToNextWeekDay(WeekDay weekday);
    inline wxDateTime GetNextWeekDay(WeekDay weekday) const;

        // set to the previous week day before this one
    wxDateTime& SetToPrevWeekDay(WeekDay weekday);
    inline wxDateTime GetPrevWeekDay(WeekDay weekday) const;

        // set to Nth occurrence of given weekday in the given month of the
        // given year (time is set to 0), return true on success and false on
        // failure. n may be positive (1..5) or negative to count from the end
        // of the month (see helper function SetToLastWeekDay())
    bool SetToWeekDay(WeekDay weekday,
                      int n = 1,
                      Month month = Inv_Month,
                      int year = Inv_Year);
    inline wxDateTime GetWeekDay(WeekDay weekday,
                                 int n = 1,
                                 Month month = Inv_Month,
                                 int year = Inv_Year) const;

        // sets to the last weekday in the given month, year
    inline bool SetToLastWeekDay(WeekDay weekday,
                                 Month month = Inv_Month,
                                 int year = Inv_Year);
    inline wxDateTime GetLastWeekDay(WeekDay weekday,
                                     Month month = Inv_Month,
                                     int year = Inv_Year);

        // returns the date corresponding to the given week day of the given
        // week (in ISO notation) of the specified year
    static wxDateTime SetToWeekOfYear(int year,
                                      wxDateTime_t numWeek,
                                      WeekDay weekday = Mon);

        // sets the date to the last day of the given (or current) month or the
        // given (or current) year
    wxDateTime& SetToLastMonthDay(Month month = Inv_Month,
                                  int year = Inv_Year);
    inline wxDateTime GetLastMonthDay(Month month = Inv_Month,
                                      int year = Inv_Year) const;

        // sets to the given year day (1..365 or 366)
    wxDateTime& SetToYearDay(wxDateTime_t yday);
    inline wxDateTime GetYearDay(wxDateTime_t yday) const;

        // The definitions below were taken verbatim from
        //
        //      http://www.capecod.net/~pbaum/date/date0.htm
        //
        // (Peter Baum's home page)
        //
        // definition: The Julian Day Number, Julian Day, or JD of a
        // particular instant of time is the number of days and fractions of a
        // day since 12 hours Universal Time (Greenwich mean noon) on January
        // 1 of the year -4712, where the year is given in the Julian
        // proleptic calendar. The idea of using this reference date was
        // originally proposed by Joseph Scalizer in 1582 to count years but
        // it was modified by 19th century astronomers to count days. One
        // could have equivalently defined the reference time to be noon of
        // November 24, -4713 if were understood that Gregorian calendar rules
        // were applied. Julian days are Julian Day Numbers and are not to be
        // confused with Julian dates.
        //
        // definition: The Rata Die number is a date specified as the number
        // of days relative to a base date of December 31 of the year 0. Thus
        // January 1 of the year 1 is Rata Die day 1.

        // get the Julian Day number (the fractional part specifies the time of
        // the day, related to noon - beware of rounding errors!)
    double GetJulianDayNumber() const;
    double GetJDN() const { return GetJulianDayNumber(); }

        // get the Modified Julian Day number: it is equal to JDN - 2400000.5
        // and so integral MJDs correspond to the midnights (and not noons).
        // MJD 0 is Nov 17, 1858
    double GetModifiedJulianDayNumber() const { return GetJDN() - 2400000.5; }
    double GetMJD() const { return GetModifiedJulianDayNumber(); }

        // get the Rata Die number
    double GetRataDie() const;

        // TODO algorithms for calculating some important dates, such as
        //      religious holidays (Easter...) or moon/solar eclipses? Some
        //      algorithms can be found in the calendar FAQ


    // Timezone stuff: a wxDateTime object constructed using given
    // day/month/year/hour/min/sec values is interpreted as this moment in
    // local time. Using the functions below, it may be converted to another
    // time zone (e.g., the Unix epoch is wxDateTime(1, Jan, 1970).ToGMT()).
    //
    // These functions try to handle DST internally, but there is no magical
    // way to know all rules for it in all countries in the world, so if the
    // program can handle it itself (or doesn't want to handle it at all for
    // whatever reason), the DST handling can be disabled with noDST.
    // ------------------------------------------------------------------------

        // transform to any given timezone
    inline wxDateTime ToTimezone(const TimeZone& tz, bool noDST = false) const;
    wxDateTime& MakeTimezone(const TimeZone& tz, bool noDST = false);

        // interpret current value as being in another timezone and transform
        // it to local one
    inline wxDateTime FromTimezone(const TimeZone& tz, bool noDST = false) const;
    wxDateTime& MakeFromTimezone(const TimeZone& tz, bool noDST = false);

        // transform to/from GMT/UTC
    wxDateTime ToUTC(bool noDST = false) const { return ToTimezone(UTC, noDST); }
    wxDateTime& MakeUTC(bool noDST = false) { return MakeTimezone(UTC, noDST); }

    wxDateTime ToGMT(bool noDST = false) const { return ToUTC(noDST); }
    wxDateTime& MakeGMT(bool noDST = false) { return MakeUTC(noDST); }

    wxDateTime FromUTC(bool noDST = false) const
        { return FromTimezone(UTC, noDST); }
    wxDateTime& MakeFromUTC(bool noDST = false)
        { return MakeFromTimezone(UTC, noDST); }

        // is daylight savings time in effect at this moment according to the
        // rules of the specified country?
        //
        // Return value is > 0 if DST is in effect, 0 if it is not and -1 if
        // the information is not available (this is compatible with ANSI C)
    int IsDST(Country country = Country_Default) const;


    // accessors: many of them take the timezone parameter which indicates the
    // timezone for which to make the calculations and the default value means
    // to do it for the current timezone of this machine (even if the function
    // only operates with the date it's necessary because a date may wrap as
    // result of timezone shift)
    // ------------------------------------------------------------------------

        // is the date valid?
    inline bool IsValid() const { return m_time != wxLongLong(wxINT64_MIN); }

        // get the broken down date/time representation in the given timezone
        //
        // If you wish to get several time components (day, month and year),
        // consider getting the whole Tm structure first and retrieving the
        // value from it - this is much more efficient
    Tm GetTm(const TimeZone& tz = Local) const;

        // get the number of seconds since the Unix epoch - returns (time_t)-1
        // if the value is out of range
    inline time_t GetTicks() const;

        // get the century, same as GetCentury(GetYear())
    int GetCentury(const TimeZone& tz = Local) const
            { return GetCentury(GetYear(tz)); }
        // get the year (returns Inv_Year if date is invalid)
    int GetYear(const TimeZone& tz = Local) const
            { return GetTm(tz).year; }
        // get the month (Inv_Month if date is invalid)
    Month GetMonth(const TimeZone& tz = Local) const
            { return (Month)GetTm(tz).mon; }
        // get the month day (in 1..31 range, 0 if date is invalid)
    wxDateTime_t GetDay(const TimeZone& tz = Local) const
            { return GetTm(tz).mday; }
        // get the day of the week (Inv_WeekDay if date is invalid)
    WeekDay GetWeekDay(const TimeZone& tz = Local) const
            { return GetTm(tz).GetWeekDay(); }
        // get the hour of the day
    wxDateTime_t GetHour(const TimeZone& tz = Local) const
            { return GetTm(tz).hour; }
        // get the minute
    wxDateTime_t GetMinute(const TimeZone& tz = Local) const
            { return GetTm(tz).min; }
        // get the second
    wxDateTime_t GetSecond(const TimeZone& tz = Local) const
            { return GetTm(tz).sec; }
        // get milliseconds
    wxDateTime_t GetMillisecond(const TimeZone& tz = Local) const
            { return GetTm(tz).msec; }

        // get the day since the year start (1..366, 0 if date is invalid)
    wxDateTime_t GetDayOfYear(const TimeZone& tz = Local) const;
        // get the week number since the year start (1..52 or 53, 0 if date is
        // invalid)
    wxDateTime_t GetWeekOfYear(WeekFlags flags = Monday_First,
                               const TimeZone& tz = Local) const;
        // get the year to which the number returned from GetWeekOfYear()
        // belongs
    int GetWeekBasedYear(const TimeZone& tz = Local) const;
        // get the week number since the month start (1..5, 0 if date is
        // invalid)
    wxDateTime_t GetWeekOfMonth(WeekFlags flags = Monday_First,
                                const TimeZone& tz = Local) const;

        // is this date a work day? This depends on a country, of course,
        // because the holidays are different in different countries
    bool IsWorkDay(Country country = Country_Default) const;

    // dos date and time format
    // ------------------------------------------------------------------------

        // set from the DOS packed format
    wxDateTime& SetFromDOS(unsigned long ddt);

        // pack the date in DOS format
    unsigned long GetAsDOS() const;

    // SYSTEMTIME format
    // ------------------------------------------------------------------------
#ifdef __WINDOWS__
    // convert SYSTEMTIME to wxDateTime
    wxDateTime& SetFromMSWSysTime(const struct _SYSTEMTIME& st);

    // convert wxDateTime to SYSTEMTIME
    void GetAsMSWSysTime(struct _SYSTEMTIME* st) const;

    // same as above but only take date part into account, time is always zero
    wxDateTime& SetFromMSWSysDate(const struct _SYSTEMTIME& st);
    void GetAsMSWSysDate(struct _SYSTEMTIME* st) const;
#endif // __WINDOWS__

    // comparison (see also functions below for operator versions)
    // ------------------------------------------------------------------------

        // returns true if the two moments are strictly identical
    inline bool IsEqualTo(const wxDateTime& datetime) const;

        // returns true if the date is strictly earlier than the given one
    inline bool IsEarlierThan(const wxDateTime& datetime) const;

        // returns true if the date is strictly later than the given one
    inline bool IsLaterThan(const wxDateTime& datetime) const;

        // returns true if the date is strictly in the given range
    inline bool IsStrictlyBetween(const wxDateTime& t1,
                                  const wxDateTime& t2) const;

        // returns true if the date is in the given range
    inline bool IsBetween(const wxDateTime& t1, const wxDateTime& t2) const;

        // do these two objects refer to the same date?
    inline bool IsSameDate(const wxDateTime& dt) const;

        // do these two objects have the same time?
    inline bool IsSameTime(const wxDateTime& dt) const;

        // are these two objects equal up to given timespan?
    inline bool IsEqualUpTo(const wxDateTime& dt, const wxTimeSpan& ts) const;

    inline bool operator<(const wxDateTime& dt) const
    {
        return GetValue() < dt.GetValue();
    }

    inline bool operator<=(const wxDateTime& dt) const
    {
        return GetValue() <= dt.GetValue();
    }

    inline bool operator>(const wxDateTime& dt) const
    {
        return GetValue() > dt.GetValue();
    }

    inline bool operator>=(const wxDateTime& dt) const
    {
        return GetValue() >= dt.GetValue();
    }

    inline bool operator==(const wxDateTime& dt) const
    {
        // Intentionally do not call GetValue() here, in order that
        // invalid wxDateTimes may be compared for equality
        return m_time == dt.m_time;
    }

    inline bool operator!=(const wxDateTime& dt) const
    {
        // As above, don't use GetValue() here.
        return m_time != dt.m_time;
    }

    // arithmetic with dates (see also below for more operators)
    // ------------------------------------------------------------------------

        // return the sum of the date with a time span (positive or negative)
    inline wxDateTime Add(const wxTimeSpan& diff) const;
        // add a time span (positive or negative)
    inline wxDateTime& Add(const wxTimeSpan& diff);
        // add a time span (positive or negative)
    inline wxDateTime& operator+=(const wxTimeSpan& diff);
    inline wxDateTime operator+(const wxTimeSpan& ts) const
    {
        wxDateTime dt(*this);
        dt.Add(ts);
        return dt;
    }

        // return the difference of the date with a time span
    inline wxDateTime Subtract(const wxTimeSpan& diff) const;
        // subtract a time span (positive or negative)
    inline wxDateTime& Subtract(const wxTimeSpan& diff);
        // subtract a time span (positive or negative)
    inline wxDateTime& operator-=(const wxTimeSpan& diff);
    inline wxDateTime operator-(const wxTimeSpan& ts) const
    {
        wxDateTime dt(*this);
        dt.Subtract(ts);
        return dt;
    }

        // return the sum of the date with a date span
    inline wxDateTime Add(const wxDateSpan& diff) const;
        // add a date span (positive or negative)
    wxDateTime& Add(const wxDateSpan& diff);
        // add a date span (positive or negative)
    inline wxDateTime& operator+=(const wxDateSpan& diff);
    inline wxDateTime operator+(const wxDateSpan& ds) const
    {
        wxDateTime dt(*this);
        dt.Add(ds);
        return dt;
    }

        // return the difference of the date with a date span
    inline wxDateTime Subtract(const wxDateSpan& diff) const;
        // subtract a date span (positive or negative)
    inline wxDateTime& Subtract(const wxDateSpan& diff);
        // subtract a date span (positive or negative)
    inline wxDateTime& operator-=(const wxDateSpan& diff);
    inline wxDateTime operator-(const wxDateSpan& ds) const
    {
        wxDateTime dt(*this);
        dt.Subtract(ds);
        return dt;
    }

        // return the difference between two dates
    inline wxTimeSpan Subtract(const wxDateTime& dt) const;
    inline wxTimeSpan operator-(const wxDateTime& dt2) const;

    wxDateSpan DiffAsDateSpan(const wxDateTime& dt) const;

    // conversion to/from text
    // ------------------------------------------------------------------------

    // all conversions functions return true to indicate whether parsing
    // succeeded or failed and fill in the provided end iterator, which must
    // not be NULL, with the location of the character where the parsing
    // stopped (this will be end() of the passed string if everything was
    // parsed)

        // parse a string in RFC 822 format (found e.g. in mail headers and
        // having the form "Wed, 10 Feb 1999 19:07:07 +0100")
    bool ParseRfc822Date(const wxString& date,
                         wxString::const_iterator *end);

        // parse a date/time in the given format (see strptime(3)), fill in
        // the missing (in the string) fields with the values of dateDef (by
        // default, they will not change if they had valid values or will
        // default to Today() otherwise)
    bool ParseFormat(const wxString& date,
                     const wxString& format,
                     const wxDateTime& dateDef,
                     wxString::const_iterator *end);

    bool ParseFormat(const wxString& date,
                     const wxString& format,
                     wxString::const_iterator *end)
    {
        return ParseFormat(date, format, wxDefaultDateTime, end);
    }

    bool ParseFormat(const wxString& date,
                     wxString::const_iterator *end)
    {
        return ParseFormat(date, wxASCII_STR(wxDefaultDateTimeFormat), wxDefaultDateTime, end);
    }

        // parse a string containing date, time or both in ISO 8601 format
        //
        // notice that these functions are new in wx 3.0 and so we don't
        // provide compatibility overloads for them
    bool ParseISODate(const wxString& date)
    {
        wxString::const_iterator end;
        return ParseFormat(date, wxS("%Y-%m-%d"), &end) && end == date.end();
    }

    bool ParseISOTime(const wxString& time)
    {
        wxString::const_iterator end;
        return ParseFormat(time, wxS("%H:%M:%S"), &end) && end == time.end();
    }

    bool ParseISOCombined(const wxString& datetime, char sep = 'T')
    {
        wxString::const_iterator end;
        const wxString fmt = wxS("%Y-%m-%d") + wxString(sep) + wxS("%H:%M:%S");
        return ParseFormat(datetime, fmt, &end) && end == datetime.end();
    }

        // parse a string containing the date/time in "free" format, this
        // function will try to make an educated guess at the string contents
    bool ParseDateTime(const wxString& datetime,
                       wxString::const_iterator *end);

        // parse a string containing the date only in "free" format (less
        // flexible than ParseDateTime)
    bool ParseDate(const wxString& date,
                   wxString::const_iterator *end);

        // parse a string containing the time only in "free" format
    bool ParseTime(const wxString& time,
                   wxString::const_iterator *end);


        // this function accepts strftime()-like format string (default
        // argument corresponds to the preferred date and time representation
        // for the current locale) and returns the string containing the
        // resulting text representation
    wxString Format(const wxString& format = wxASCII_STR(wxDefaultDateTimeFormat),
                    const TimeZone& tz = Local) const;
        // preferred date representation for the current locale
    wxString FormatDate() const { return Format(wxS("%x")); }
        // preferred time representation for the current locale
    wxString FormatTime() const { return Format(wxS("%X")); }
        // returns the string representing the date in ISO 8601 format
        // (YYYY-MM-DD)
    wxString FormatISODate() const { return Format(wxS("%Y-%m-%d")); }
        // returns the string representing the time in ISO 8601 format
        // (HH:MM:SS)
    wxString FormatISOTime() const { return Format(wxS("%H:%M:%S")); }
        // return the combined date time representation in ISO 8601 format; the
        // separator character should be 'T' according to the standard but it
        // can also be useful to set it to ' '
    wxString FormatISOCombined(char sep = 'T') const
        { return FormatISODate() + sep + FormatISOTime(); }


    // backwards compatible versions of the parsing functions: they return an
    // object representing the next character following the date specification
    // (i.e. the one where the scan had to stop) or a special NULL-like object
    // on failure
    //
    // they're not deprecated because a lot of existing code uses them and
    // there is no particular harm in keeping them but you should still prefer
    // the versions above in the new code
    wxAnyStrPtr ParseRfc822Date(const wxString& date)
    {
        wxString::const_iterator end;
        return ParseRfc822Date(date, &end) ? wxAnyStrPtr(date, end)
                                           : wxAnyStrPtr();
    }

    wxAnyStrPtr ParseFormat(const wxString& date,
                            const wxString& format = wxASCII_STR(wxDefaultDateTimeFormat),
                            const wxDateTime& dateDef = wxDefaultDateTime)
    {
        wxString::const_iterator end;
        return ParseFormat(date, format, dateDef, &end) ? wxAnyStrPtr(date, end)
                                                        : wxAnyStrPtr();
    }

    wxAnyStrPtr ParseDateTime(const wxString& datetime)
    {
        wxString::const_iterator end;
        return ParseDateTime(datetime, &end) ? wxAnyStrPtr(datetime, end)
                                             : wxAnyStrPtr();
    }

    wxAnyStrPtr ParseDate(const wxString& date)
    {
        wxString::const_iterator end;
        return ParseDate(date, &end) ? wxAnyStrPtr(date, end)
                                     : wxAnyStrPtr();
    }

    wxAnyStrPtr ParseTime(const wxString& time)
    {
        wxString::const_iterator end;
        return ParseTime(time, &end) ? wxAnyStrPtr(time, end)
                                     : wxAnyStrPtr();
    }

    // In addition to wxAnyStrPtr versions above we also must provide the
    // overloads for C strings as we must return a pointer into the original
    // string and not inside a temporary wxString which would have been created
    // if the overloads above were used.
    //
    // And then we also have to provide the overloads for wxCStrData, as usual.
    // Unfortunately those can't return anything as we don't have any
    // sufficiently long-lived wxAnyStrPtr to return from them: any temporary
    // strings it would point to would be destroyed when this function returns
    // making it impossible to dereference the return value. So we just don't
    // return anything from here which at least allows to keep compatibility
    // with the code not testing the return value. Other uses of this method
    // need to be converted to use one of the new bool-returning overloads
    // above.
    void ParseRfc822Date(const wxCStrData& date)
        { ParseRfc822Date(wxString(date)); }
    const char* ParseRfc822Date(const char* date);
    const wchar_t* ParseRfc822Date(const wchar_t* date);

    void ParseFormat(const wxCStrData& date,
                     const wxString& format = wxASCII_STR(wxDefaultDateTimeFormat),
                     const wxDateTime& dateDef = wxDefaultDateTime)
        { ParseFormat(wxString(date), format, dateDef); }
    const char* ParseFormat(const char* date,
                            const wxString& format = wxASCII_STR(wxDefaultDateTimeFormat),
                            const wxDateTime& dateDef = wxDefaultDateTime);
    const wchar_t* ParseFormat(const wchar_t* date,
                               const wxString& format = wxASCII_STR(wxDefaultDateTimeFormat),
                               const wxDateTime& dateDef = wxDefaultDateTime);

    void ParseDateTime(const wxCStrData& datetime)
        { ParseDateTime(wxString(datetime)); }
    const char* ParseDateTime(const char* datetime);
    const wchar_t* ParseDateTime(const wchar_t* datetime);

    void ParseDate(const wxCStrData& date)
        { ParseDate(wxString(date)); }
    const char* ParseDate(const char* date);
    const wchar_t* ParseDate(const wchar_t* date);

    void ParseTime(const wxCStrData& time)
        { ParseTime(wxString(time)); }
    const char* ParseTime(const char* time);
    const wchar_t* ParseTime(const wchar_t* time);


    // implementation
    // ------------------------------------------------------------------------

        // construct from internal representation
    wxDateTime(const wxLongLong& time) : m_time(time) { }

        // get the internal representation
    inline wxLongLong GetValue() const;

    // a helper function to get the current time_t
    static time_t GetTimeNow() { return time(NULL); }

    // another one to get the current time broken down
    static struct tm *GetTmNow()
    {
        static struct tm l_CurrentTime;
        return GetTmNow(&l_CurrentTime);
    }

    // get current time using thread-safe function
    static struct tm *GetTmNow(struct tm *tmstruct);

private:
    // the current country - as it's the same for all program objects (unless
    // it runs on a _really_ big cluster system :-), this is a static member:
    // see SetCountry() and GetCountry()
    static Country ms_country;

    // this constant is used to transform a time_t value to the internal
    // representation, as time_t is in seconds and we use milliseconds it's
    // fixed to 1000
    static const long TIME_T_FACTOR;

    // returns true if we fall in range in which we can use standard ANSI C
    // functions
    inline bool IsInStdRange() const;

    // assign the preferred first day of a week to flags, if necessary
    void UseEffectiveWeekDayFlags(WeekFlags &flags) const;

    // parse time zone (e.g. "+0100") between [iterator,dateEnd)
    bool ParseRFC822TimeZone(wxString::const_iterator* iterator,
                             const wxString::const_iterator& dateEnd);

    // the internal representation of the time is the amount of milliseconds
    // elapsed since the origin which is set by convention to the UNIX/C epoch
    // value: the midnight of January 1, 1970 (UTC)
    wxLongLong m_time;
};

// ----------------------------------------------------------------------------
// This class contains a difference between 2 wxDateTime values, so it makes
// sense to add it to wxDateTime and it is the result of subtraction of 2
// objects of that class. See also wxDateSpan.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxTimeSpan
{
public:
    // constructors
    // ------------------------------------------------------------------------

        // return the timespan for the given number of milliseconds
    static wxTimeSpan Milliseconds(wxLongLong ms) { return wxTimeSpan(0, 0, 0, ms); }
    static wxTimeSpan Millisecond() { return Milliseconds(1); }

        // return the timespan for the given number of seconds
    static wxTimeSpan Seconds(wxLongLong sec) { return wxTimeSpan(0, 0, sec); }
    static wxTimeSpan Second() { return Seconds(1); }

        // return the timespan for the given number of minutes
    static wxTimeSpan Minutes(long min) { return wxTimeSpan(0, min, 0 ); }
    static wxTimeSpan Minute() { return Minutes(1); }

        // return the timespan for the given number of hours
    static wxTimeSpan Hours(long hours) { return wxTimeSpan(hours, 0, 0); }
    static wxTimeSpan Hour() { return Hours(1); }

        // return the timespan for the given number of days
    static wxTimeSpan Days(long days) { return Hours(24 * days); }
    static wxTimeSpan Day() { return Days(1); }

        // return the timespan for the given number of weeks
    static wxTimeSpan Weeks(long days) { return Days(7 * days); }
    static wxTimeSpan Week() { return Weeks(1); }

        // default ctor constructs the 0 time span
    wxTimeSpan() { }

        // from separate values for each component, date set to 0 (hours are
        // not restricted to 0..24 range, neither are minutes, seconds or
        // milliseconds)
    inline wxTimeSpan(long hours,
                      long minutes = 0,
                      wxLongLong seconds = 0,
                      wxLongLong milliseconds = 0);

        // default copy ctor is ok

        // no dtor

    // arithmetic with time spans (see also below for more operators)
    // ------------------------------------------------------------------------

        // return the sum of two timespans
    inline wxTimeSpan Add(const wxTimeSpan& diff) const;
        // add two timespans together
    inline wxTimeSpan& Add(const wxTimeSpan& diff);
        // add two timespans together
    wxTimeSpan& operator+=(const wxTimeSpan& diff) { return Add(diff); }
    inline wxTimeSpan operator+(const wxTimeSpan& ts) const
    {
        return wxTimeSpan(GetValue() + ts.GetValue());
    }

        // return the difference of two timespans
    inline wxTimeSpan Subtract(const wxTimeSpan& diff) const;
        // subtract another timespan
    inline wxTimeSpan& Subtract(const wxTimeSpan& diff);
        // subtract another timespan
    wxTimeSpan& operator-=(const wxTimeSpan& diff) { return Subtract(diff); }
    inline wxTimeSpan operator-(const wxTimeSpan& ts) const
    {
        return wxTimeSpan(GetValue() - ts.GetValue());
    }

        // multiply timespan by a scalar
    inline wxTimeSpan Multiply(int n) const;
        // multiply timespan by a scalar
    inline wxTimeSpan& Multiply(int n);
        // multiply timespan by a scalar
    wxTimeSpan& operator*=(int n) { return Multiply(n); }
    inline wxTimeSpan operator*(int n) const
    {
        return wxTimeSpan(*this).Multiply(n);
    }

        // return this timespan with opposite sign
    wxTimeSpan Negate() const { return wxTimeSpan(-GetValue()); }
        // negate the value of the timespan
    wxTimeSpan& Neg() { m_diff = -GetValue(); return *this; }
        // negate the value of the timespan
    wxTimeSpan& operator-() { return Neg(); }

        // return the absolute value of the timespan: does _not_ modify the
        // object
    inline wxTimeSpan Abs() const;

        // there is intentionally no division because we don't want to
        // introduce rounding errors in time calculations

    // comparison (see also operator versions below)
    // ------------------------------------------------------------------------

        // is the timespan null?
    bool IsNull() const { return m_diff == 0l; }
        // returns true if the timespan is null
    bool operator!() const { return !IsNull(); }

        // is the timespan positive?
    bool IsPositive() const { return m_diff > 0l; }

        // is the timespan negative?
    bool IsNegative() const { return m_diff < 0l; }

        // are two timespans equal?
    inline bool IsEqualTo(const wxTimeSpan& ts) const;
        // compare two timestamps: works with the absolute values, i.e. -2
        // hours is longer than 1 hour. Also, it will return false if the
        // timespans are equal in absolute value.
    inline bool IsLongerThan(const wxTimeSpan& ts) const;
        // compare two timestamps: works with the absolute values, i.e. 1
        // hour is shorter than -2 hours. Also, it will return false if the
        // timespans are equal in absolute value.
    bool IsShorterThan(const wxTimeSpan& t) const;

    inline bool operator<(const wxTimeSpan &ts) const
    {
        return GetValue() < ts.GetValue();
    }

    inline bool operator<=(const wxTimeSpan &ts) const
    {
        return GetValue() <= ts.GetValue();
    }

    inline bool operator>(const wxTimeSpan &ts) const
    {
        return GetValue() > ts.GetValue();
    }

    inline bool operator>=(const wxTimeSpan &ts) const
    {
        return GetValue() >= ts.GetValue();
    }

    inline bool operator==(const wxTimeSpan &ts) const
    {
        return GetValue() == ts.GetValue();
    }

    inline bool operator!=(const wxTimeSpan &ts) const
    {
        return GetValue() != ts.GetValue();
    }

    // breaking into days, hours, minutes and seconds
    // ------------------------------------------------------------------------

        // get the max number of weeks in this timespan
    inline int GetWeeks() const;
        // get the max number of days in this timespan
    inline int GetDays() const;
        // get the max number of hours in this timespan
    inline int GetHours() const;
        // get the max number of minutes in this timespan
    inline int GetMinutes() const;
        // get the max number of seconds in this timespan
    inline wxLongLong GetSeconds() const;
        // get the number of milliseconds in this timespan
    wxLongLong GetMilliseconds() const { return m_diff; }

    // conversion to text
    // ------------------------------------------------------------------------

        // this function accepts strftime()-like format string (default
        // argument corresponds to the preferred date and time representation
        // for the current locale) and returns the string containing the
        // resulting text representation. Notice that only some of format
        // specifiers valid for wxDateTime are valid for wxTimeSpan: hours,
        // minutes and seconds make sense, but not "PM/AM" string for example.
    wxString Format(const wxString& format = wxASCII_STR(wxDefaultTimeSpanFormat)) const;

    // implementation
    // ------------------------------------------------------------------------

        // construct from internal representation
    wxTimeSpan(const wxLongLong& diff) : m_diff(diff) { }

        // get the internal representation
    wxLongLong GetValue() const { return m_diff; }

private:
    // the (signed) time span in milliseconds
    wxLongLong m_diff;
};

// ----------------------------------------------------------------------------
// This class is a "logical time span" and is useful for implementing program
// logic for such things as "add one month to the date" which, in general,
// doesn't mean to add 60*60*24*31 seconds to it, but to take the same date
// the next month (to understand that this is indeed different consider adding
// one month to Feb, 15 - we want to get Mar, 15, of course).
//
// When adding a month to the date, all lesser components (days, hours, ...)
// won't be changed unless the resulting date would be invalid: for example,
// Jan 31 + 1 month will be Feb 28, not (non existing) Feb 31.
//
// Because of this feature, adding and subtracting back again the same
// wxDateSpan will *not*, in general give back the original date: Feb 28 - 1
// month will be Jan 28, not Jan 31!
//
// wxDateSpan can be either positive or negative. They may be
// multiplied by scalars which multiply all deltas by the scalar: i.e. 2*(1
// month and 1 day) is 2 months and 2 days. They can be added together and
// with wxDateTime or wxTimeSpan, but the type of result is different for each
// case.
//
// Beware about weeks: if you specify both weeks and days, the total number of
// days added will be 7*weeks + days! See also GetTotalDays() function.
//
// Equality operators are defined for wxDateSpans. Two datespans are equal if
// they both give the same target date when added to *every* source date.
// Thus wxDateSpan::Months(1) is not equal to wxDateSpan::Days(30), because
// they not give the same date when added to 1 Feb. But wxDateSpan::Days(14) is
// equal to wxDateSpan::Weeks(2)
//
// Finally, notice that for adding hours, minutes &c you don't need this
// class: wxTimeSpan will do the job because there are no subtleties
// associated with those.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxDateSpan
{
public:
    // constructors
    // ------------------------------------------------------------------------

        // this many years/months/weeks/days
    wxDateSpan(int years = 0, int months = 0, int weeks = 0, int days = 0)
    {
        m_years = years;
        m_months = months;
        m_weeks = weeks;
        m_days = days;
    }

        // get an object for the given number of days
    static wxDateSpan Days(int days) { return wxDateSpan(0, 0, 0, days); }
    static wxDateSpan Day() { return Days(1); }

        // get an object for the given number of weeks
    static wxDateSpan Weeks(int weeks) { return wxDateSpan(0, 0, weeks, 0); }
    static wxDateSpan Week() { return Weeks(1); }

        // get an object for the given number of months
    static wxDateSpan Months(int mon) { return wxDateSpan(0, mon, 0, 0); }
    static wxDateSpan Month() { return Months(1); }

        // get an object for the given number of years
    static wxDateSpan Years(int years) { return wxDateSpan(years, 0, 0, 0); }
    static wxDateSpan Year() { return Years(1); }

        // default copy ctor is ok

        // no dtor

    // accessors (all SetXXX() return the (modified) wxDateSpan object)
    // ------------------------------------------------------------------------

        // set number of years
    wxDateSpan& SetYears(int n) { m_years = n; return *this; }
        // set number of months
    wxDateSpan& SetMonths(int n) { m_months = n; return *this; }
        // set number of weeks
    wxDateSpan& SetWeeks(int n) { m_weeks = n; return *this; }
        // set number of days
    wxDateSpan& SetDays(int n) { m_days = n; return *this; }

        // get number of years
    int GetYears() const { return m_years; }
        // get number of months
    int GetMonths() const { return m_months; }
        // returns 12*GetYears() + GetMonths()
    int GetTotalMonths() const { return 12*m_years + m_months; }
        // get number of weeks
    int GetWeeks() const { return m_weeks; }
        // get number of days
    int GetDays() const { return m_days; }
        // returns 7*GetWeeks() + GetDays()
    int GetTotalDays() const { return 7*m_weeks + m_days; }

    // arithmetic with date spans (see also below for more operators)
    // ------------------------------------------------------------------------

        // return sum of two date spans
    inline wxDateSpan Add(const wxDateSpan& other) const;
        // add another wxDateSpan to us
    inline wxDateSpan& Add(const wxDateSpan& other);
        // add another wxDateSpan to us
    inline wxDateSpan& operator+=(const wxDateSpan& other);
    inline wxDateSpan operator+(const wxDateSpan& ds) const
    {
        return wxDateSpan(GetYears() + ds.GetYears(),
                          GetMonths() + ds.GetMonths(),
                          GetWeeks() + ds.GetWeeks(),
                          GetDays() + ds.GetDays());
    }

        // return difference of two date spans
    inline wxDateSpan Subtract(const wxDateSpan& other) const;
        // subtract another wxDateSpan from us
    inline wxDateSpan& Subtract(const wxDateSpan& other);
        // subtract another wxDateSpan from us
    inline wxDateSpan& operator-=(const wxDateSpan& other);
    inline wxDateSpan operator-(const wxDateSpan& ds) const
    {
        return wxDateSpan(GetYears() - ds.GetYears(),
                          GetMonths() - ds.GetMonths(),
                          GetWeeks() - ds.GetWeeks(),
                          GetDays() - ds.GetDays());
    }

        // return a copy of this time span with changed sign
    inline wxDateSpan Negate() const;
        // inverse the sign of this timespan
    inline wxDateSpan& Neg();
        // inverse the sign of this timespan
    wxDateSpan& operator-() { return Neg(); }

        // return the date span proportional to this one with given factor
    inline wxDateSpan Multiply(int factor) const;
        // multiply all components by a (signed) number
    inline wxDateSpan& Multiply(int factor);
        // multiply all components by a (signed) number
    inline wxDateSpan& operator*=(int factor) { return Multiply(factor); }
    inline wxDateSpan operator*(int n) const
    {
        return wxDateSpan(*this).Multiply(n);
    }

    // ds1 == d2 if and only if for every wxDateTime t t + ds1 == t + ds2
    inline bool operator==(const wxDateSpan& ds) const
    {
        return GetYears() == ds.GetYears() &&
               GetMonths() == ds.GetMonths() &&
               GetTotalDays() == ds.GetTotalDays();
    }

    inline bool operator!=(const wxDateSpan& ds) const
    {
        return !(*this == ds);
    }

private:
    int m_years,
        m_months,
        m_weeks,
        m_days;
};

// ----------------------------------------------------------------------------
// wxDateTimeArray: array of dates.
// ----------------------------------------------------------------------------

WX_DECLARE_USER_EXPORTED_OBJARRAY(wxDateTime, wxDateTimeArray, WXDLLIMPEXP_BASE);

// ----------------------------------------------------------------------------
// wxDateTimeHolidayAuthority: an object of this class will decide whether a
// given date is a holiday and is used by all functions working with "work
// days".
//
// NB: the base class is an ABC, derived classes must implement the pure
//     virtual methods to work with the holidays they correspond to.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_FWD_BASE wxDateTimeHolidayAuthority;
WX_DEFINE_USER_EXPORTED_ARRAY_PTR(wxDateTimeHolidayAuthority *,
                              wxHolidayAuthoritiesArray,
                              class WXDLLIMPEXP_BASE);

class wxDateTimeHolidaysModule;
class WXDLLIMPEXP_BASE wxDateTimeHolidayAuthority
{
friend class wxDateTimeHolidaysModule;
public:
    // returns true if the given date is a holiday
    static bool IsHoliday(const wxDateTime& dt);

    // fills the provided array with all holidays in the given range, returns
    // the number of them
    static size_t GetHolidaysInRange(const wxDateTime& dtStart,
                                     const wxDateTime& dtEnd,
                                     wxDateTimeArray& holidays);

    // clear the list of holiday authorities
    static void ClearAllAuthorities();

    // add a new holiday authority (the pointer will be deleted by
    // wxDateTimeHolidayAuthority)
    static void AddAuthority(wxDateTimeHolidayAuthority *auth);

    // the base class must have a virtual dtor
    virtual ~wxDateTimeHolidayAuthority();

protected:
    // this function is called to determine whether a given day is a holiday
    virtual bool DoIsHoliday(const wxDateTime& dt) const = 0;

    // this function should fill the array with all holidays between the two
    // given dates - it is implemented in the base class, but in a very
    // inefficient way (it just iterates over all days and uses IsHoliday() for
    // each of them), so it must be overridden in the derived class where the
    // base class version may be explicitly used if needed
    //
    // returns the number of holidays in the given range and fills holidays
    // array
    virtual size_t DoGetHolidaysInRange(const wxDateTime& dtStart,
                                        const wxDateTime& dtEnd,
                                        wxDateTimeArray& holidays) const = 0;

private:
    // all holiday authorities
    static wxHolidayAuthoritiesArray ms_authorities;
};

// the holidays for this class are all Saturdays and Sundays
class WXDLLIMPEXP_BASE wxDateTimeWorkDays : public wxDateTimeHolidayAuthority
{
protected:
    virtual bool DoIsHoliday(const wxDateTime& dt) const wxOVERRIDE;
    virtual size_t DoGetHolidaysInRange(const wxDateTime& dtStart,
                                        const wxDateTime& dtEnd,
                                        wxDateTimeArray& holidays) const wxOVERRIDE;
};

// ============================================================================
// inline functions implementation
// ============================================================================

// ----------------------------------------------------------------------------
// private macros
// ----------------------------------------------------------------------------

#define MILLISECONDS_PER_DAY 86400000l

// some broken compilers (HP-UX CC) refuse to compile the "normal" version, but
// using a temp variable always might prevent other compilers from optimising
// it away - hence use of this ugly macro
#ifndef __HPUX__
    #define MODIFY_AND_RETURN(op) return wxDateTime(*this).op
#else
    #define MODIFY_AND_RETURN(op) wxDateTime dt(*this); dt.op; return dt
#endif

// ----------------------------------------------------------------------------
// wxDateTime construction
// ----------------------------------------------------------------------------

inline bool wxDateTime::IsInStdRange() const
{
    // if sizeof(time_t) is greater than 32 bits, we assume it
    // is safe to return values exceeding wxINT32_MAX

    return m_time >= 0l &&
        ( (sizeof(time_t) > 4 ) || ( (m_time / TIME_T_FACTOR) < wxINT32_MAX) );
}

/* static */
inline wxDateTime wxDateTime::Now()
{
    struct tm tmstruct;
    return wxDateTime(*GetTmNow(&tmstruct));
}

/* static */
inline wxDateTime wxDateTime::Today()
{
    wxDateTime dt(Now());
    dt.ResetTime();

    return dt;
}

inline wxDateTime& wxDateTime::Set(time_t timet)
{
    if ( timet == (time_t)-1 )
    {
        m_time = wxInvalidDateTime.m_time;
    }
    else
    {
        // assign first to avoid long multiplication overflow!
        m_time = timet - WX_TIME_BASE_OFFSET;
        m_time *= TIME_T_FACTOR;
    }

    return *this;
}

inline wxDateTime& wxDateTime::SetToCurrent()
{
    *this = Now();
    return *this;
}

inline wxDateTime::wxDateTime(time_t timet)
{
    Set(timet);
}

inline wxDateTime::wxDateTime(const struct tm& tm)
{
    Set(tm);
}

inline wxDateTime::wxDateTime(const Tm& tm)
{
    Set(tm);
}

inline wxDateTime::wxDateTime(double jdn)
{
    Set(jdn);
}

inline wxDateTime& wxDateTime::Set(const Tm& tm)
{
    wxASSERT_MSG( tm.IsValid(), wxT("invalid broken down date/time") );

    return Set(tm.mday, (Month)tm.mon, tm.year,
               tm.hour, tm.min, tm.sec, tm.msec);
}

inline wxDateTime::wxDateTime(wxDateTime_t hour,
                              wxDateTime_t minute,
                              wxDateTime_t second,
                              wxDateTime_t millisec)
{
    Set(hour, minute, second, millisec);
}

inline wxDateTime::wxDateTime(wxDateTime_t day,
                              Month        month,
                              int          year,
                              wxDateTime_t hour,
                              wxDateTime_t minute,
                              wxDateTime_t second,
                              wxDateTime_t millisec)
{
    Set(day, month, year, hour, minute, second, millisec);
}

// ----------------------------------------------------------------------------
// wxDateTime accessors
// ----------------------------------------------------------------------------

inline wxLongLong wxDateTime::GetValue() const
{
    wxASSERT_MSG( IsValid(), wxT("invalid wxDateTime"));

    return m_time;
}

inline time_t wxDateTime::GetTicks() const
{
    wxASSERT_MSG( IsValid(), wxT("invalid wxDateTime"));
    if ( !IsInStdRange() )
    {
        return (time_t)-1;
    }

    return (time_t)((m_time / TIME_T_FACTOR).GetValue()) + WX_TIME_BASE_OFFSET;
}

inline bool wxDateTime::SetToLastWeekDay(WeekDay weekday,
                                         Month month,
                                         int year)
{
    return SetToWeekDay(weekday, -1, month, year);
}

inline wxDateTime
wxDateTime::GetWeekDayInSameWeek(WeekDay weekday,
                                 WeekFlags WXUNUSED(flags)) const
{
    MODIFY_AND_RETURN( SetToWeekDayInSameWeek(weekday) );
}

inline wxDateTime wxDateTime::GetNextWeekDay(WeekDay weekday) const
{
    MODIFY_AND_RETURN( SetToNextWeekDay(weekday) );
}

inline wxDateTime wxDateTime::GetPrevWeekDay(WeekDay weekday) const
{
    MODIFY_AND_RETURN( SetToPrevWeekDay(weekday) );
}

inline wxDateTime wxDateTime::GetWeekDay(WeekDay weekday,
                                         int n,
                                         Month month,
                                         int year) const
{
    wxDateTime dt(*this);

    return dt.SetToWeekDay(weekday, n, month, year) ? dt : wxInvalidDateTime;
}

inline wxDateTime wxDateTime::GetLastWeekDay(WeekDay weekday,
                                             Month month,
                                             int year)
{
    wxDateTime dt(*this);

    return dt.SetToLastWeekDay(weekday, month, year) ? dt : wxInvalidDateTime;
}

inline wxDateTime wxDateTime::GetLastMonthDay(Month month, int year) const
{
    MODIFY_AND_RETURN( SetToLastMonthDay(month, year) );
}

inline wxDateTime wxDateTime::GetYearDay(wxDateTime_t yday) const
{
    MODIFY_AND_RETURN( SetToYearDay(yday) );
}

// ----------------------------------------------------------------------------
// wxDateTime comparison
// ----------------------------------------------------------------------------

inline bool wxDateTime::IsEqualTo(const wxDateTime& datetime) const
{
    return *this == datetime;
}

inline bool wxDateTime::IsEarlierThan(const wxDateTime& datetime) const
{
    return *this < datetime;
}

inline bool wxDateTime::IsLaterThan(const wxDateTime& datetime) const
{
    return *this > datetime;
}

inline bool wxDateTime::IsStrictlyBetween(const wxDateTime& t1,
                                          const wxDateTime& t2) const
{
    // no need for assert, will be checked by the functions we call
    return IsLaterThan(t1) && IsEarlierThan(t2);
}

inline bool wxDateTime::IsBetween(const wxDateTime& t1,
                                  const wxDateTime& t2) const
{
    // no need for assert, will be checked by the functions we call
    return IsEqualTo(t1) || IsEqualTo(t2) || IsStrictlyBetween(t1, t2);
}

inline bool wxDateTime::IsSameDate(const wxDateTime& dt) const
{
    Tm tm1 = GetTm(),
       tm2 = dt.GetTm();

    return tm1.year == tm2.year &&
           tm1.mon == tm2.mon &&
           tm1.mday == tm2.mday;
}

inline bool wxDateTime::IsSameTime(const wxDateTime& dt) const
{
    // notice that we can't do something like this:
    //
    //    m_time % MILLISECONDS_PER_DAY == dt.m_time % MILLISECONDS_PER_DAY
    //
    // because we have also to deal with (possibly) different DST settings!
    Tm tm1 = GetTm(),
       tm2 = dt.GetTm();

    return tm1.hour == tm2.hour &&
           tm1.min == tm2.min &&
           tm1.sec == tm2.sec &&
           tm1.msec == tm2.msec;
}

inline bool wxDateTime::IsEqualUpTo(const wxDateTime& dt,
                                    const wxTimeSpan& ts) const
{
    return IsBetween(dt.Subtract(ts), dt.Add(ts));
}

// ----------------------------------------------------------------------------
// wxDateTime arithmetic
// ----------------------------------------------------------------------------

inline wxDateTime wxDateTime::Add(const wxTimeSpan& diff) const
{
    wxASSERT_MSG( IsValid(), wxT("invalid wxDateTime"));

    return wxDateTime(m_time + diff.GetValue());
}

inline wxDateTime& wxDateTime::Add(const wxTimeSpan& diff)
{
    wxASSERT_MSG( IsValid(), wxT("invalid wxDateTime"));

    m_time += diff.GetValue();

    return *this;
}

inline wxDateTime& wxDateTime::operator+=(const wxTimeSpan& diff)
{
    return Add(diff);
}

inline wxDateTime wxDateTime::Subtract(const wxTimeSpan& diff) const
{
    wxASSERT_MSG( IsValid(), wxT("invalid wxDateTime"));

    return wxDateTime(m_time - diff.GetValue());
}

inline wxDateTime& wxDateTime::Subtract(const wxTimeSpan& diff)
{
    wxASSERT_MSG( IsValid(), wxT("invalid wxDateTime"));

    m_time -= diff.GetValue();

    return *this;
}

inline wxDateTime& wxDateTime::operator-=(const wxTimeSpan& diff)
{
    return Subtract(diff);
}

inline wxTimeSpan wxDateTime::Subtract(const wxDateTime& datetime) const
{
    wxASSERT_MSG( IsValid() && datetime.IsValid(), wxT("invalid wxDateTime"));

    return wxTimeSpan(GetValue() - datetime.GetValue());
}

inline wxTimeSpan wxDateTime::operator-(const wxDateTime& dt2) const
{
    return this->Subtract(dt2);
}

inline wxDateTime wxDateTime::Add(const wxDateSpan& diff) const
{
    return wxDateTime(*this).Add(diff);
}

inline wxDateTime& wxDateTime::Subtract(const wxDateSpan& diff)
{
    return Add(diff.Negate());
}

inline wxDateTime wxDateTime::Subtract(const wxDateSpan& diff) const
{
    return wxDateTime(*this).Subtract(diff);
}

inline wxDateTime& wxDateTime::operator-=(const wxDateSpan& diff)
{
    return Subtract(diff);
}

inline wxDateTime& wxDateTime::operator+=(const wxDateSpan& diff)
{
    return Add(diff);
}

// ----------------------------------------------------------------------------
// wxDateTime and timezones
// ----------------------------------------------------------------------------

inline wxDateTime
wxDateTime::ToTimezone(const wxDateTime::TimeZone& tz, bool noDST) const
{
    MODIFY_AND_RETURN( MakeTimezone(tz, noDST) );
}

inline wxDateTime
wxDateTime::FromTimezone(const wxDateTime::TimeZone& tz, bool noDST) const
{
    MODIFY_AND_RETURN( MakeFromTimezone(tz, noDST) );
}

// ----------------------------------------------------------------------------
// wxTimeSpan construction
// ----------------------------------------------------------------------------

inline wxTimeSpan::wxTimeSpan(long hours,
                              long minutes,
                              wxLongLong seconds,
                              wxLongLong milliseconds)
{
    // assign first to avoid precision loss
    m_diff = hours;
    m_diff *= 60l;
    m_diff += minutes;
    m_diff *= 60l;
    m_diff += seconds;
    m_diff *= 1000l;
    m_diff += milliseconds;
}

// ----------------------------------------------------------------------------
// wxTimeSpan accessors
// ----------------------------------------------------------------------------

inline wxLongLong wxTimeSpan::GetSeconds() const
{
    return m_diff / 1000l;
}

inline int wxTimeSpan::GetMinutes() const
{
    // For compatibility, this method (and the other accessors) return int,
    // even though GetLo() actually returns unsigned long with greater range.
    return static_cast<int>((GetSeconds() / 60l).GetLo());
}

inline int wxTimeSpan::GetHours() const
{
    return GetMinutes() / 60;
}

inline int wxTimeSpan::GetDays() const
{
    return GetHours() / 24;
}

inline int wxTimeSpan::GetWeeks() const
{
    return GetDays() / 7;
}

// ----------------------------------------------------------------------------
// wxTimeSpan arithmetic
// ----------------------------------------------------------------------------

inline wxTimeSpan wxTimeSpan::Add(const wxTimeSpan& diff) const
{
    return wxTimeSpan(m_diff + diff.GetValue());
}

inline wxTimeSpan& wxTimeSpan::Add(const wxTimeSpan& diff)
{
    m_diff += diff.GetValue();

    return *this;
}

inline wxTimeSpan wxTimeSpan::Subtract(const wxTimeSpan& diff) const
{
    return wxTimeSpan(m_diff - diff.GetValue());
}

inline wxTimeSpan& wxTimeSpan::Subtract(const wxTimeSpan& diff)
{
    m_diff -= diff.GetValue();

    return *this;
}

inline wxTimeSpan& wxTimeSpan::Multiply(int n)
{
    m_diff *= (long)n;

    return *this;
}

inline wxTimeSpan wxTimeSpan::Multiply(int n) const
{
    return wxTimeSpan(m_diff * (long)n);
}

inline wxTimeSpan wxTimeSpan::Abs() const
{
    return wxTimeSpan(GetValue().Abs());
}

inline bool wxTimeSpan::IsEqualTo(const wxTimeSpan& ts) const
{
    return GetValue() == ts.GetValue();
}

inline bool wxTimeSpan::IsLongerThan(const wxTimeSpan& ts) const
{
    return GetValue().Abs() > ts.GetValue().Abs();
}

inline bool wxTimeSpan::IsShorterThan(const wxTimeSpan& ts) const
{
    return GetValue().Abs() < ts.GetValue().Abs();
}

// ----------------------------------------------------------------------------
// wxDateSpan
// ----------------------------------------------------------------------------

inline wxDateSpan& wxDateSpan::operator+=(const wxDateSpan& other)
{
    m_years += other.m_years;
    m_months += other.m_months;
    m_weeks += other.m_weeks;
    m_days += other.m_days;

    return *this;
}

inline wxDateSpan& wxDateSpan::Add(const wxDateSpan& other)
{
    return *this += other;
}

inline wxDateSpan wxDateSpan::Add(const wxDateSpan& other) const
{
    wxDateSpan ds(*this);
    ds.Add(other);
    return ds;
}

inline wxDateSpan& wxDateSpan::Multiply(int factor)
{
    m_years *= factor;
    m_months *= factor;
    m_weeks *= factor;
    m_days *= factor;

    return *this;
}

inline wxDateSpan wxDateSpan::Multiply(int factor) const
{
    wxDateSpan ds(*this);
    ds.Multiply(factor);
    return ds;
}

inline wxDateSpan wxDateSpan::Negate() const
{
    return wxDateSpan(-m_years, -m_months, -m_weeks, -m_days);
}

inline wxDateSpan& wxDateSpan::Neg()
{
    m_years = -m_years;
    m_months = -m_months;
    m_weeks = -m_weeks;
    m_days = -m_days;

    return *this;
}

inline wxDateSpan& wxDateSpan::operator-=(const wxDateSpan& other)
{
    return *this += other.Negate();
}

inline wxDateSpan& wxDateSpan::Subtract(const wxDateSpan& other)
{
    return *this -= other;
}

inline wxDateSpan wxDateSpan::Subtract(const wxDateSpan& other) const
{
    wxDateSpan ds(*this);
    ds.Subtract(other);
    return ds;
}

#undef MILLISECONDS_PER_DAY

#undef MODIFY_AND_RETURN

// ============================================================================
// binary operators
// ============================================================================

// ----------------------------------------------------------------------------
// wxTimeSpan operators
// ----------------------------------------------------------------------------

wxTimeSpan WXDLLIMPEXP_BASE operator*(int n, const wxTimeSpan& ts);

// ----------------------------------------------------------------------------
// wxDateSpan
// ----------------------------------------------------------------------------

wxDateSpan WXDLLIMPEXP_BASE operator*(int n, const wxDateSpan& ds);

// ============================================================================
// other helper functions
// ============================================================================

// ----------------------------------------------------------------------------
// iteration helpers: can be used to write a for loop over enum variable like
// this:
//  for ( m = wxDateTime::Jan; m < wxDateTime::Inv_Month; wxNextMonth(m) )
// ----------------------------------------------------------------------------

WXDLLIMPEXP_BASE void wxNextMonth(wxDateTime::Month& m);
WXDLLIMPEXP_BASE void wxPrevMonth(wxDateTime::Month& m);
WXDLLIMPEXP_BASE void wxNextWDay(wxDateTime::WeekDay& wd);
WXDLLIMPEXP_BASE void wxPrevWDay(wxDateTime::WeekDay& wd);

#endif // wxUSE_DATETIME

#endif // _WX_DATETIME_H
