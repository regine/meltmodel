#!/usr/bin/env python
# Copyright 2012 Lyman Gillispie
# This code is distrubuted under the GPL
# Author: Lyman Gillispie


class Error(Exception):
    pass

import sys

def query_yes_no(question, default="yes"):
    """Ask a yes/no question via raw_input() and return their answer.

    "question" is a string that is presented to the user.
    "default" is the presumed answer if the user just hits <Enter>.
        It must be "yes" (the default), "no" or None (meaning
        an answer is required of the user).

    The "answer" return value is one of "yes" or "no".
    """
    valid = {"yes":True,   "y":True,  "ye":True,
             "no":False,     "n":False}
    if default == None:
        prompt = " [y/n] "
    elif default == "yes":
        prompt = " [Y/n] "
    elif default == "no":
        prompt = " [y/N] "
    else:
        raise ValueError("invalid default answer: '%s'" % default)

    while True:
        sys.stdout.write(question + prompt)
        choice = raw_input().lower()
        if default is not None and choice == '':
            return valid[default]
        elif choice in valid:
            return valid[choice]
        else:
            sys.stdout.write("Please respond with 'yes' or 'no' "\
                             "(or 'y' or 'n').\n")

def jsontodat(input, output=None, silent=False):
    """Takes in a valid json input and prints, either to the screen
    or to stdout, a valid input.dat file
    Args:   input : the filename of our .json input
            output : filename for output, None results in being put to stdout
            silent : If true, does not warn on file overwrite
    """
    import json

    
    # Open json config file and load the contents to a dictionary
    with open(input) as in_file:
        try:
          config = json.load(in_file)
        except ValueError as e:
            print "Json error, " + str(e) + " in {0}".format(in_file.name)
            return

# Outputs config file (input.dat) to std out or to a file
    if output == None: 
        print melt_mod_dict_to_dat(config)
    else:
        if not silent:
            # check to see if the file exists
            try:
                out_file = open(output)
                out_file.close()
                choice = query_yes_no("{0}".format(output) +
                       " already exists. Overwrite?" ,default="no")
                if not choice:
                    return
            except IOError:
                pass    

        with open(output, 'w') as out_file:
            out_file.write(melt_mod_dict_to_dat(config))
        
    return

