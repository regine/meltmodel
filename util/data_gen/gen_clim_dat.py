#!/usr/bin/env python
# Copyright 2012 Lyman Gillispie
# This code is distributed under the GPL License 
# Author: Lyman Gillispie


class Error(Exception):
    pass

class DateConflict(Error):
    def __init__(self,value):
        self.value=value
    def __str__(self):
        return repr(self.value)


def sim_clim_data(beg_year, end_year, jd_beg_day, jd_end_day, hours, temp,
                  precip, discharge, tab_len = 4):
    this_tab = tab_len*' '
    if beg_year > end_year:
        raise DateConflict('beg_year > end_year')
    elif beg_year == end_year and jd_beg_day > jd_end_day:
        raise DateConflict('start date is after end date')
    else:
        pass

    print('year, day, hour, temp, precip (mm),    Q(m3/s)\n')

    for year in range(beg_year,end_year+1):
        # Start on the right day this year
        if year == beg_year:
            first_day = jd_beg_day
        else:
            first_day = 1

        # end on the right day this year 
        if year == end_year:
            last_day = jd_end_day
        else:
            last_day = 365

        for day in range(first_day,last_day+1):
            for time in hours:
                disp_strings = map(str,[year, day, time, temp, precip, discharge])
                print ( this_tab.join(disp_strings) )
    

def main():
    import argparse
    #Class for correct formatting of ranges in argument help
    class RangeChoiceHelpFormatter(argparse.HelpFormatter):
        def _metavar_formatter(self, action, default_metavar):
             if action.metavar is not None:
                 result = action.metavar
             elif action.choices is not None:
                 result = '{%s .. %s}' % (min(action.choices), max(action.choices))
             else:
                 result = default_metavar

             def this_format(tuple_size):
                if isinstance(result, tuple):
                    return result
                else:
                    return (result, ) * tuple_size

             return this_format

    parser = argparse.ArgumentParser(formatter_class = RangeChoiceHelpFormatter)
    args_formating = parser.add_argument_group("Time Formatting Options")
    args_formating.add_argument( "-f", "--formatclimdata", required=True,
            type=int, choices=[1,2,3],
            help=(' 1: midnight is 0 in the morning \n'
                '2:  midnight is 24 in the morning\n'
                '3: midnight is 24 at night') )

    args_hour_or_day = args_formating.add_mutually_exclusive_group(required=True)
    args_hour_or_day.add_argument("-dr", "--daily", action="store_true",
            help=('daily readings'))
    args_hour_or_day.add_argument("-hr", "--hourly", action="store_true",
            help=('hourly readings'))

    args_dates = parser.add_argument_group("Dates to be simulated")
    args_dates.add_argument("-by", "--beg_year", required=True, 
            type=int, choices=xrange(1500,3001),
            help=('first year to be output'))
    args_dates.add_argument("-bd", "--jd_beg_day", required=True,
            type=int, choices=xrange(1,366),
            help=('Julian day to begin on'))
    args_dates.add_argument("-ey", "--end_year", required=True,
            type=int, choices=xrange(1500,3001),
            help=('year to end in'))
    args_dates.add_argument("-ed", "--jd_end_day", required=True,
            type=int, choices=xrange(1,366),
            help=('Julian day to end on'))

    clim_data = parser.add_argument_group("Climate data")
    clim_data.add_argument('-t', '--temp', 
            type=float, default=0.0,
            help=('Temperature at all times'))
    clim_data.add_argument('-p', '--precip',
            type=float, default=0.0,
            help=('Constant precipitation, in mm'))
    clim_data.add_argument('-q', '--discharge',
            type=float, default=0.0,
            help=('Constant rate of discharge, in m^3/s'))


    args = parser.parse_args()

    #Determine what the hours of the day are

    if args.daily:
        if args.formatclimdata == 1:
            hours = [0]
        else:
            hours = [24]
    else:
        if args.formatclimdata == 1:
            hours = range(0,24)
        elif args.formatclimdata == 2:
            hours = range(1,24)
            hours.insert(0,24)
        elif args.formatclimdata ==3:
            hours = range(1,25)

    sim_clim_data(args.beg_year, args.end_year, 
            args.jd_beg_day, args.jd_end_day, hours,
            args.temp, args.precip, args.discharge)




if __name__ == '__main__':
    main()

