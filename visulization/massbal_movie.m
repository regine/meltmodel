
for year = (1961:2010)
    filename = ['/Users/cminor/Documents/work/hock_mass_balance/code/'...
        'melt_mod/lemon_example/output/massbal' sprintf('%i',year) '_243.bin'];
    grid(filename, 1);
    pause(0.1);
end
    