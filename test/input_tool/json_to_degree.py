# Copyright 2012 Lyman Gillispie
# This code is distrubuted under the GPL
# Author: Lyman Gillispie

class Error(Exception):
    pass

def main():
    import argparse
    import json
    
# Parse commandline options
    parser = argparse.ArgumentParser()

    args_files = parser.add_argument_group('Files')
    args_files.add_argument('input', type=str, 
            help='json equivalent of input.dat')
    args_files.add_argument('-o','--output', type=str, 
            default='input.dat',help='output filename')

#    parser.add_argument('-s', '--silent', action="store_true",
#            help='do not warn on overwrite')

    args = parser.parse_args()

# Open json config file and load the contents to a dictionary
    with open(args.input) as in_file:
        try:
          config = json.load(in_file)
        except ValueError as e:
            print "Json error, " + str(e) + " in {0}".format(in_file.name)
            return

# check if out_file exists    
#    if not args.silent:
#      try:
#         with open(args.output) as out_file:
#               print "{0} already exists. Overwrite? [y/N]".format(args.output)
#      except IOError: pass
    
    melt_mod_dict_to_dat(config)

    return
       
def melt_mod_dict_to_dat(config):
    '''Super ugly conversion method, 
    Args: config - a dictionary containing all of the appropriate elements
    Returns: currently prints to screen
    TODO: return a strin to be output to a file or to the screen
    '''
    first_part = '\n\n{daysscreenoutput}\n{inpath}\n{outpath}\n{jdbeg}  {yearbeg}\n{jdend}  {yearend}\n{disyes}\n{calcgridyes}\n\n{maxmeltstakes}\n{plusminus}\n{do_out}\n\n{shayes} {exkyes} {solyes} {diryes} {dir2yes} {difyes} {gloyes} {albyes} {swbyes} {linyes} {loutyes}\n\n{netyes} {senyes} {latyes} {raiyes} {enbyes} {melyes}  {ablyes} {surftempyes} {posyes} {ddfyes}\n{surfyes}\n{snowyes}\n{daysnow}\n{numbersnowdaysout}'
 
    print(first_part).format(**config)

    print " ".join(map(str, config['jdsurface']))
    print ""

    second_part = '{winterbalyes}\n{winterjdbeg}  {winterjdend}\n{summerbalyes}\n{summerjdbeg}  {summerjdend}\n{datesfromfileyes}\n{namedatesmassbal}\n{beltwidth}\n{snow2zeroeachyearyes}\n{snowfreeyes}\n\n{cumulmeltyes}\n{cm_or_m}\n{do_out_area}\n{outgridnumber}\n\n'

    print(second_part).format(**config)

    for grid in config['outgrids']:
        print (grid['name'] + " " + 
            " ".join(map(str, grid['location'])) + 
            " " + str(grid['outglobnet']) + '\n')

    third_part = '{methodinisnow}\n{methodsnowalbedo}\n{methodglobal}\n{methodlonginstation}\n{methodlongin}\n{methodsurftempglac}\n\n{methodturbul}\n{method_z0Te}\n{methodiceheat}\n{methodnegbal}\n\n{scalingyes}\n{gamma}\n{c_coefficient}\n\n{namedgm}\n{namedgmdrain}\n{namedgmglac}\n{namedgmslope}\n{namedgmaspect}\n{namedgmskyview}\n{namedgmfirn}\n{nameinitialsnow}\n{nameklima}\n\n{laenge}\n{breite}\n{reflongitude}\n{rowclim}\n{colclim}\n{climoutsideyes}  {heightclim}\n{gridsize}\n{timestep}\n\n{formatclimdata}\n{maxcol}\n{coltemp}\n{colhum}\n{colwind}\n{colglob}\n{colref}\n{colnet}\n{collongin}\n{collongout}\n{colprec}\n{colcloud}\n{coltempgradvarying}\n{coldis}\n\n{ERAtempshift}\n{ERAwindshift}\n\n{methodtempinterpol}\n{tempgrad}\n{tempscenario}\n{precscenario}\n'



    print(third_part).format(**config)

    print (str(config["monthtempgradyes"]) + " " +
    " ".join(map(str,config["monthtempgrad"])))
    print (str(config["monthtempscenyes"]) + " " +
    " ".join(map(str,config["monthtempscen"])))
    print (str(config["monthprecipscenyes"]) + " "+
    " ".join(map(str,config["monthprecipscen"])))
    print ''

    fourth_part ='{n_albfiles}\n{albsnow}\n{albslush}\n{albice}\n{albfirn}\n{albrock}\n{albmin}\n{snowalbincrease}\n{albiceproz}\n{ndstart}\n\n{split}\n{prozdiffuse}\n{trans}\n{ratio}\n{ratiodir2dir}\n{surftemplapserate}\n{directfromfile}\n{pathdirectfile}\n{daysdirect}\n{slopestation}\n\n{iterstep}\n{z0wice}\n{dividerz0T}\n{dividerz0snow}\n{z0proz}\n{icez0min:.10f}\n{icez0max}\n\n{methodprecipinterpol}\n{precgrad}\n{precgradhigh} {precgradelev}\n{preccorr}\n{snowmultiplierglacier}\n{snowmultiplierrock}\n{threshtemp}\n{onlyglacieryes}\n{glacierpart}\n\n{nameqcalc}\n{nodis}\n{firnkons}\n{snowkons}\n{icekons}\n{icekons}\n\n{qfirnstart}\n{qsnowstart}\n{qicestart}\n{qicestart}\n{qground}\n{jdstartr2diff}\n\n{disyesopt}\n{optkA}\n{startopt1}\n{stepopt1}\n{anzahlopt1}\n{optkB}\n{startopt2}\n{stepopt2}\n{anzahlopt2}\n{namematrix}\n\n{percolationyes}\n{slushformationyes}\n{densificationyes}\n{wetstartyes}\n{ndepths}\n{factinter}\n\n{thicknessfirst}\n{thicknessdeep}\n{depthdeep}\n{denssnow}\n{irrwatercontyes}\n{irrwatercont}\n\n{factsubsurfout}\n{offsetsubsurfout}\n\n{runoffyes} {superyes} {wateryes} {surfwateryes} {slushyes} {coldsnowyes} {coldtotyes}\n\n\n\n{ddmethod}\n{DDFice}\n{DDFsnow}\n\n{meltfactor}\n{radfactorice}\n{radfactorsnow}\n{debrisfactor}\n\n{coordinatesyes}'

    print(fourth_part).format(**config)

    for stake in config["stake_coords"]:
        print " ".join(map(str,stake))
    
    return

if __name__ == '__main__':
    main()


