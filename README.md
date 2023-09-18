# Flappy Bird
- A replication of the mobile game Flappy Bird with reinforcement learning.

## Description
- Reproduced in C++
- Flappy Bird has two main files: Engine and Flappy
   - Flappy: Contains sprite/texture, movement logic, and information about the bird.
   - Engine: Contains the brunt of the overarching logic
      - Has AI logic, background movement, window settings, pipe logic, reset, collisions, and much more 
- Currently implemented/implementing:
   - Reinforcement-learning with Q-learning and Epsilon-Greedy algorithms.
      - Unordered map with custom hash function used for Q-table and N-table.
      - Split into three main parts:
         - Initialize: Checks whether the tuple has been registered in Q-table and/or N-table.
         - Action: Decide whether to explore or exploit based on hyperparameters.
         - Learn: Updates the tables with new values.

## Specs
- Used Visual Studios 2022.
- Used Visual C++ 15 (2017) - 32-bit.

## Sources
- Click "Latest Stable Build" to find the different SFMLs [here](https://www.sfml-dev.org/download.php).
- Steps on how to [download and prep SFML into Visual Studios](http://gamecodeschool.com/sfml/setting-up-visual-studio-and-sfml-development-environment/).
- Steps on how to [create an SFML template in Visual Studios](http://gamecodeschool.com/sfml/building-your-first-sfml-game-project/).


