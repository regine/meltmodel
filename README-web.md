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
and generate valid HTML files. 


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

If you receive an error telling you you do not have to correct file
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
Jekyll and Redcarpet. Again, it's possible that you may need
to add ```sudo``` to the beginning of these commands to install
the packages.
* Type:

        $ gem install jekyll
        $ gem install redcarpet

* And that's it! It's not uncommon that your packages will
be installed successfully, but an error installing
the documentation will be displayed, this is usually fine;
unless gem exits early, citing an installation error,
your installation should be fine.

* __Caveat__: Github doesn't always use the most recent
version of jekyll or redcarpet for pages, to ensure
that the versions of these packages match the versions
used by Github, take a peek at
(their documentation)[https://help.github.com/articles/using-jekyll-with-pages]
for using Jekyll and Pages, as of this writing, the versions
may be found in the
(github-pages gemspec)[https://github.com/github/pages-gem/blob/master/github-pages.gemspec#L16].
If indeed an older version is called for, say jekyll needs to be at version 1.1.2,
and redcarpet needs to be version 2.2.2
it's possible to install these specifc versions via

        $ gem install jekyll --version "=1.1.2"
        $ gem install redcarpet --version "=2.2.2"

 
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

The ```gh-pages``` is what is known as a "orphan branch",
and its contents are unrelated to the main line of
development for the model. By switching into the ```gh-pages```
branch, git will replace the contents of the ```meltmodel```
folder downloaded from Github (by default this will be the
current version of the models' source code) with the source
for the webpage. If you need to get back to the model code
type ```git branch``` to see the list of branches tracked by
git, and then ```git checkout <branch name>``` to checkout
the code in branch ```<branch name>```. This does not work if changes have been made to files in the repository that have not been committed yet.
A workaround is to temporarily stash the changes by the command ```git stash```. Now you can switch branches. Once you switch back you can
retrieve the changes by ```git stash pop```. ```git stash list``` gives you a list of what is in the stash. Remove _site before switching back to the model release branch.
Alternatively (recommended), instead of switching between gh-pages and the release branch, checkout out ```gh-pages``` and the release in different folders, i.e. download 2 different copies.
More info about git can be found at the (git project)[http://git-scm.com].

It's common that details
regarding the compilation or use of the model change, and they need
to be documented somewhere. A good idea is to first document these
in the text and markdown files included with the model source code;
this helps ensure that when someone download your code, they've also
recieved instructions on how to get it up and running. This is especially
important for users who may be using older code, with instructions
differing from those posted to the webpage. Doing this makes it easier
to update the webpage as well, since many of the pages here correspond
to files inculded with the model source code. Below is a list
of files which roughly corrsepond with oneanother

<table>
  <thead>
    <tr>
      <th>gh-pages</th>
      <th>Model Source</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>index.md</td>
      <td>README.md</td>
    </tr>
    <tr>
      <td>install-web.md</td>
      <td>INSTALL.md</td>
    </tr>
  </tbody>
</table>

The contents of the above file should be more or less the same
for the currently released version of the model.

### Making Edits
This webpage is mostly written in Markdown (files end with ```.md```),
with some HTML templates and CSS, and converted
to HTML by [Jekyll](http://jekyllrb.com). To edit the .md file, open them in any text editor and edit them.

Major edits
to the page will require some familiarity
with how Jekyll works, you can read its documentation at
[http://jekyllrb.com](http://jekyllrb.com).

Any file in the directory root not prefixed by an
underscore will be processed by Jekyll and included
in the final Webpage. At the moment, these consist of
Markdown files as well as a CSS file which defines
how the page is displayed. Any of these files may be
edited with your favorite text-editor.

The ```_layouts``` directory contains the HTML
templates which wrap around the pages in the site.
More information about the directory structure
is available [here](http://jekyllrb.com/docs/structure/).

### Building the Site

Once we have made edits, if we would like to verify
that the changes we've made look the way we want
them to, we can use Jekyll to generate the HTML files
which will be served on Github.  The generated webpage
will be located in a the directory ```_site```,
if this directory does not exist, it will be created,
it if does exist, its contents will be removed.

- At the terminal enter

        $ jekyll build

- Some information should be displayed in the terminal, and you will be
returned to a command prompt.
- If everything has gone successfully, there should be a new directory
```_site```, the contents of which are the homepage for the model!
You can now preview the site by opening the HTML files in ```_site```,
make edits and repeat.

Jekyll comes with a few useful command-line options.

  * When working, it can get tiresome to rebuild the site
  constantly, the ```--watch``` option tells Jekyll to watch
  the directory, and rebuild the site every time a file is changed.
  At the command prompt, type

          $ jekyll build --watch 

  Jekyll will now run indefinitely, regenerating the webpage
  each time you save changes to the source. To stop Jekyll:
  type ```Ctl-c``` in the terminal window in which Jekyll is running.
  
  * While one can open the generated HTML files and preview them
  in a browser, it's a better idea to preview them through a
  web-server; this ensures things like CSS
  and Javascript files used by the page are found correctly.
  Jekyll comes with a small webserver built in, which we can use
  to preview the webpage before finalizing any changes. By typing

          $ jekyll serve

    at our command prompt; Jekyll will generate the
    webpage, and start a webserver, which will serve
    the webpage locally. In your web-browser, 
    browse to [http://localhost:4000](http://localhost:4000)
    to see what the site will look like when posted to Github. 
    When you are done, open the terminal window in which you
    ran jekyll, and stop the server by typing ```Ctl-c```.
    

  * __Recommended__: We can combine these two, and Jekyll will both
  serve the webpage locally, and watch for changes,
  and regenerate the site as we make changes. Type

          $ jekyll serve --watch

  Now, after making and saving changes, reload the browser
  window which is previewing the webpage, your changes
  should appear. Again, typing ```Ctl-c``` in the appropriate
  terminal will stop the jekyll process.

  

### Uploading the Site to GitHub Pages

Once we are finished editing the site, we need to
upload it to Github. This is done by making a new
commit to the ```gh-pages``` branch using git.

- Commit these changes; we'll use the `-a` flag to add all changes made

        $ git commit -a

  git will open an editor, and you should enter a brief
  message describing the changes you have made.
  If the message is sufficiently short, you can bypass the
  editor, and enter you message on the commandline using the ```-m```
  command-line option

        $ git commit -a -m "<commit-message>"

- Push these changes to the server:

        $ git push origin gh-pages 

- Check the project website and make sure the changes have posted. If not, check
Github's [status page](http://status.github.com) to see if there's an outage.

- The full documentation for Github Pages can be viewed [here](http://help.github.com/pages/)
