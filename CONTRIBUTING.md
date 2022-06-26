# Contributing

So, you want to contribute? Great!
Contributing is not only about creating fixes, but also reporting bugs. Before reporting a bug, please make sure to use the latest core and database revision.

# Issues

TBD

# Mandatory things when creating a ticket:

-   Branch
-   commit hash (if you get something like FirelandsCore rev. unknown 1970-01-01 00:00:00 +0000 (Archived branch) (Win64, Release).
-   entries of affected creatures / items / quests with a link to the relevant wowhead page.
-   clear title and description of the bug - if your english is very bad, please use google translate or yandex to translate to english and include one text in your native language.

When reporting a crash, you MUST compile in debug mode because release dumps are useless (not enough information).

We sugest the title and body to have the next style:

DB/Quest: The Collapse

4.3.4 branch
hash 63f96a282307
The quest "The Collapse" http://www.wowhead.com/quest=11706 lacks final event.

# Creating Pull Requests:

1. Fork it.
2. Create a branch (`git checkout -b fix/general-fixes`) (Note: general-fixes is an arbitrary name, but the first part is based on Gitflow)
3. Commit your changes (`git commit -am "(feat): Added Snarkdown"`)
4. Push to the branch (`git push origin fixes`)
5. Open a Pull Request

When creating patches read:

-   [FirelandsCore Development Standards](TBD)
-   [WDB Fields](TBD)
-   [Git Squash](TBD)

We suggest that you create one branch for each C++ based fix: this will allow you to create more fixes without having to wait for your pull request to be merged.
For the SQL files coming with C++ based fixes the naming schema is `YYYY_MM_DD_i_database.sql`, where `YYYY_MM_DD` is the date of the fix, `i_database` is the _ith_ sql created that day for `database`.
When doing changes to `auth` or `characters` database remember to update the base files (`/sql/base/*`).
For SQL only fixes, please [create a ticket](TBD).
Since it's very unlikely that your Pull Request will be merged on the day that you open it, please name the files with an impossible date to avoid merging issues ie: 2015_13_32_00_world.sql

# Wiki

The wiki is located at [https://github.com/FirelandsProject/firelands-wiki](https://github.com/FirelandsProject/firelands-wiki).

You are welcome to create an account and help us improve and extend the wiki.

# Requirements

Software requirements are available in the [wiki](https://github.com/FirelandsProject/firelands-wiki) for
Windows, Linux and Mac OSX.

If you choose Linux, we recommend to use Debian 8, since it's the Linux that we use to test compilations.
