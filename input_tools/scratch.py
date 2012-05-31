def dict_from_native(nat_conf_file):

    #read from the native conf file and split at the newlinw
    old_conf = nat_conf_file.read().splitlines()
    new_config_dict = dict()


# execution_params: line 3-9 of nat_conf_file
    new_config_dict['execution_params'] = {'jdbeg': int( old_conf[5].split()[0] ),
        'jdend': int( old_conf[6].split()[0] ),
        'yearend': int( old_conf[6].split()[1] ),
        'daysscreenoutput': int( old_conf[2].split()[0] ),
        'disyes': int( old_conf[7].split()[0] ),
        'outpath': old_conf[4].split()[0] ,
        'calcgridyes': int( old_conf[8].split()[0] ), 
        'yearbeg': int( old_conf[5].split()[1] ),
        'inpath': old_conf[3].split()[0]}

    # output_params: lines 11-22 of nat_conf_file
    new_config_dict['output_params'] = {['daysnow':,
        'plusminus': int( old_conf[11].split()[0] ),
        'exkyes',
        'melyes',
        'loutyes',
        'swbyes',
        'netyes',
        'surftempyes',
        'shayes',
        'posyes',
        'diryes',
        'maxmeltstakes': int( old_conf[10].split()[0] ),
        'numbersnowdaysout',
        'surfyes',
        'difyes',
        'snowyes',
        'ablyes',
        'dir2yes',
        'latyes',
        'raiyes',
        'do_out',
        'linyes',
        'jd_snow_cover',
        'ddfyes',
        'enbyes',
        'senyes',
        'jd_surf_type',
        'gloyes',
        'albyes',
        'solyes']

# mass_balance_params
# energy_balance_params
# turbulence_params
# scaling_params
# input_files
# grid_info
# climate_data
# corrections_to_input
# lapse_rate
# surface_type_albedo
# radiation
# turbulence
# precipitation
# discharge
# discharge_initial
# optimization
# snow_model
# temperature_index
# output_stakes
