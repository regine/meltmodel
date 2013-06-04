---
layout: front_page
title: DEBaM and DETIM
subtitle: Making Changes to This Webpage
---
The Basics:
-----------

This page is designed to be used by [Github Pages](http://pages.github.com/),
using [Jekyll](http://jekyllrb.com/), a program written in Ruby
which generates HTML from a variety of different
source formats. In particular, we can write pages in 
[Markdown](http://daringfireball.net/projects/markdown/basics)
and generate valid html files. 


Prerequisites
-------------

To make basic edits, you don't *need* to have
Jekyll installed on your machine, you can simply edit the files,
make a commit and push them to Github; Pages will generate the
webpage, and you can see how it looks live. For more substantial
work, it make sense for us to have a version of Jekyll installed
so that we can preview work, and iron out the kinks before making
changes public.

The default library that Jekyll uses to parse Markdown behaves
irregularly, so we will also install Redcarpet, an optional
Markdown library, supported by Github and Github Pages.

We'll use Ruby-Gems, a package manager for the Ruby programming
language, and it's command-line program ```gem``` to install Jekyll
and Redcarpet.

If you already have Ruby, Ruby Gems, and Jekyll installed, you
can feel free to skip to Editing the Site.



### Getting Ruby and Ruby-Gems
- __OS X__: This is perhaps the easiest situation, since Ruby comes
preinstalled on OS X. You should update ```gem``` though. At a command prompt,
do 

        $ gem update --system

If you recieve an error telling you you do not hav to correct file
permissions, you may need to add ```sudo``` to the beginning of
this command.
    
- __Linux__: Since many distros don't include Ruby by default, you'll need
to install it using your package manager.

    * __apt__ (Ubunutu, Debian etc.), do

            $ apt-get install ruby
            $ apt-get install ruby-gems
    
    * __pacman__ (ArchLinux and derivatives) do

            $ pacman -S ruby


### Getting Jekyll and Redcarpet Using Ruby-Gems
These instructions should work regardless of your platform

* Open a terminal window, we will use ```gem```, to install
Jekyll and Redcarpet
* Type:

        $ gem install jekyll
        $ gem install redcarpet

* And that's it! Again, it's possible that you may need to add ```sudo```
to the beginning of these commands to install the packages. 

Editing the Site
----------------

### Download a Copy of the Webpage
Github has a system known as Pages, to automatically generate webpages
for projects which are hosted there.  Pages uses a git branch
of the project called ```gh-pages``` to store the project webpage.
If you would like to make edits to the webpage, open your terminal,
and start with a fresh clone of the project:

    $ git clone https://github.com/regine/meltmodel.git

Now that we have a clean working copy, we need to navigate into it
and check out the ```gh-pages``` branch.
    
    $ cd meltmodel
    
    $ git checkout gh-pages


### Making Edits
This webpage is mostly written in Markdown,
with some HTML templates and CSS, and converted
to HTML by [Jekyll](http://jekyllrb.com). Major edits
to the page will require some familiarity
with how Jekyll works, you can read its documentation at
[http://jekyllrb.com](http://jekyllrb.com).

Any file in the directory root not prefixed by an
underscore will be procedded by Jekyll and included
in the final wepage. At the moment, these consist of
Markdown files as well as a CSS file which defines
how the page is displayed. Any of these files may be
edited with your favorite text-editor.

The ```_layouts``` directory contains the HTML
templates which wrap around the pages in the site.
More information about the directory structure
is availible [here](http://jekyllrb.com/docs/structure/).

### Building the Site

Once we have made edits, if we would like to verify
that the changes we've made look whe way we want
them to, we can use Jekyll to generate the HTML files
which will be served on Github.  The generated webpage
will be located in a the directory ```_site```,
if this directory does nto exist, it will be created,
it if does exist, its contents will be removed.

- At the terminal enter

        $ jekyll build

- Some information should be displayed in the terminal, and you will be
returned to a command prompt.
- If everything has gone successfully, there should be a new directory
```_site```, the contents of which are the homepage for the model!
You can now preview the site, make edits and repeat.

Jekyll comes with a few useful comand-line options.

  * When working, it can get tiresome to rebuild the site
  constantly, the ```--watch``` option tells Jekyll to watch
  the directory, and rebuild the site every time a file is changed.
  At the command prompt, type

          $ jekyll build --watch 
  
  * While one can open the generated HTML files and preview them
  in a browser, it's a better idea to preview them by having a
  web-server serve up the pages, this ensures things like CSS
  and Javascript files used by the page are found correctly.
  Jekyll comes with a small webserver built in, which we can use
  to preview the webpage before finalizing any changes. By typing

          $ jekyll serve

    at our command prompt; Jekyll will generate the
    webpage, and serve it locally. In your web-browser, 
    browse to [http://localhost:4000](http://localhost:4000)
    to see what the site will look like when posted ot Github. 
    When you are done, open the terminal window in which you
    ran jekyll, and stop the server by typing ```Ctl-C```.
    

  * We can combine these two, and Jekyll will both
  serve the webpage locally, and watch for changes,
  and regenerate the site as we make changes. Type

          $ jekyll serve --watch

  Now, after making and saving changes, reload the browser
  window which is previewing the webpage, your changes
  sould appear.

  

### Uploading the Site to GitHub Pages

Once we are finished editing the site, we need to
upload it to Github. This is done by making a new
commit to the ```gh-pages``` branch using git.

- Generate the final page to be uploaded, make sure to preview it just in
case there's some subtle syntax error somewhere.
- Commit these changes; we'll use the `-a` flag to add all changes made

        $ git commit -a

- Push these changes to the server:

        $ git push origin gh-pages 

- Check the project website and make sure the changes have posted. If not, check
Github's [status page](http://status.github.com) to see if there's an outage.

- The full documentation for Github Pages can be viewed [here](http://help.github.com/pages/)
