#!/bin/sh
git branch -a
git checkout master
git checkout $1
git rebase master $1
git branch
git log -4
git log --pretty=format:"%s%b" master..HEAD | ( git reset --soft master && git commit -F - )
git log -4
