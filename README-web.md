---
layout: front_page
title: DEBaM and DETIM
subtitle: Making Changes to This Webpage
---
The Basics:
-----------

Beginning in January 2015, we've migrated this page from markdown-based format
centered around Jekyll and Gihthub Pages to a more vanilla, html-based format.
We still host the page on Github, but now all editing happens in raw html
files, which no longer require any generation for previewing.

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
the code in branch ```<branch name>```. This does not work if changes have been
made to files in the repository that have not been committed yet.
A workaround is to temporarily stash the changes by the command ```git stash```.
Now you can switch branches. Once you switch back you can
retrieve the changes by ```git stash pop```. ```git stash list``` gives you a
list of what is in the stash.
Alternatively (recommended), instead of switching between gh-pages and the
release branch, checkout out ```gh-pages``` and the release in different folders,
i.e. download 2 different copies.
More info about git can be found at the [git project](http://git-scm.com).

It's common that details
regarding the compilation or use of the model change, and they need
to be documented somewhere. A good idea is to first document these
in the text and .md files included with the model source code;
this helps ensure that when someone download your code, they've also
recieved instructions on how to get it up and running. This is especially
important for users who may be using older code, with instructions
differing from those posted to the webpage. Doing this makes it easier
to update the webpage as well, since many of the pages here correspond
to files inculded with the model source code. Below is a list
of files which roughly corrsepond

<table>
  <thead>
    <tr>
      <th>gh-pages</th>
      <th>Model Source</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>index.html</td>
      <td>README.md</td>
    </tr>
    <tr>
      <td>install-web.html</td>
      <td>INSTALL.md</td>
    </tr>
  </tbody>
</table>

The contents of the above file should be more or less the same
for the currently released version of the model.

### Making Edits
This webpage is written in HTML, and styled with a CSS file.
These files may be modified in any text editior suitable for editing source code.

### Previewing the Site

Once we have made edits, it's wise choice is to preview them in a web-browser
to verify that they look correct. Simply open the .html file you've been editin
using your favorite browser.


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