def melt_mod_dict_to_dat(config):
    '''Super ugly conversion method, 
    Args: config - a dictionary containing all of the appropriate elements
    Returns: outstring, a valid input.dat for the Hock melt model
    TODO: - change "+=" flow to a "list-append" and .join flow 
          - change format strings to that they fit on a line
    '''

    outstring = ''

    first_part = '\n\n{daysscreenoutput}\n{inpath}\n{outpath}\n{jdbeg}  {yearbeg}\n{jdend}  {yearend}\n{disyes}\n{calcgridyes}\n\n{maxmeltstakes}\n{plusminus}\n{do_out}\n\n{shayes} {exkyes} {solyes} {diryes} {dir2yes} {difyes} {gloyes} {albyes} {swbyes} {linyes} {loutyes}\n\n{netyes} {senyes} {latyes} {raiyes} {enbyes} {melyes} {ablyes} {surftempyes} {posyes} {ddfyes}\n{surfyes}\n{snowyes}\n{daysnow}\n{numbersnowdaysout}\n'
 
    outstring += (first_part).format(**config)

    outstring += " ".join(map(str, config['jdsurface'])) + "\n\n"

    second_part = '{winterbalyes}\n{winterjdbeg}  {winterjdend}\n{summerbalyes}\n{summerjdbeg}  {summerjdend}\n{datesfromfileyes}\n{namedatesmassbal}\n{beltwidth}\n{snow2zeroeachyearyes}\n{snowfreeyes}\n\n{cumulmeltyes}\n{cm_or_m}\n{do_out_area}\n{outgridnumber}\n\n\n'

    outstring += (second_part).format(**config)

    for grid in config['outgrids']:
        outstring +=  (grid['name'] + " " + 
            " ".join(map(str, grid['location'])) + 
            " " + str(grid['outglobnet']) + "\n") 
    
    outstring += "\n"

    third_part = '{methodinisnow}\n{methodsnowalbedo}\n{methodglobal}\n{methodlonginstation}\n{methodlongin}\n{methodsurftempglac}\n\n{methodturbul}\n{method_z0Te}\n{methodiceheat}\n{methodnegbal}\n\n{scalingyes}\n{gamma}\n{c_coefficient}\n\n{namedgm}\n{namedgmdrain}\n{namedgmglac}\n{namedgmslope}\n{namedgmaspect}\n{namedgmskyview}\n{namedgmfirn}\n{nameinitialsnow}\n{nameklima}\n\n{laenge}\n{breite}\n{reflongitude}\n{rowclim}\n{colclim}\n{climoutsideyes}  {heightclim}\n{gridsize}\n{timestep}\n\n{formatclimdata}\n{maxcol}\n{coltemp}\n{colhum}\n{colwind}\n{colglob}\n{colref}\n{colnet}\n{collongin}\n{collongout}\n{colprec}\n{colcloud}\n{coltempgradvarying}\n{coldis}\n\n{ERAtempshift}\n{ERAwindshift}\n\n{methodtempinterpol}\n{tempgrad}\n{tempscenario}\n{precscenario}\n\n'



    outstring += (third_part).format(**config)

    outstring +=  (str(config["monthtempgradyes"]) + " " +
    " ".join(map(str,config["monthtempgrad"])) + "\n")
    outstring +=  (str(config["monthtempscenyes"]) + " " +
    " ".join(map(str,config["monthtempscen"])) + "\n")
    outstring +=  (str(config["monthprecipscenyes"]) + " "+
    " ".join(map(str,config["monthprecipscen"])) + "\n")
    outstring += "\n"

    fourth_part ='{n_albfiles}\n{albsnow}\n{albslush}\n{albice}\n{albfirn}\n{albrock}\n{albmin}\n{snowalbincrease}\n{albiceproz}\n{ndstart}\n\n{split}\n{prozdiffuse}\n{trans}\n{ratio}\n{ratiodir2dir}\n{surftemplapserate}\n{directfromfile}\n{pathdirectfile}\n{daysdirect}\n{slopestation}\n\n{iterstep}\n{z0wice}\n{dividerz0T}\n{dividerz0snow}\n{z0proz}\n{icez0min:.10f}\n{icez0max}\n\n{methodprecipinterpol}\n{precgrad}\n{precgradhigh} {precgradelev}\n{preccorr}\n{snowmultiplierglacier}\n{snowmultiplierrock}\n{threshtemp}\n{onlyglacieryes}\n{glacierpart}\n\n{nameqcalc}\n{nodis}\n{firnkons}\n{snowkons}\n{icekons}\n{rockkons}\n\n{qfirnstart}\n{qsnowstart}\n{qicestart}\n{qicestart}\n{qground}\n{jdstartr2diff}\n\n{disyesopt}\n{optkA}\n{startopt1}\n{stepopt1}\n{anzahlopt1}\n{optkB}\n{startopt2}\n{stepopt2}\n{anzahlopt2}\n{namematrix}\n\n{percolationyes}\n{slushformationyes}\n{densificationyes}\n{wetstartyes}\n{ndepths}\n{factinter}\n\n{thicknessfirst}\n{thicknessdeep}\n{depthdeep}\n{denssnow}\n{irrwatercontyes}\n{irrwatercont}\n\n{factsubsurfout}\n{offsetsubsurfout}\n\n{runoffyes} {superyes} {wateryes} {surfwateryes} {slushyes} {coldsnowyes} {coldtotyes}\n\n\n\n{ddmethod}\n{DDFice}\n{DDFsnow}\n\n{meltfactor}\n{radfactorice}\n{radfactorsnow}\n{debrisfactor}\n\n{coordinatesyes}\n'

    outstring += (fourth_part).format(**config)

    for stake in config["stake_coords"]:
        outstring +=  (" ".join(map(str,stake)) + "\n")
    
    return outstring

def test_1():
    jsontodat("test/json_test_in.json", output="test/foo.txt", silent=True)
    with open("test/foo.txt") as file:
        gen_str = file.read()
    with open("test/json_test_out.txt",) as file:
        test_str = file.read()

    assert(test_str == gen_str)
    
    
   
def main():
    import argparse
    
# Parse commandline options
    parser = argparse.ArgumentParser(description='Convert json formatted ' +
            'config files to valid input.dat config files '+
            'for the Hock melt model.')

    parser.add_argument('input', type=str, 
            help='json equivalent of input.dat')
    parser.add_argument('-o','--output', type=str, 
            help='output filename, if unspecified output is sent to stdout')
 
    parser.add_argument('-s', '--silent', action="store_true",
            default=False,
            help='silently overwrite <output>, if it exists')

    args = parser.parse_args()
    jsontodat(args.input, args.output, args.silent)


if __name__ == '__main__':
    main()


