---
layout: front_page 
title: DEBaM and DETIM
subtitle: Contributing
---

Git and Github
--------------
We use [git](git-scm.com) to keep track of changes we make to
the model. Git is a revision control system, similar in purpose 
to SVN or CVS, but different in execution. 
If you're new to git, Github has some good documentation on
[setting up git](https://help.github.com/articles/set-up-git),
and [gitref](http://gitref.org/) is a good quick-reference page and has an
excellent git tutorial as well,
the git project's [documentation](http://git-scm.com/documentation)
is a good resource for more in-depth details on git's workings,
and Jan Krüger has a great
[git cheat sheet](http://jan-krueger.net/development/git-cheat-sheet-take-two)
if one needs a quick reference.

The model repository is hosted by [Github](http://github.com),
a webpage that offers project hosting, as well as a bevy of tools
for collaborating using git. Signing up, as well as hosting
open source projects, is free, and we strongly recommend that collaborators
sign up and use Github to assist in collaboration.


Versioning
----------
As of version 1.0.0 (Sep 2013), we've begun using [Semantic Versioning](http://semver.org/)
to dictate model version numbers. Roughly speaking, version numbers consist of
MAJOR.MINOR.PATCH, which are incremented when

1. MAJOR: when backwards incompatible changes are made

2. MINOR: when backwards-compatible functionality is added

3. PATCH: when backwards-compatible bug fixes are made.

The upshot of which is that your parameter files for model version
```1.0.3``` will not work with version ```3.6.4``` but will work
with ```1.2.0``` and ```1.0.4```.  If you are contributing, we
highly recommend you read the [full SemVer specification](http://semver.org).

The various versions of the model are marked in git with [tags](http://git-scm.com/book/en/Git-Basics-Tagging)
and can be [reviewed on Github](https://github.com/regine/meltmodel/releases).

Using Git to contribute to model development
--------------------------------------------
Git's a flexible tool, which allows for a wide variety of
work flows.  Here's one which serves well for working with DEBaM
and DETIM, i.e. if you want to contribute to the model by adding a feature, fixing a bug, 
editing documentation etc.

###Workflow:

1. __Fork__:
  Github allows you to create your own fork of the model code,
  and host it on Github as well. Think of your fork as your own version of
  the model, which you can modify, upload and share these modifications. Github also
  offers tools for combining changes from forks into the main project, as well
  as tools to discussing changes before they are merged. These tools make forking
  the project a great way to get started contributing, instructions for making your
  own fork on Github are located at [https://help.github.com/articles/fork-a-repo](https://help.github.com/articles/fork-a-repo).

  This only needs to be done the first time you start working with the
  model code; your fork will need to be updated occasionally to keep up
  to date with changes to the model, but you shouldn't need to re-fork the
  model.
  
  How to create a fork: 
   - Got to the (model github page)[https://github.com/regine/meltmodel]
   - click on 'Fork'
   This makes your own fork and at the same time copies the entire github model repository
   (including all branches) to your fork on github.
   
2. __Download a copy of your fork to your local computer__
   open terminal
   
       $ git clone https://github.com/USERNAME/meltmodel.git
   (Replace 'USERNAME' with your user name)
   Now you a local copy of your Github fork on your computer.
   
3. __Branch__:
  Make a new branch off of
  the **master** branch of your local copy, and give it a descriptive name.
  Branches let you make changes and develop them separate
  from the main line of development. This way if your changes take
  a while or you need to work on a different part of the project
  you can commit the changes you've made so far, and switch
  back to your master branch for model application. Or if your changes don't
  work out, you can delete the changes you made by simply deleting the
  branch instead of having to track down every change made.

  Say we want to add a new parameterization to compute longwave radiation,
  at the terminal (in folder meltmodel) make a new branch by typing:

        $ git branch longwave
        $ git checkout longwave
  or, we can use the equivalent one-liner:

        $ git checkout -b longwave
  this will create a new branch, entitled ```longwave```
  and make that the active branch. Now, any changes which we commit to
  the repository will be made to the new branch only.

  The title of the branch ```longwave``` is just an example, we could have
  named the new branch anything, but we should give the branches
 descriptive names.

4. __Make Changes to the source code__:
  Now that we're in a new branch on our local computer, we can make changes at will, by
  editing, adding or removing files.
  Git will recognize files in the directory as being in one of
  four statuses:
    - *Untracked:* Git is not tracking this file, and it is not
      included in the repository.
    - *Unmodified:* Git is tracking this file, and it has not
      changed since the most recent commit.
    - *Modified:* This file is being tracked, and has changed
      since the last commit, but Git has not been told to
      include these changes.
    - *Staged:* Some changes made to this file have been staged,
      and these changes will be included the next time a commit
      is made.
  
  At the command line, you can view the status of your files
  via the command

        $ git status 
  Editing a tracked, but unmodified, file will change its status
  to modified, but these changes will not be tracked by Git until we
  commit them to the repository. 

5. __Commit your Changes__:
  Once you're happy with your changes, it's time to assemble
  and commit your changes which means that you can browse the history of changes in the code
  as the changes become part of the version control. Because Git doesn't
  force us to save all of the changes we've made at the same time,
  we can separate our edits into several commits. If we think 
  of a single commit as a snapshot of the project at a particular
  point in time, it's a good idea to ensure that every commit
  we make represents a single change to the project, be it a
  bugfix, a new feature or one rearrangement. One benefit
  of making frequent, atomic commits, is that when bugs are found,
  and you need to back-track to find its source, it is much easier
  to back track through a series of small changes to root out the
  offending changes than it is to pick apart a large commit, where
  several desperate things have been changed. It's not a bad idea
  to make commits at semi-regular intervals while you're working
  on the code; once you've finished a small task, make a commit.
  This way your commit history presents a history of how the code
  has changed. You may want to compile the model before committing your changes
  (go to the meltmodel folder and type 'make models')

  So how do we make a commit? Assuming we've made some changes,
  we need to assemble a commit, by *staging* changes. Git requires
  you to explicitly add every change that you want included. Suppose
  we've only changed the source file ```radiat.c``` and
  the corresponding header file ```radiat.h```. 
  * *Stage* These files are currently untracked by Git. We add them
    by staging them using the ```git add``` command.

            $ git add radiat.c radiat.h
  * If we added a new source file, e.g. ```longwave.c``` we add that file including the header file,     
  * From here we can use ```git status``` to see the status of
    the local repository, if we decide we'd like to undo some changes
    or add more changes we still can before we make our commit,
    but we will need to stage these changes too, with another
    ```git add``` command.
  * *Commit* Once we've staged a collection of changes,
  we need to save these changes to the repository.
  Simply type

            $ git commit
  a text editor will open, and you should enter a descriptive message
  describing the changes made by this commit.
  If your message is sufficiently terse, you can enter it at the
  command line, with the ```-m``` option:

            $ git commit -m "Added new longwave radiation parameterization."
  Now, if we check the status of the local repository using ```git status```
  we will see that the changes we committed are no longer marked
  as modified.

6. __Make More Changes and Make More Commits__:
  We can now happily edit away, making more changes at will. Repeat
  steps 2 and 3 as needed, making new commits as you go.
 
7. __Push Branch to Github__:
  At some point you'll want to push the entire branch that contains your local changes 
  to your own github repository which allows you to access the content from any other
  computer and also makes your changes visible to others. If you
  have pulled the source from your own fork on Github (see step 2 above) this is
  as easy as

        $ git push origin longwave
  where "longwave" is our example of the name of the branch and needs to be replaced
  by the name of the branch you are using.
  This tells git to push the changes you've made on the branch 
  "longwave" to the remote server named "origin", which is by default the server you 
  cloned the model from.

  Until you are sure that you want to include the changes you've made
  in the master branch of your fork, it's wise to keep your edits isolated
  in the branch you've made. Continue this by repeating steps
  2 - 5. When your code is ready to be included in the model's
  "official" code (master version), proceed to 9.

8. __Deleting a Branch__:
  Git's branches are easy to make, and easy to delete. If we decide that
  the changes we've made on the branch aren't to your likeing, and
  want to get rid of them entirely, we can delete the entire branch. 
  Here we're going to delete the ```longwave``` branch.
  
 
 	1. __Deleting a Branch Locally__:
 	We should first make sure that we're on a different branch from the one
    we're deleting. Also you have to commit all your changes or stash them 
    before you can switch branches:
        
        	$ git checkout master
  we can then delete the branch in our working copy

        	$ git branch -D longwave
        
	2. __Deleting a Branch on Github__:
  The above only deletes the local version of the branch. If this branch has 
  been pushed to Github, we can delete it there too. This should be done
  with caution. 

        	$ git push origin :longwave


9. __Submit a pull request__:
  A good way to let people know that you've been working on the model,
  and that your changes may be ready to be merged into the master version on github, is to
  submit a Pull Request through Github. Once you've submitted a Pull
  request, people can review the changes, discuss modifications, and
  submit follow-up commits if need be.  Github's
  [help page](https://help.github.com/articles/using-pull-requests)
  on Pull Requests is a good place to learn more about Pull Requests.
 
   How to make a pull request:
    Go to your fork on github and change into the branch that you want to merge with the 
    'official' master version.
    click on 'submit pull request'.

   This will notify the model administrators and create a page where people can discuss
   the proposed model changes.
 
10. __Merge Into Master__:
  Once changes have been reviewed and approved, our final task is to
  merge them into the main code repository. This can be done in
  two ways:
  1. Using the Pull Request Merge Button: Github provides an automated
     way to pull in changes via the Pull Request. If Git detects conflicts
     in the merge, you will be presented with instructions on manually proceeding
     with the merge.

  2. Manually merging via the commandline:
     Following the example we have been working with, to merge
     the changes we've made on the ```longwave```
     branch into the ```master``` branch we would do the following
     from the command line:

            $ git checkout master 
            $ git merge longwave

11. __Push to Github__:
  Finally, to push the changes we've made up to Github, we would do the following
  from the command line.

        $ git push origin master

### A Concise version of the above

__Making Model Changes__:

  1. Create a new branch:

        $ git branch <branch_name>

  2. Check out a branch:
      
        $ git checkout <branch_name>
  3. Make Changes/ Add files

        $ git add <new/changed_filename>
  4. Check on what's added/changed/staged

        $ git status
  5. Commit changes

        $ git commit
  6. Push to Github

        $ git push origin
  7. Delete a local branch

        $ git branch -d <branch_name>
  8. Merge one branch (source) into another (target)

        $ git checkout <target_branch_name>
        $ git merge <source_branch_name>
