#!/bin/bash
## By Seobryn

##------------------------------- VARIABLES ---------------------------------##

MODULE_TEMPLATE_URL=""

##------------------------------- CODE ---------------------------------##

read -p "Enter the name of your future module: " "MODULE_NAME"
echo $MODULE_NAME | fgrep -q ' '

while [ $? -eq 0 ]
do
  echo 'Blanks are not allowed!'
  read -p "Enter the name of your future module: " MODULE_NAME
  echo $MODULE_NAME | fgrep -q ' '
done

if test -n "$MODULE_NAME"
then
    echo "--- Cloning 'skeleton-module' as $MODULE_NAME/"
    git clone --depth 1 -b master $MODULE_TEMPLATE_URL $MODULE_NAME

    echo "--- Removing 'skeleton-module/.git/' history"
    cd $MODULE_NAME && rm -rf .git/

    echo "--- Init new git repository"
    git init && git add -A && git commit -m "Initial commit - $MODULE_NAME"

    echo "--- Ready to code"
fi
