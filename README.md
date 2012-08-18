This is the source for DeTIM and DEBaM's web-page.

The Basics:
-----------

The page is generated from source by [Jekyll](http://jekyllrb.com/), and uses 
the Jekyll plugin [Jekyll-scholar](https://github.com/inukshuk/jekyll-scholar)
to handle bibliography and citations.

Github pages runs off of Jekyll, but doesn't allow plugin execution,
necessitating off-line page compilation. First, we need to install Jekyll
and Jekyll-scholar. Luckily, this is pretty easy.

Jekyll is a program written in Ruby which generates static web-pages
from a variety of different source formats. In particular, we can write pages
in Markdown and generate valid html files.

Jekyll-scholar is a plugin for Jekyll, that allows us to use bibtex files
to generate citations and bibliographies.

If you already have Jekyll and Jekyll-scholar installed, skip down to
__Building the Site__.


Getting Ruby and Ruby-Gems
---------------------------------

- __OS X__: This is perhaps the easiest situation, since Ruby comes
preinstalled on OS X. 
    
- __Linux__: Since many distros don't include ruby by default, you'll need
to install it using your package manager.
    * __apt__ (Ubunutu, Debian etc.), do
        $ sudo apt-get install ruby
        $ sudo apt-get install ruby-gems


Getting Jekyll and Jekyll-scholar Using Ruby-Gems
-------------------------------------------------
These instructions should work regardless of your platform

* Open a terminal window, we're going to use ```gem```, Ruby's 
package manager, to install both Jekyll and Jekyll-scholar.
* Type:
    $ gem install jekyll
    $ gem install jekyll-scholar
* And that's it! If you receive an error telling you that you don't
have permission to install the gems, add ```sudo``` to the beginning of
each command.


Building the Site
-----------------

- Now that we have Jekyll and Jekyll-scholar set up, open your terminal
and navigate to the melt_mod source directory (the one containing this
README file).
- At the terminal enter
    $ jekyll
- Some information should be displayed in the terminal, and you should be
returned to a comamnd prompt.
- If everything has gone sucessfully, there should be a new directory
```_site```, the contents of which are the homepage for the model!
