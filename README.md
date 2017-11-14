# Bug Blast

## Synopsis 

*Bug Blast* is a course project assigned in an undergraduate computer
science [class](https://goo.gl/pL3s9R) centered around basic data strutures,
algorithms, and object-oriented programming. It is a 2D game :video_game: that
features the player attempting to exterminate enemy bugs by dropping bug sprays
while trying to avoid being infested. The different components of this game are
derived from the original [Bomberman](https://en.wikipedia.org/wiki/Bomberman)
game, created by [Hudson Soft](https://en.wikipedia.org/wiki/Hudson_Soft), a
Japanese video game company.

![screenshot](hub/bug_blast_animated.gif "Bug Blast")

## Gameplay

The player navigates through a series of mazes using bug sprays to exterminate
bugs called *Zumi*. In order to advance to the next maze, the player must first
exterminate all of the *Zumi* before an exit square appears to move on to the
next level.

If the player is on the same square as a *Zumi* at any given time,
the player loses a life. The player starts with 3 lives.

When the player drops a bug sprayer, a timer :clock5: will count down, and
after a specified amount of time, the sprayer will spray poison mist onto the
surrounding squares. Any *Zumi* on any of those squares when the timer expires
will be exterminated. The player can also be killed by this bug sprayer when
it goes off, so be careful! In addition, any destroyable bricks can be
destroyed by these detonated bug sprayers. The player can only drop 2 bug
sprayers at a time.

There are two types of *Zumi*, *Simple Zumi* and *Complex Zumi*. *Simple Zumi*
are essentially blind and only move in straight lines. *Complex Zumi*, on the
other hand, move aggressively toward the player when they are within a
particular smelling distance of the player.

### Controls

The following shows the common controls in this game. 

| Action                   | Key Binding |
| ------------------------ | ----------- |
| Move up :arrow_up:       | `W`         |
| Move down :arrow_down:   | `S`         |
| Move left :arrow_left:   | `A`         |
| Move right :arrow_right: | `D`         |
| Quit                     | `Q`         |
| Drop bug spray           | `[SPACE]`   |

 Alternatively, the arrow keys of the keyboard can be used to move the player
 avatar around.

### Goodies :candy:

When Zumi are exterminated, they may drop a goodie. There are three kinds of
goodies in this game.

An *Extra Life* goodie gives the player an extra life.

A *Walk Through Walls* goodie allows the player to walk through destroyable
bricks. However, it only lasts for a limited amount of time once it is
retrieved. If the player is on the same square as a destroyable brick when the
power of the goodie goes away, the player will lose a life. Be careful!

A *More Sprayers* goodie allows the player to drop more bug sprayers at once
for a limited amount of time.

### Scoring

The following shows the points distribution in the game.

| Action                   | Points |
| ------------------------ | ------ |
| Destroy *Simple Zumi*    | 100    |
| Destroy *Complex Zumi*   | 500    |
| Pick up goodie           | 1000   |

Every level also has bonus points that the player can earn after completing
each level. However, the amount of bonus points that can be earned in a level
is reduced by one point for each tick of the game. Whatever bonus remains when
the maze is completed is added to the total score.

### Customizing Levels

New levels can be customized and added to this game by specifying the
parameters of each level in a separate data file. The following shows the
data file for Level 0.

```
probOfGoodieOverall=100
probOfExtraLifeGoodie=0
probOfWalkThruGoodie=100
probOfMoreSprayersGoodie=0
ticksPerSimpleZumiMove=5
ticksPerComplexZumiMove=10
goodieLifetimeInTicks=40
levelBonus=1000
walkThruLifetimeTicks=200
boostedSprayerLifetimeTicks=200
maxBoostedSprayers=6
complexZumiSearchDistance=5

###############
#    * @ *    #
# ####   #### #
# * s*   *  * #
# *  *   *  * #
#    #####    #
#             #
#             #
#             #
#             #
# #    *    # #
#*************#
#   *  #  *   #
#  *   e   *  #
###############
```

The parameters and layout of the maze **must** be separated by a new line in
order for the data file to be parsed correctly. In addition, all parameter
values **must** be non-negative integers.

The following describes the first few parameters related to the likelihood of
some goodie being dropped when a Zumi is exterminated.

| Parameter                  | Probability of...                              |
| -------------------------- | ---------------------------------------------- |
| `probOfGoodieOverall`      | Any goodie being dropped                       |
| `probOfExtraLifeGoodie`    | *Extra Life* goodie being dropped              |
| `probOfWalkThruGoodie`     | *Walk Through Walls* goodie being dropped      |
| `probOfMoreSprayersGoodie` | *More Sprayers* goodie being dropped           |

The values of `probOfExtraLifeGoodie`, `probOfWalkThruGoodie`, and
`probOfMoreSprayersGoodie` are probability values given that some goodie is
dropped, and **must** add up to 100 (100%).

The following describes some other parameters related to speed or duration of
different objects in the game.

| Parameter                     | Meaning (in ticks)                         |
| ----------------------------- | ------------------------------------------ |
| `ticksPerSimpleZumiMove`      | How fast *Simple Zumi* move                |
| `ticksPerComplexZumiMove`     | How fast *Complex Zumi* move               |
| `goodieLifetimeInTicks`       | How long goodies last after being dropped  |
| `walkThruLifetimeTicks`       | How long *Walk Through Walls* goodies last |
| `boostedSprayerLifetimeTicks` | How long *More Sprayers* goodie last       |

The following describes other miscellaneous parameters.

| Parameter                   | Meaning                                       |
| --------------------------- | --------------------------------------------- |
| `levelBonus`                | Maximum bonus points                          |
| `maxBoostedSprayers`        | Number of sprayers for *More Sprayers* goodie |
| `complexZumiSearchDistance` | How far *Complex Zumi* can smell              |

The following describes what each character in the maze layout represents.

| Object                      | Character |
| --------------------------- | --------- |
| Permanent brick             | `#`       |
| Destroyable brick           | `*`       |
| Starting location of player | `@`       |
| Simple *Zumi*               | `s`       |
| Complex *Zumi*              | `c`       |
| Exit                        | `e`       |

The specified maze **must** be 15 squares wide by 15 squares high. The
perimeter of the maze **must** be surrounded by permanent bricks. Any space
characters in the maze layout represents space in which the player and Zumi
can walk.

## Development

The code for this project is adopted from skeleton code provided by
[Carey Nachenberg](http://careynachenberg.weebly.com/) and
[David Smallberg](http://www.bruinwalk.com/professors/david-a-smallberg/).
The skeleton code provides functionality for graphics and audio.

This project can be developed on the latest version of Visual Studio or Xcode.
For further instructions on setup for either IDE, please refer to the `README`
in their respective folder.

