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

Using Git
---------
Git's a flexible tool, which allows for a wide variety of
work flows.  Here's one which serves well for working with DEBaM
and DETIM.

###Workflow:

1. __Branch__:
  When making changes, i.e. adding a feature, fixing a bug,
  or editing documentation, make a new branch off of
  the **relese_0.2** branch, and give it a descriptive name.
  Branches let you make changes and develop them separate
  from the main line of development. This way if your changes take
  awhile or you need to work on a different part of the project
  you can commit the changes you've made so far, and switch
  back to the release branch. Or if your changes simply don't
  work out, you can delete the changes you made by simply deleting the
  branch instead of having to track down every change made.

  Say we want to add some code for calculating skin temperatures,
  at the terminal just type:

        $ git branch skin_temperature
        $ git checkout skin_temperature
  or, we can use the equivalent one-liner:

        $ git checkout -b skin_temperature
  this will create a new branch, entitled ```skin_temperature```
  and make that the active branch. Now, any changes which we commit to
  the repository will be made to the new branch only.

  The title ```skin_temperature``` is just an example, we could have
  named the new branch anything,  but we should give the branches
 descriptive names; a branch in which we plan to fix a bug

2. __Make Changes__:
  Now that we're in a new branch, we can make changes at will, by
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

3. __Commit your Changes__:
  Once you're happy with your changes, it's time to assemble
  and commit your changes to the repository. Because Git doesn't
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
  has changed.

  So how do we make a commit? Assuming we've made some changes,
  we need to assemble a commit, by *staging* changes. Git requires
  you to explicitly add every change that you want included. Suppose
  we've created a new library called ```skintemperature.c``` and
  a corresponding header file ```skintemterature.h```. 
  * *Stage* These files are currently untracked by Git. We add them
    by staging them using the ```git add``` command.

            $ git add skintemperature.c skintemperature.h
  * From here we can use ```git status``` to see the status of
    the repository, if we decide we'd like to undo some changes
    or add more changes we still can before we make our commit,
    but we will need the stage these changes too, with another
    ```git add``` command.
  * *Commit* Once we've staged a collection of changes,
  we need to save these changes to the repository.
  Simply type

            $ git commit
  a text editor will open, and you should enter a descriptive message
  describing the changes made by this commit.
  If your message is sufficiently terse, you can enter it at the
  command line, with the ```-m``` option:

            $ git commit -m "Added skin temperature calculations."
  Now, if we check the status of the repository using ```git status```
  we will see that the changes we committed are no longer marked
  as modified.

4. __Make More Changes and Make More Commits__:
  We can now happily edit away, making more changes at will. Repeat
  steps 2 and 3 as needed, making new commits as you go.
 
4. __Push Branch to Github__:
  At some point you'll want to share you changes with others. If you
  have pulled the source from your own fork on Github this is
  as easy as

        $ git push origin skin_temperature
  where you should replace "skin\_temperature" with the name of the
  branch you've been working on.
  This tells git to push the changes you've made on the branch 
  "skin\_temperature"to the remote server named "origin".

  Until you are sure that you want to include the changes you've made
  in the release of the model, it's wise to keep your edits isolated
  in the branch you've made. Continue this by repeating steps
  2 - 5. When your code is ready to be included in the model's
  "official" code, proceed to 6.

5. __Deleting a Branch__:
  Git's branches are easy to make, and easy to delete. If we decide that
  the changes we've made on the branch aren't to our likeing, and
  want to get rid of them entirely, we can delete the entire branch. 
  Here we're going to delete the ```skin_temperature``` branch.
  We shoudld first make sure that we're on different branch from the one
  we're deleting:
 
        $ git checkout release_0.2
  we can then delete the branch in our working copy

        $ git branch -d skin_temperature

  This only deletes the local version of the branch; if this branch has 
  been pushed to Github, we can delete it there too. This should be done
  with caution. 

        $ git push origin :skin_temperature


6. __Submit a pull request__:
  A good way to let people know that you've been working on the model,
  and that your changes may be ready to be merged into the model, is to
  submit a Pull Request through Github. Once you've submitted a Pull
  request, people can review the changes, discuss modifications, and
  submit follow-up commits if need be.  Github's
  [help page](https://help.github.com/articles/using-pull-requests)
  on Pull Requests is a good place to learn more about Pull Requests.
 
7. __Merge Into Master__:
  Once changes have been reviewed and approved, our final task is to
  merge them into the main code repository. This can be done in
  two ways:
  1. Using the Pull Request Merge Button: Github provides an automated
     way to pull in changes via the Pull Request. If Git detects conflicts
     in the merge, you will be presented with instructions on manually proceeding
     with the merge.

  2. Manually merging via the commandline:
     Following the example we have been working with, to merge
     the changes we've made on the ```skin_temperature```
     branch into the ```release_0.2``` branch we would do the following
     from the command line:

            $ git checkout release_0.2 
            $ git merge skin_temperature

8. __Push to Github__:
  Finally, to push the changes we've made up to Github, we would do the following
  from the command line.

        $ git push origin release_0.2

### A Concise version of the above

__Making Changes___:

  1. Create a new branch:

        $ git branch <branch_name>

  2. Make Changes/ Add files

        $ git add <new/changed_filename>
  3. Check on what's added/changed/staged

        $ git status
  4. Commit changes

        $ git commit
  5. Push to Github

        $ git push origin
  6. Delete a local branch

        $ git branch -d <branch_name>
  7. Merge one branch (source) into another (target)

        $ git checkout <target_branch_name>
        $ git merge <source_branch_name>
