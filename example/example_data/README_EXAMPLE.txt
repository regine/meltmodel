README EXAMPLE

How to get the example running on your computer:

1.) Copy the input.txt in the example folder into the model's bin folder where the 2 model's executables (detim, debam) are.

2.) Make an output folder anywhere (for example, output_data under the example folder.

3.) Adjust input.txt: adjust the name of the path where the example_data is and the name of the output folder that you created
(both at the very top of input.txt)

4.) Go back to the bin-folder and run detim or debam at a terminal

That's it. The model should work now with the parameters given.
All model output files should be in the output folder that you created and specified in input.txt.


NOTE:

1.) that if you experiment with other parameters in input.txt it may not work. 
For example there is no sky view grid file and therefore some of the energy balance options won't work.

2.) that the example mixes data from different applications (regions). 
It gives you an idea how output files look like but the numbers are not necessarily realistic. 
For example, the file cummassbal.txt contains zero for a long period, simply because it is too cold to produce 
any melt and there is no precipitation. The balances over the modeled period are positive.

3.) For your own application take this example file and adjust the parameters for your case.